/*
 * CAN.h
 *
 *  Created on: Aug 8, 2020
 *      Author: Poornachander
 */

#ifndef SHARED_CODE_CAN_H_
#define SHARED_CODE_CAN_H_

#include "stm32f4xx.h"

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
} CAN_config_S;

void CAN_init(void);
void CAN_run1ms(void);
void CAN_SendMessage(const uint16_t messageID, const uint8_t * const data, const uint8_t dataLength);


#endif /* SHARED_CODE_CAN_H_ */
