/*
 * stm32fx.h
 */

#ifndef STM32FX_H_
#define STM32FX_H_

#if DEVICE_FAMILY_STM32F4 == 1
#include "stm32f4xx.h"
#elif DEVICE_FAMILY_STM32F0 == 1
#include "stm32f0xx.h"
#endif

#endif /* STM32FX_H_ */
