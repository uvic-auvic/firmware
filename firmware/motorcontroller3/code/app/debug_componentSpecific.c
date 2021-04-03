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
	.rxPin = 12,
	.txPin = 11,
	.GPIOPort = GPIOA,
	.AFNumber = GPIO_AF_USART6,

	// UART
	.UARTPeriph = USART6,

	// DMA
	.DMAPeriph = DMA2,
	.TXDMAStream = DMA2_Stream7,
	.TXDMAChannel = DMA_Channel_5,
};

extern const debug_config_S debug_config;
const debug_config_S debug_config =
{
	.HWConfig = &debug_HWConfig
};

