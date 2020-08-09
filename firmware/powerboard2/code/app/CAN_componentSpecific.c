/*
 * CAN_componentSpecific.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Poornachander
 */

#include "CAN.h"

static void CAN_enablePeripheralsClockCallback(void);

static const CAN_HWConfig_S CAN_HWConfig =
{
	// GPIO
	.rxPin = GPIO_PinSource12,
	.txPin = GPIO_PinSource11,
	.GPIOPort = GPIOA,
	.AFNumber = GPIO_AF_USART6,

	// Common
	.enablePeripheralsClockCallback = CAN_enablePeripheralsClockCallback,
};

extern const CAN_config_S CAN_config;
const CAN_config_S CAN_config =
{
	.HWConfig = &CAN_HWConfig
};

static void CAN_enablePeripheralsClockCallback(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}