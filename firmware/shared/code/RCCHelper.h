/*
 * RCCHelper.h
 *
 *  Created on: Aug 14, 2020
 *      Author: Poornachander
 */

#ifndef SHARED_CODE_RCCHELPER_H_
#define SHARED_CODE_RCCHELPER_H_

#include "stm32f4xx.h"

void RCCHelper_clockCmd(const void * const periph, const FunctionalState newState);

#endif /* SHARED_CODE_RCCHELPER_H_ */
