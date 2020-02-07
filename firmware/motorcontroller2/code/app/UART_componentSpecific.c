/*
 * UART_componentSpecific.c
 *
 *  Created on: Jul 23, 2019
 *      Author: Poornachander
 */

#include <stdlib.h>
#include "stm32f4xx.h"
#include "UART.h"

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength);
static void UART_enablePeripheralsClockCallback(void);

static const UART_HWConfig_S UART_HWConfig = 
{
	.rxPin = GPIO_PinSource7,
	.txPin = GPIO_PinSource6,
	.GPIOPort = GPIOB,

	.UARTPeriph = USART1,
	.UARTInterruptNumber = USART1_IRQn,
	.baudRate = 9600,

	.enablePeripheralsClockCallback = UART_enablePeripheralsClockCallback
};

extern UART_config_S UART_config;
UART_config_S UART_config =
{
	.receiveCallback = UART_receiveCallback,
	.HWConfig = &UART_HWConfig
};

static void UART_enablePeripheralsClockCallback(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength)
{
	if(receiveData != NULL)
	{
		UART_writeLen(receiveData, receiveDataLength);
	}
}
