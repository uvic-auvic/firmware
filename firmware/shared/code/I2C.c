/*
 * I2C.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Shuhang Tan
 *      Reference: https://github.com/uvic-auvic/Firmware-Motor_Controller_2.0/blob/master/src/I2C.c
 *                 (I2C code for motor controller 2)
 */

#include "I2C.h"
#include <stdlib.h>
#include "stm32f4xx.h"

// I2C SDA and SCL pins
#define _I2C_SDA_GPIO  (GPIO_Pin_9) // PC9
#define _I2C_SCL_GPIO  (GPIO_Pin_8) // PA8

// I2C sendBuffer size
#define _I2C_BUFFER_SIZE 8

typedef enum{
	I2C_STATE_SEND,    // Write
	I2C_STATE_RECEIVE, // Read
}I2C_state;

// Global variables and their initialization
I2C_state I2C_state_S = I2C_STATE_RECEIVE;

uint8_t slave_address = 0x1;

// Length of the data to be sent or received (in bytes)
volatile uint8_t data_length = 0;

// Data counter, 0 when a process finished or not started
volatile uint8_t data_count = 0;

uint8_t* sendBuffer;
uint8_t* receiveBuffer;

void I2C_init(void)
{
	// GPIO Init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = _I2C_SDA_GPIO;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = _I2C_SCL_GPIO;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Alternative function config for SDA and SCL
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);


	// I2C3 Init
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	I2C_InitTypeDef I2C_Init_Struct;
	I2C_Init_Struct.I2C_ClockSpeed          = 50000; // Any requirement for I2C clock speed?
	I2C_Init_Struct.I2C_Ack                 = I2C_Ack_Enable;
	I2C_Init_Struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init_Struct.I2C_DutyCycle           = I2C_DutyCycle_2;
	I2C_Init_Struct.I2C_Mode                = I2C_Mode_I2C;
	I2C_Init_Struct.I2C_OwnAddress1         = 0x0;
	I2C_Init(I2C1, &I2C_Init_Struct);

	// Enable I2C3
	I2C_Cmd(I2C3, ENABLE);

	// Enable event interrupts & buffer interrupts
	I2C_ITConfig(I2C3, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C3, I2C_IT_BUF, ENABLE);

	// NVIC Init
	NVIC_InitTypeDef NVIC_Init_Struct;
	NVIC_Init_Struct.NVIC_IRQChannel                   = I2C3_EV_IRQn;
	NVIC_Init_Struct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init_Struct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_Init(&NVIC_Init_Struct);

	// Set priority for ISR and enable ISR
	NVIC_SetPriority(I2C3_EV_IRQn, 0);
	NVIC_EnableIRQ(I2C3_EV_IRQn);
}

bool I2C_send(I2C_channel_E channel, const uint8_t * data, const uint8_t length)
{
	// Check if I2C is being used currently
	if ((data_length != 0) || ((I2C3->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY))
		return false;
	sendBuffer = data;
	if (sendBuffer != data)
		return false;
	I2C_state_S = I2C_STATE_SEND;
	slave_address = I2C_channelToAddressMapping[channel];
	data_length = length;
	I2C_GenerateSTART(I2C3, ENABLE);
	return true;
}

bool I2C_receive(I2C_channel_E channel, const uint8_t * data, const uint8_t length)
{
	// Check if I2C is being used currently
	if ((data_length != 0) || ((I2C3->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY))
		return false;
	receiveBuffer = data;
	if (receiveBuffer != data)
		return false;
	I2C_state_S = I2C_STATE_RECEIVE;
	slave_address = I2C_channelToAddressMapping[channel];
	data_length = length;
	I2C_GenerateSTART(I2C3, ENABLE);
	return true;
}

// See stm32f413 programming manual P855-861
void I2C3_EV_IRQHandler(void)
{
	// Check Start condition
	if ((I2C3->SR1 & I2C_SR1_SB) == I2C_SR1_SB)
	{
		I2C_Send7bitAddress(I2C3, slave_address, (uint8_t)I2C_state_S);
	}
	// Waits for address sent bit to be set
	else if ((I2C3->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR)
	{
		// Clearing ADDR bit by reading SR1 (line 101) and then reading SR2
		I2C3->SR2;
		if (I2C_state_S == I2C_STATE_SEND)
		{
			I2C_SendData(I2C3, &sendBuffer);
			sendBuffer ++;
			data_length --;
		}
		else if (I2C_state_S == I2C_STATE_RECEIVE)
		{
			if (data_length == 1)
			{
				I2C3->CR1 &= ~(I2C_CR1_ACK);
				I2C_GenerateSTOP(I2C3, ENABLE); // Generate a stop condition
			}
		}
	}
	// Sending data to slave
	else if ((I2C3->SR1 & I2C_SR1_TXE) == I2C_SR1_TXE)
	{
		if (data_length > 0)
		{
			I2C_SendData(I2C3, &sendBuffer);
			sendBuffer ++;
			data_length --;
		}
		else
		{
			data_count = 0; // Clear counter
			I2C_GenerateSTOP(I2C3, ENABLE); // Generate a stop condition
		}
	}
	// Receiving data from slave
	else if ((I2C3->SR1 & I2C_SR1_RXNE) == I2C_SR1_RXNE)
	{

		*receiveBuffer = I2C_ReceiveData(I2C3);
		receiveBuffer ++;
		data_length --;
		if (data_length == 1)
		{
			I2C3->CR1 &= ~(I2C_CR1_ACK);
			I2C_GenerateSTOP(I2C3, ENABLE); // Generate a stop condition
		}
		if (data_length == 0)
		{
			data_count = 0;
		}
	}
	// Error checking: acknowledge failure
	else if ((I2C3->SR1 & I2C_SR1_AF) == I2C_SR1_AF)
	{
		// Stop I2C, do this transfer again later
		data_count = 0;
		data_length = 0;
		I2C_GenerateSTOP(I2C3, ENABLE); // Generate a stop condition
	}
}
