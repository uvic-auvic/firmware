/*
 * UART_componentSpecific.c
 *
 *  Created on: Jul 23, 2019
 *      Author: Poornachander
 */

#include "stm32f0xx.h"
#include "UARTv2.h"
#include "RTOS.h"
#include "messageHandler.h"

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength);
static void UART_enablePeripheralsClockCallback(void);

static const UART_HWConfig_S UART_HWConfig = 
{
	// GPIO
	.rxPin = GPIO_PinSource10,
	.txPin = GPIO_PinSource9,
	.GPIOPort = GPIOA,
	.AFNumber = GPIO_AF_1,

	// UART
	.UARTPeriph = USART1,
	.UARTInterruptNumber = USART1_IRQn,
	.baudRate = 115200,

	// DMA
	.DMAController = DMA1,
	.DMAChannelRX = DMA1_Channel3,
	.DMAChannelTX = DMA1_Channel2,
	.DMARXInterruptNumber = DMA1_Channel2_3_IRQn,
	.DMATXInterruptNumber = DMA1_Channel2_3_IRQn,
	// Common
	.enablePeripheralsClockCallback = UART_enablePeripheralsClockCallback,
};

extern UART_config_S UART_config;
UART_config_S UART_config =
{
	.taskPriority = RTOS_TASK_1MS_PRIORITY,
	.receiveCallback = UART_receiveCallback,
	.HWConfig = &UART_HWConfig
};

static void UART_enablePeripheralsClockCallback(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength)
{
	if(receiveData != NULL)
	{
		messageHandler_messageReceivedCallback((const protocol_message_S * const)receiveData);
	}
}
