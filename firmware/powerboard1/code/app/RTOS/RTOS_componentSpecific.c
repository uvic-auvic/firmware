/*
 * RTOS_componentSpecific.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

// #include "stm32f0xx.h"

/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
	// RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	// GPIOC->MODER |= GPIO_MODER_MODER8_0;
	// GPIOC->ODR |= GPIO_Pin_8;
}

void RTOS_run1ms(void)
{

}

void RTOS_run10ms(void)
{

}

void RTOS_run100ms(void)
{

}

void RTOS_run1000ms(void)
{

}

