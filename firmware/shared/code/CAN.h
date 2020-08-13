/*
 * CAN.h
 *
 *  Created on: Aug 8, 2020
 *      Author: Poornachander
 */

#ifndef SHARED_CODE_CAN_H_
#define SHARED_CODE_CAN_H_

#include "stm32f4xx.h"
#include "protocol.h"

typedef struct
{
	// GPIO
	uint8_t 		rxPin;
	uint8_t 		txPin;
	GPIO_TypeDef *	GPIOPort;
	uint8_t			AFNumber;

    // CAN
    CAN_TypeDef *   CANPeriph;

    //Common
	void 		(* enablePeripheralsClockCallback)(void);
} CAN_HWConfig_S;

typedef struct
{
    const CAN_HWConfig_S * const HWConfig;
	void	(* messageReceivedCallback)(const protocol_MID_E messageID, const protocol_allMessages_U * const message);
} CAN_config_S;

void CAN_init(void);
bool CAN_sendMessage(const protocol_MID_E messageID, const protocol_allMessages_U * const message, const uint8_t dataLength);
void CAN_filterAdd(const protocol_MID_E messageID, const uint16_t filterNumber);

#endif /* SHARED_CODE_CAN_H_ */
