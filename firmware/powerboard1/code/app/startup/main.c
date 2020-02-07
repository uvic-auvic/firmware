/*
 * hello_world.c
 *
 *  Created on: Jul 18, 2019
 *      Author: Poornachander
 */

#include "stm32f0xx.h"

int main()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->ODR |= GPIO_Pin_8;
	for(;;);
	return 0;
}
