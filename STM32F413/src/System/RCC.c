/*
 * RCC.c
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 */

#include "System/RCC.h"

void RCC_AHB1ENR(uint32_t Peripheral){
	RCC->AHB1ENR |= Peripheral;
	// introduce latency for clock. do this by polling the bit. This is required because of the race condition.
	while(!(RCC->AHB1ENR & Peripheral)){
		// Do Nothing
	}
}

void RCC_APB1ENR(uint32_t Peripheral){
	RCC->APB1ENR |= Peripheral;
	while(!(RCC->APB1ENR & Peripheral)){
		// Do Nothing
	}
}
void RCC_APB2ENR(uint32_t Peripheral){
	RCC->APB2ENR |= Peripheral;
	while(!(RCC->APB2ENR & Peripheral)){
		// Do Nothing
	}
}

/* Cheat Codes. Try not to use these */
void RCC_EnableAllGpioClk(void){
	uint32_t GPIOclk = (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN);
	
	RCC->AHB1ENR |= GPIOclk;
	// introduce latency for clock. do this by polling the bit. This is required because of the race condition.
	while(!(RCC->AHB1ENR & GPIOclk)){
		// Do Nothing
	}
}

void RCC_ResetAllGpioClk(void){
	uint32_t GPIOclk = (RCC_AHB1RSTR_GPIOARST | RCC_AHB1RSTR_GPIOBRST | RCC_AHB1RSTR_GPIOCRST | RCC_AHB1RSTR_GPIODRST | RCC_AHB1RSTR_GPIOERST | RCC_AHB1RSTR_GPIOFRST | RCC_AHB1RSTR_GPIOGRST | RCC_AHB1RSTR_GPIOHRST);
	
	RCC->AHB1RSTR |= GPIOclk;
	// introduce latency for clock. do this by polling the bit. This is required because of the race condition.
	while(!(RCC->AHB1RSTR & GPIOclk)){
		// Do Nothing
	}
}
