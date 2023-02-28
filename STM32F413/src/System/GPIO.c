/*
 * GPIO.c
 *
 *  Created on: Jan. 28, 2023
 *      Author: amann
 */

#include "System/GPIO.h"


GPIO_Channel GPIO_Init(GPIO_Pin Pin, GPIO_TypeDef* GPIOx, uint32_t MODER, uint32_t PUPDR,
		uint32_t OSPEEDR, uint32_t AF, uint16_t OTYPER){
	// Enable the clock for the GPIOx.
	// TODO: Use a Dict and Hash to map GPIOx to the define address for the GPIO Clock. We dont want to enable all of them because that draws more current.
	RCC_EnableAllGpioClk();

	// Modify the register
	GPIOx->MODER |= MODER;
	GPIOx->OTYPER |= OTYPER;
	GPIOx->PUPDR |= PUPDR;
	GPIOx->OSPEEDR |= OSPEEDR;
	GPIOx->AFR[(Pin < P8) ? 0 : 1] |= AF;

	GPIO_Channel ret;
	ret.GPIOx = GPIOx;
	ret.PIN = Pin;
	return ret;
}

void GPIO_DeInit(GPIO_Channel* Channel){
	// Disable the clocks
	RCC_ResetAllGpioClk();
	// Toggle Off the GPIOx registers
}

GPIO_State GPIO_ReadPin(GPIO_Channel* Channel){
	return (GPIO_State)(Channel->GPIOx->IDR & (1U << Channel->PIN));
}

void GPIO_WritePin(GPIO_Channel* Channel, GPIO_State State){
	// Best way. If state is reset, it will shift to [16,31]; if set, it wont shift it.
	Channel->GPIOx->BSRR |= ((1 << Channel->PIN) << (BSRR_RESET_OFFSET * (State))); // Move to upper registers

	// Inefficient method because of the branching instruction.
//	if(Channel->STATE == GPIO_SET){
//		Channel->GPIOx->BSRR = (1U << Channel->PIN);
//	} else {
//		Channel->GPIOx->BSRR = ((1U << Channel->PIN) << 16); // Move to upper registers
//	}
//	}
}

void GPIO_TogglePin(GPIO_Channel* Channel){
	// Use the XOR bitwise operator to toggle the state of the pin. Use ODR, not BSRR
	Channel->GPIOx->ODR ^= (1 << Channel->PIN); // 0 XOR 0 = 0. The other bits will remain 0 or 1.
}
