/*
 * I2C.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Shuhang Tan
 *      Reference: https://github.com/uvic-auvic/Firmware-Motor_Controller_2.0/blob/master/src/I2C.c
 *                 (I2C code for motor controller 2)
 */

#include "I2C.h"
#include "assert.h"
#include "utils.h"
#include "RCCHelper.h"
#include <string.h>
#include <stdlib.h>
#include "LED.h"

// I2C SDA and SCL pins
#define _I2C_SDA_GPIO  (GPIO_Pin_9) // PC9
#define _I2C_SCL_GPIO  (GPIO_Pin_8) // PA8

// I2C sendBuffer size
#define _I2C_BUFFER_SIZE    (10U)

// Data structure for global variables that aren't config or state
typedef struct
{
	// Buffers for sending and receiving data
	uint8_t sendBuffer[_I2C_BUFFER_SIZE];
	uint8_t receiveBuffer[_I2C_BUFFER_SIZE];

	// Slave's channel and address information
	I2C_channel_E slaveChannel;
	uint8_t slaveAddress;

	// Length of the data to be sent or received (in bytes)
	uint8_t dataLength;

	// Pointer for the buffers (for convenience reason)
	uint8_t* bufferPtr;
} I2C_data_S;

// Adding an idle state because reading the BUSY bit in SR2 can cause problems
typedef enum{
	I2C_STATE_SEND,    // Write
	I2C_STATE_RECEIVE, // Read
	I2C_STATE_IDLE,    // Idle
}I2C_state_S;

// Global variables and their initialization
extern const I2C_config_S I2C_config;

static I2C_state_S I2C_state = I2C_STATE_IDLE;

static I2C_data_S I2C_data =
{
	{0x0},
	{0x0},
	0,
	0x0,
	0x0,
	0x0
};


void I2C_init(void)
{
	// Check that all configs are valid
	assert(IS_GPIO_PIN_SOURCE(I2C_config.HWConfig->SDAPin));
	assert(IS_GPIO_ALL_PERIPH(I2C_config.HWConfig->SDAPort));
	assert(IS_GPIO_PIN_SOURCE(I2C_config.HWConfig->SCLPin));
	assert(IS_GPIO_ALL_PERIPH(I2C_config.HWConfig->SCLPort));
	assert(IS_I2C_ALL_PERIPH(I2C_config.HWConfig->I2CPeriph));

	// GPIO Init
	RCCHelper_clockCmd(I2C_config.HWConfig->SDAPort, ENABLE);
	if (I2C_config.HWConfig->SDAPort != I2C_config.HWConfig->SCLPort)
	{
		RCCHelper_clockCmd(I2C_config.HWConfig->SCLPort, ENABLE);
	}

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin   = BITVALUE(I2C_config.HWConfig->SDAPin);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(I2C_config.HWConfig->SDAPort, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = BITVALUE(I2C_config.HWConfig->SCLPin);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(I2C_config.HWConfig->SCLPort, &GPIO_InitStruct);

	// Alternative function config for SDA and SCL
	GPIO_PinAFConfig(I2C_config.HWConfig->SDAPort, I2C_config.HWConfig->SDAPin, I2C_config.HWConfig->I2CAFNum);
	GPIO_PinAFConfig(I2C_config.HWConfig->SCLPort, I2C_config.HWConfig->SCLPin, I2C_config.HWConfig->I2CAFNum);

	// I2C peripheral init
	I2C_setup();

	// NVIC Init for event interrupt
	NVIC_InitTypeDef NVIC_Init_Struct;
	NVIC_Init_Struct.NVIC_IRQChannel                   = I2C_config.HWConfig->I2CEVIRQ;
	NVIC_Init_Struct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init_Struct.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_Init_Struct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_Init(&NVIC_Init_Struct);

	// NVIC Init for error interrupt
	NVIC_Init_Struct.NVIC_IRQChannel                   = I2C_config.HWConfig->I2CERIRQ;
	NVIC_Init_Struct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init_Struct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_Init_Struct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_Init(&NVIC_Init_Struct);

	// Set priority for ISR and enable ISR
	NVIC_EnableIRQ(I2C_config.HWConfig->I2CEVIRQ);
	NVIC_EnableIRQ(I2C_config.HWConfig->I2CERIRQ);
}

void I2C_setup(void)
{
	// I2C2 Init
	RCCHelper_clockCmd(I2C_config.HWConfig->I2CPeriph, ENABLE);

	/*
	 * Reset I2C after a delay: After enabling the RCC clock, the BUSY bit in the SR2 register
	 *                          became 1, which means the peripheral cannot start any data
	 *                          transaction. According to STM32 documents and resources on
	 *                          Google, this could be a silicon defect, and reseting the RCC
	 *                          clock after a small delay is the current work-around.
	 * (https://community.st.com/s/question/0D50X00009XkYxY/stm32f105rc-i2c2-always-busy)
	 *
	 * 15 is the minimum value of delay that ensures I2C's functioning for now.
	 * If the BUSY bit cannot be reset, increase the delay time.
	 * DO NOT check BUSY bit in SR2 in the code!!! It will then be set FOREVER!!!
	 */
	int i = 0;
	while (i < 15){
		i++;
	}
	RCCHelper_resetCmd(I2C_config.HWConfig->I2CPeriph, ENABLE);
	RCCHelper_resetCmd(I2C_config.HWConfig->I2CPeriph, DISABLE);

	I2C_InitTypeDef I2C_Init_Struct;
	I2C_StructInit(&I2C_Init_Struct);
	I2C_Init_Struct.I2C_ClockSpeed          = 50000; // Any requirement for I2C clock speed?
	I2C_Init_Struct.I2C_Ack                 = I2C_Ack_Enable;
	I2C_Init_Struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init_Struct.I2C_DutyCycle           = I2C_DutyCycle_2;
	I2C_Init_Struct.I2C_Mode                = I2C_Mode_I2C;
	I2C_Init_Struct.I2C_OwnAddress1         = 0x0;
	I2C_Init(I2C_config.HWConfig->I2CPeriph, &I2C_Init_Struct);

	// Enable event interrupts and buffer interrupts
	I2C_ITConfig(I2C_config.HWConfig->I2CPeriph, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C_config.HWConfig->I2CPeriph, I2C_IT_BUF, ENABLE);
	I2C_ITConfig(I2C_config.HWConfig->I2CPeriph, I2C_IT_ERR, ENABLE);

	// Enable I2C2
	// This doesn't work, PE cannot be set
	//I2C_Cmd(I2C_config.HWConfig->I2CPeriph, ENABLE);
	I2C_config.HWConfig->I2CPeriph->CR1 |= I2C_CR1_PE;
}

bool I2C_send(I2C_channel_E channel, const uint8_t * data, const uint8_t length)
{
	bool task_delivery = false;

	// Check the validity of message
	if((data != NULL) && (length != 0) && (length <= _I2C_BUFFER_SIZE))
	{
		// Check the availability of I2C
		if ((I2C_data.dataLength == 0) && (I2C_state == I2C_STATE_IDLE)) {

			memcpy(I2C_data.sendBuffer, data, length);
			I2C_state = I2C_STATE_SEND;
			I2C_data.slaveChannel = channel;
			I2C_data.slaveAddress = I2C_channelToAddressMapping(channel);
			I2C_data.dataLength = length;
			I2C_GenerateSTART(I2C_config.HWConfig->I2CPeriph, ENABLE);
			task_delivery = true;
		}
	}
	return task_delivery;
}

bool I2C_receive(I2C_channel_E channel, const uint8_t length)
{
	bool task_delivery = false;

	// Check the validity of message
	if((length != 0) && (length <= _I2C_BUFFER_SIZE))
	{
		// Check the availability of I2C
		if ((I2C_data.dataLength == 0) && (I2C_state == I2C_STATE_IDLE)) {

			memset(I2C_data.receiveBuffer, 0, sizeof(I2C_data.receiveBuffer)); // Empty the receive buffer
			I2C_state = I2C_STATE_RECEIVE;
			I2C_data.slaveChannel = channel;
			I2C_data.slaveAddress = I2C_channelToAddressMapping(channel);
			I2C_data.dataLength = length;
			I2C_GenerateSTART(I2C_config.HWConfig->I2CPeriph, ENABLE);
			task_delivery = true;
		}
	}
	return task_delivery;
}

bool is_idle()
{
	if (I2C_state != I2C_STATE_IDLE) return false;
	else return true;
}

// See stm32f413 programming manual P855-861
static inline void I2C_private_TxRxIRQ(I2C_TypeDef * I2Cx)
{
	// Check Start condition
	if ((I2Cx->SR1 & I2C_SR1_SB) == I2C_SR1_SB)
	{
		// LSB is the r/w bit
		I2C_Send7bitAddress(I2Cx, (I2C_data.slaveAddress << I2C_OAR1_ADD0), (uint8_t)I2C_state);
	}
	// Waits for address sent bit to be set
	else if ((I2Cx->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR)
	{
		// Clearing ADDR bit by reading SR1 and then reading SR2
		I2Cx->SR2;
		if (I2C_state == I2C_STATE_SEND)
		{
			I2C_data.bufferPtr = &I2C_data.sendBuffer[0];
			I2C_SendData(I2Cx, *I2C_data.bufferPtr);
			I2C_data.bufferPtr ++;
			I2C_data.dataLength --;
		}
		else if (I2C_state == I2C_STATE_RECEIVE)
		{
			I2C_data.bufferPtr = &I2C_data.receiveBuffer[0];
			if (I2C_data.dataLength == 1)
			{
				I2C2->CR1 &= ~(I2C_CR1_ACK);
				I2C_GenerateSTOP(I2Cx, ENABLE); // Generate a stop condition
			}
		}
	}
	// Sending data to slave
	else if ((I2Cx->SR1 & I2C_SR1_TXE) == I2C_SR1_TXE)
	{
		if (I2C_data.dataLength > 0)
		{
			I2C_SendData(I2Cx, *I2C_data.bufferPtr);
			I2C_data.bufferPtr ++;
			I2C_data.dataLength --;
		}
		else
		{
			// Generate a stop condition ONLY ONCE
			if (I2C_state != I2C_STATE_IDLE){
				I2C_GenerateSTOP(I2Cx, ENABLE);
			}
			I2C_state = I2C_STATE_IDLE;
		}
	}
	// Receiving data from slave
	else if ((I2Cx->SR1 & I2C_SR1_RXNE) == I2C_SR1_RXNE)
	{
		*I2C_data.bufferPtr = I2C_ReceiveData(I2Cx);
		I2C_data.dataLength --;
		if (I2C_data.dataLength == 1)
		{
			I2Cx->CR1 &= ~(I2C_CR1_ACK);
			I2C_GenerateSTOP(I2Cx, ENABLE); // Generate a stop condition
		}
		if (I2C_data.dataLength == 0)
		{
			I2C_state = I2C_STATE_IDLE;
		}
		if (I2C_config.messageReceivedCallback != NULL)
		{
			protocol_MID_E messageID =  (protocol_MID_E)0x0;
			uint32_t receivedData = (uint32_t)*I2C_data.bufferPtr;
			I2C_config.messageReceivedCallback(messageID, (protocol_allMessages_U *)receivedData);
		}
		// Get ready to receive next data
		I2C_data.bufferPtr ++;
	}
}

// See stm32f413 programming manual P860
static inline void I2C_private_ERIRQ(I2C_TypeDef * I2Cx)
{
	// Error handling: acknowledge failure
	if ((I2Cx->SR1 & I2C_SR1_AF) == I2C_SR1_AF)
	{
		// Stop current transaction
		I2C_data.dataLength = 0;
		// Generate a stop condition ONLY ONCE
		if (I2C_state != I2C_STATE_IDLE){
			I2C_GenerateSTOP(I2Cx, ENABLE);
		}
		// For debugging: toggle the red LED
		LED_toggleLED(LED_CHANNEL_RED);
		I2C_state = I2C_STATE_IDLE;
		// Clear the AF flag
		I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
		}
}

void I2C1_EV_IRQHandler(void){
	I2C_private_TxRxIRQ(I2C1);
}

void I2C2_EV_IRQHandler(void){
	I2C_private_TxRxIRQ(I2C2);
}

void I2C3_EV_IRQHandler(void){
	I2C_private_TxRxIRQ(I2C3);
}

void I2C1_ER_IRQHandler(void){
	I2C_private_ERIRQ(I2C1);
}

void I2C2_ER_IRQHandler(void){
	I2C_private_ERIRQ(I2C2);
}

void I2C3_ER_IRQHandler(void){
	I2C_private_ERIRQ(I2C3);
}
