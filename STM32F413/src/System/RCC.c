/*
 * RCC.c
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 */

#include "System/RCC.h"

extern void RCC_EnableGpioClk(GPIO_TypeDef * GPIOx){
	if(GPIOx == GPIOA){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOAEN);
	} else if(GPIOx == GPIOB){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOBEN);
	} else if(GPIOx == GPIOC){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOCEN);
	} else if(GPIOx == GPIOD){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIODEN);
	} else if(GPIOx == GPIOE){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOEEN);
	} else if(GPIOx == GPIOF){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOFEN);
	} else if(GPIOx == GPIOG){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOGEN);
	} else if(GPIOx == GPIOH){
		 RCC_AHB1ENR(RCC_AHB1ENR_GPIOHEN);
	}
}

extern void RCC_EnableI2cClk(I2C_TypeDef * I2Cx){
	if(I2Cx == I2C1){
		RCC_APB1ENR(RCC_APB1ENR_I2C1EN);
	} else if(I2Cx == I2C2){
		RCC_APB1ENR(RCC_APB1ENR_I2C2EN);
	} else if(I2Cx == I2C3){
		RCC_APB1ENR(RCC_APB1ENR_I2C3EN);
	}
}

void RCC_AHB1ENR(uint32_t Peripheral){
	RCC->AHB1ENR |= Peripheral;
	// Poll until the clock is enabled to avoid race condition.
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
