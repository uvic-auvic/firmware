/*
 * CAN_componentSpecific.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Poornachander
 */

#include "CAN.h"
#include "messageHandler.h"

static void CAN_enablePeripheralsClockCallback(void);
static void CAN_messageReceivedCallback(const protocol_MID_E messageID, const protocol_allMessages_U * const message);

static const CAN_HWConfig_S CAN_HWConfig =
{
	// GPIO
	.rxPin = 8,
	.txPin = 9,
	.GPIOPort = GPIOB,
	.AFNumber = GPIO_AF8_CAN1,

	// CAN
	.CANPeriph = CAN1,

	// Common
	.enablePeripheralsClockCallback = CAN_enablePeripheralsClockCallback,
};

extern const CAN_config_S CAN_config;
const CAN_config_S CAN_config =
{
	.HWConfig = &CAN_HWConfig,
	.messageReceivedCallback = CAN_messageReceivedCallback,
};

static void CAN_enablePeripheralsClockCallback(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOBEN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_CAN1EN, ENABLE);
}

static void CAN_messageReceivedCallback(const protocol_MID_E messageID, const protocol_allMessages_U * const message)
{
	messageHandler_messageReceivedCallback(messageID, message);
}