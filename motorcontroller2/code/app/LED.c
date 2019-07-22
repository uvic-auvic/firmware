/*
 * LED.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "stm32f4xx.h"

void LED_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER12_0;

}

void LED_run15(void)
{
	GPIOD->ODR ^= GPIO_Pin_15;
}

void LED_run14(void)
{
	GPIOD->ODR ^= GPIO_Pin_14;
}

void LED_run13(void)
{
	GPIOD->ODR ^= GPIO_Pin_13;
}

void LED_run12(void)
{
	GPIOD->ODR ^= GPIO_Pin_12;
}
