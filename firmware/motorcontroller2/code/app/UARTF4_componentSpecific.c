/*
 * UART_componentSpecific.c
 *
 *  Created on: Feb 20, 2020
 *      Author: Poornachander
 */

#include "utils.h"
#include "stm32f4xx.h"
#include "UARTF4.h"
#include "RTOS.h"
#include "messageHandler.h"

static void UART_enablePeripheralsClockCallback(void);
static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength);

static const UART_HWConfig_S UART_HWConfig =
{
	// GPIO
	.rxPin = GPIO_PinSource7,
	.txPin = GPIO_PinSource6,
	.GPIOPort = GPIOB,
	.AFNumber = GPIO_AF_USART1,

	// UART
	.UARTPeriph = USART1,
	.UARTInterruptNumber = USART1_IRQn,
	.baudRate = 9600,

	// DMA
	.DMAController = DMA2,
	.DMAStreamRX = DMA2_Stream5,
	.DMAChannelRX = DMA_Channel_4,
	.DMAStreamTX = DMA2_Stream7,
	.DMAChannelTX = DMA_Channel_4,
	.DMARXInterruptNumber = DMA2_Stream5_IRQn,
	.DMATXInterruptNumber = DMA2_Stream7_IRQn,

	// Common
	.enablePeripheralsClockCallback = UART_enablePeripheralsClockCallback,
};

extern const UART_config_S UART_config;
const UART_config_S UART_config =
{
	.taskPriority = RTOS_TASK_1MS_PRIORITY,
	.receiveCallback = UART_receiveCallback,
	.HWConfig = &UART_HWConfig
};

static void UART_enablePeripheralsClockCallback(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
}

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength)
{
	UNUSED(receiveDataLength);
	if(receiveData != NULL)
	{
		messageHandler_messageReceivedCallback((const protocol_message_S * const)receiveData);
	}
}

/* DMA Interrupt Handler */
void DMA2_Stream7_IRQHandler(void)
{
	UART_DMAInterruptHandler();
}

void DMA2_Stream5_IRQHandler(void)
{
	UART_DMAInterruptHandler();
}

