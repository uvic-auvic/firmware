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

IRQn_Type interruptHelper_getIRQn_USART(const USART_TypeDef * const UARTPeriph);
void interruptHelper_registerCallback_USART(const USART_TypeDef * const UARTPeriph, const IRQCallback callback);

#endif /* SHARED_CODE_INTERRUPTHELPER_H_ */
