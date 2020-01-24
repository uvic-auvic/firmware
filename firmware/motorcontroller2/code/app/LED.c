/*
 * LED.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "stm32f4xx.h"

void LED_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER14_0;
}

void LED_run15(void)
{
	GPIOB->ODR ^= GPIO_Pin_0;
}

void LED_run14(void)
{
	GPIOB->ODR ^= GPIO_Pin_7;
}

void LED_run13(void)
{
	GPIOB->ODR ^= GPIO_Pin_14;
}

void LED_run12(void)
{
	// GPIOB->ODR ^= GPIO_Pin_12;
}
