/*
 * GPIO.c
 *
 *  Created on: Jan. 28, 2023
 *      Author: amann
 */

#include "System/GPIO.h"


GPIO_Channel GPIO_Init(GPIO_Pin Pin, GPIO_TypeDef* GPIOx, uint32_t MODER, uint32_t PUPDR,
		uint32_t OSPEEDR, uint32_t AF, uint16_t OTYPER){
	// Enable the GPIO Clock
	RCC_EnableGpioClk(GPIOx);


	// Modify the GPIO
	GPIOx->MODER |= MODER;
	GPIOx->OTYPER |= OTYPER;
	GPIOx->PUPDR |= PUPDR;
	GPIOx->OSPEEDR |= OSPEEDR;
	GPIOx->AFR[(Pin < P8) ? 0 : 1] |= AF;

	// Return an object to save
	GPIO_Channel ret;
	ret.GPIOx = GPIOx;
	ret.PIN = Pin;
	return ret;
}

GPIO_State GPIO_ReadPin(GPIO_Channel* Channel){
	return (GPIO_State)(Channel->GPIOx->IDR & (1U << Channel->PIN));
}

void GPIO_WritePin(GPIO_Channel* Channel, GPIO_State State){
	//If state is reset, it will shift to [16,31]; if set, it wont shift it.
	Channel->GPIOx->BSRR |= ((1 << Channel->PIN) << (BSRR_RESET_OFFSET * (State))); // Move to upper registers
}

void GPIO_TogglePin(GPIO_Channel* Channel){
	// Use the XOR bitwise operator to toggle the state of the pin. Use ODR, not BSRR
	Channel->GPIOx->ODR ^= (1 << Channel->PIN); // 0 XOR 0 = 0. The other bits will remain 0 or 1.
}
