/*
 * debug_componentSpecific.c
 *
 *  Created on: Aug 6, 2020
 *      Author: Poornachander
 */

#include "debug.h"

static void debug_enablePeripheralsClockCallback(void);

static const debug_HWConfig_S debug_HWConfig =
{
	// GPIO
	.rxPin = 12,
	.txPin = 11,
	.GPIOPort = GPIOA,
	.AFNumber = GPIO_AF_USART6,

	// UART
	.UARTPeriph = USART6,
	.UARTInterruptNumber = USART6_IRQn,

	// Common
	.enablePeripheralsClockCallback = debug_enablePeripheralsClockCallback,
};

extern const debug_config_S debug_config;
const debug_config_S debug_config =
{
	.HWConfig = &debug_HWConfig
};

static void debug_enablePeripheralsClockCallback(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
}

/* UART Interrupt Handler */
void USART6_IRQHandler(void)
{
	debug_UARTInterruptHandler();
}

