/*
 * ADC.h
 *
 *  Created on: Feb 23, 2023
 *      Author: amann
 */

#ifndef SYSTEM_ADC_H_
#define SYSTEM_ADC_H_

#include <stdint.h>
#include "System/GPIO.h"

void ADC_Init(GPIO_Pin PIN, GPIO_TypeDef* GPIOx);

void ADC_SetSampleTime();


uint32_t ADC_GetValue();



// Access to ADC Registers. Keep this static; dont call them outside ADC.c


#endif /* SYSTEM_ADC_H_ */
