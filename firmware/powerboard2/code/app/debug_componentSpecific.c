/*
 * debug_componentSpecific.c
 *
 *  Created on: Aug 6, 2020
 *      Author: Poornachander
 */

#include "debug.h"

static const debug_HWConfig_S debug_HWConfig =
{
	// GPIO
	.rxPin = 9,
	.txPin = 8,
	.GPIOPort = GPIOD,
	.AFNumber = GPIO_AF_USART3,

	// UART
	.UARTPeriph = USART3,
};

extern const debug_config_S debug_config;
const debug_config_S debug_config =
{
	.HWConfig = &debug_HWConfig
};

