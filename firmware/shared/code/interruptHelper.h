/*
 * interruptHelper.h
 *
 *  Created on: Aug 14, 2020
 *      Author: Poornachander
 */

#ifndef SHARED_CODE_INTERRUPTHELPER_H_
#define SHARED_CODE_INTERRUPTHELPER_H_

#include "stm32f4xx.h"

typedef void (* IRQCallback)(void);

// USART
IRQn_Type interruptHelper_getIRQn_USART(const USART_TypeDef * const UARTPeriph);
void interruptHelper_registerCallback_USART(const USART_TypeDef * const UARTPeriph, const IRQCallback callback);

// CAN
IRQn_Type interruptHelper_getIRQn_CAN(const CAN_TypeDef * const CANPeriph, const uint32_t CANIT);

#endif /* SHARED_CODE_INTERRUPTHELPER_H_ */
