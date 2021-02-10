/*
 * I2C.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Shuhang Tan
 *      Reference: https://github.com/uvic-auvic/Firmware-Motor_Controller_2.0/blob/master/src/I2C.c
 *                 (I2C code for motor controller 2)
 */

#include "I2C.h"
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "LED.h"

// I2C SDA and SCL pins
#define _I2C_SDA_GPIO  (GPIO_Pin_9) // PC9
#define _I2C_SCL_GPIO  (GPIO_Pin_8) // PA8

// I2C sendBuffer size
#define _I2C_BUFFER_SIZE    (10U)

typedef struct
{
	uint8_t Buffer[_I2C_BUFFER_SIZE];
} I2C_data_S;

// Adding an idle state because reading the BUSY bit in SR2 can cause problems
typedef enum{
	I2C_STATE_SEND,    // Write
	I2C_STATE_RECEIVE, // Read
	I2C_STATE_IDLE,    // Idle
}I2C_state_S;

// Global variables and their initialization
static I2C_data_S I2C_send_data;
static I2C_data_S I2C_receive_data;
static I2C_state_S I2C_state = I2C_STATE_IDLE;

uint8_t slave_address = 0x0;

// Length of the data to be sent or received (in bytes)
volatile uint8_t data_length = 0;

// Pointer to the buffers
uint8_t* sendBufferPtr;
uint8_t* receiveBufferPtr;

void I2C_init(void)
{
	// GPIO Init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	// Alternative function config for SDA and SCL
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2);

	// I2C peripheral init
	I2C_setup();

	// NVIC Init
	NVIC_InitTypeDef NVIC_Init_Struct;
	NVIC_Init_Struct.NVIC_IRQChannel                   = I2C2_EV_IRQn;
	NVIC_Init_Struct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init_Struct.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_Init_Struct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_Init(&NVIC_Init_Struct);

	// Set priority for ISR and enable ISR
	//NVIC_SetPriority(I2C2_EV_IRQn, 0);
	NVIC_EnableIRQ(I2C2_EV_IRQn);
}

void I2C_setup(void)
{
	// I2C2 Init
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	// Reset I2C after a delay
	// If you want to know why, it's because I2C won't work unless doing so.
	int i = 0;
	while (i < 20){
		i++;
	}
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

	I2C_InitTypeDef I2C_Init_Struct;
	I2C_Init_Struct.I2C_ClockSpeed          = 50000; // Any requirement for I2C clock speed?
	I2C_Init_Struct.I2C_Ack                 = I2C_Ack_Enable;
	I2C_Init_Struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init_Struct.I2C_DutyCycle           = I2C_DutyCycle_2;
	I2C_Init_Struct.I2C_Mode                = I2C_Mode_I2C;
	I2C_Init_Struct.I2C_OwnAddress1         = 0x0;
	I2C_Init(I2C2, &I2C_Init_Struct);

	// Enable event interrupts and buffer interrupts
	I2C_ITConfig(I2C2, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C2, I2C_IT_BUF, ENABLE);

	// Enable I2C2
	I2C_Cmd(I2C2, ENABLE);
}

bool I2C_send(I2C_channel_E channel, const uint8_t * data, const uint8_t length)
{
	// Check the validity of message
	if((data != NULL) && (length != 0) && (length <= _I2C_BUFFER_SIZE))
	{
		// Check the availability of I2C
		if ((data_length != 0) || (I2C_state != I2C_STATE_IDLE)) return false;

		memcpy(I2C_send_data.Buffer, data, length);
		I2C_state = I2C_STATE_SEND;
		slave_address = I2C_channelToAddressMapping(channel);
		data_length = length;
		I2C_GenerateSTART(I2C2, ENABLE);
		return true;
	}
	else return false;
}

bool I2C_receive(I2C_channel_E channel, const uint8_t * data, const uint8_t length)
{
	// Check the validity of message
	if((data != NULL) && (length != 0) && (length <= _I2C_BUFFER_SIZE))
	{
		// Check the availability of I2C
		if ((data_length != 0) || (I2C_state != I2C_STATE_IDLE)) return false;

		memcpy(I2C_receive_data.Buffer, data, length);
		I2C_state = I2C_STATE_RECEIVE;
		slave_address = I2C_channelToAddressMapping(channel);
		data_length = length;
		I2C_GenerateSTART(I2C2, ENABLE);
		return true;
	}
	else return false;
}

bool is_idle()
{
	if (I2C_state != I2C_STATE_IDLE) return false;
	else return true;
}

// See stm32f413 programming manual P855-861
void I2C2_EV_IRQHandler(void)
{
	// Check Start condition
	if ((I2C2->SR1 & I2C_SR1_SB) == I2C_SR1_SB)
	{
		// LSB is the r/w bit
		I2C_Send7bitAddress(I2C2, (slave_address << I2C_OAR1_ADD0), (uint8_t)I2C_state);
		//I2C2->DR = (slave_address << I2C_OAR1_ADD0) + (uint8_t)I2C_state;
	}
	// Waits for address sent bit to be set
	else if ((I2C2->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR)
	{
		// Clearing ADDR bit by reading SR1 and then reading SR2
		I2C2->SR2;
		if (I2C_state == I2C_STATE_SEND)
		{
			sendBufferPtr = &I2C_send_data.Buffer[0];
			I2C_SendData(I2C2, *sendBufferPtr);
			sendBufferPtr ++;
			data_length --;
		}
		else if (I2C_state == I2C_STATE_RECEIVE)
		{
			receiveBufferPtr = &I2C_receive_data.Buffer[0];
			if (data_length == 1)
			{
				I2C2->CR1 &= ~(I2C_CR1_ACK);
				I2C_GenerateSTOP(I2C2, ENABLE); // Generate a stop condition
			}
		}
	}
	// Sending data to slave
	else if ((I2C2->SR1 & I2C_SR1_TXE) == I2C_SR1_TXE)
	{
		if (data_length > 0)
		{
			I2C_SendData(I2C2, *sendBufferPtr);
			sendBufferPtr ++;
			data_length --;
		}
		else
		{
			// Generate a stop condition ONLY ONCE
			if (I2C_state != I2C_STATE_IDLE){
				I2C_GenerateSTOP(I2C2, ENABLE);
			}
			I2C_state = I2C_STATE_IDLE;
		}
	}
	// Receiving data from slave
	else if ((I2C2->SR1 & I2C_SR1_RXNE) == I2C_SR1_RXNE)
	{
		*receiveBufferPtr = I2C_ReceiveData(I2C2);
		receiveBufferPtr ++;
		data_length --;
		if (data_length == 1)
		{
			I2C2->CR1 &= ~(I2C_CR1_ACK);
			I2C_GenerateSTOP(I2C2, ENABLE); // Generate a stop condition
		}
		if (data_length == 0)
		{
			I2C_state = I2C_STATE_IDLE;
		}
	}
	// Error checking: acknowledge failure
	else if ((I2C2->SR1 & I2C_SR1_AF) == I2C_SR1_AF)
	{
		// Stop everything, reinit I2C
		data_length = 0;
		//LED_toggleLED(LED_CHANNEL_RED);
		I2C_GenerateSTOP(I2C2, ENABLE);
		I2C_ITConfig(I2C2, I2C_IT_EVT, DISABLE);
		I2C_ITConfig(I2C2, I2C_IT_BUF, DISABLE);
		I2C_Cmd(I2C2, DISABLE);
		I2C_DeInit(I2C2);
		I2C_setup();
	}
}
