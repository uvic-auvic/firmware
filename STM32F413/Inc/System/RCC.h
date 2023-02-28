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






#define CHEAT_CODE


void RCC_AHB1ENR(uint32_t Peripheral);
void RCC_AHB1RSTR(uint32_t Peripheral);
void RCC_APB1ENR(uint32_t Peripheral);
void RCC_APB2ENR(uint32_t Peripheral);

#ifdef CHEAT_CODE
/* Cheat Codes - Dirty Fixes to Problems*/
/* We ideally dont want to enable all the clocks because it draws more current. The solution to this is create a lookup table or dict that maps the GPIO address to the RCC address for the pin. */
void RCC_EnableAllGpioClk(void);
void RCC_ResetAllGpioClk(void);
#endif

#endif /* SYSTEM_RCC_H_ */
