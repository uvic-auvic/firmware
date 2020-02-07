/*
 * LED.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "stm32f4xx.h"

void LED_init(void)
{
#if DEVICE_STM32F413
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER14_0;
#elif DEVICE_STM32F411
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER12_0;
#elif DEVICE_PCBA_REV_A
	// TBD
#endif
}

void LED_run15(void)
{
#if DEVICE_STM32F413
	GPIOB->ODR ^= GPIO_Pin_0;
#elif DEVICE_STM32F411
	GPIOD->ODR ^= GPIO_Pin_15;
#elif DEVICE_PCBA_REV_A
	// TBD
#endif
}

void LED_run14(void)
{
#if DEVICE_STM32F413
	GPIOB->ODR ^= GPIO_Pin_14;
#elif DEVICE_STM32F411
	GPIOD->ODR ^= GPIO_Pin_14;
#elif DEVICE_PCBA_REV_A
	// TBD
#endif
}

void LED_run13(void)
{
#if DEVICE_STM32F413

#elif DEVICE_STM32F411
	GPIOD->ODR ^= GPIO_Pin_13;
#elif DEVICE_PCBA_REV_A
	// TBD
#endif
}

void LED_run12(void)
{
#if DEVICE_STM32F413
	// Only has 3 LEDs
#elif DEVICE_STM32F411
	GPIOD->ODR ^= GPIO_Pin_12;
#elif DEVICE_PCBA_REV_A
	// TBD
#endif
}
