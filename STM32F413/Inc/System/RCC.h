/*
 * RCC.h
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 */

#ifndef SYSTEM_RCC_H_
#define SYSTEM_RCC_H_

#include <stm32f413xx.h>
#include <stdint.h>

extern void RCC_EnableGpioClk(GPIO_TypeDef * GPIOx);
extern void RCC_EnableI2cClk(I2C_TypeDef * I2Cx);



/* Internal Use Only */
void RCC_AHB1ENR(uint32_t Peripheral);
void RCC_AHB1RSTR(uint32_t Peripheral);
void RCC_APB1ENR(uint32_t Peripheral);
void RCC_APB2ENR(uint32_t Peripheral);



/* Do not use these */
void RCC_EnableAllGpioClk(void);
void RCC_ResetAllGpioClk(void);
#endif /* SYSTEM_RCC_H_ */
