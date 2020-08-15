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

IRQn_Type interruptHelper_getIRQn_USART(USART_TypeDef * UARTPeriph);
void interruptHelper_registerCallback_USART(USART_TypeDef * UARTPeriph, IRQCallback callback);

#endif /* SHARED_CODE_INTERRUPTHELPER_H_ */
