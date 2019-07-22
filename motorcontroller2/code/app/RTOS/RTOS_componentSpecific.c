/*
 * RTOS_componentSpecific.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

#include "LED.h"

/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
	LED_init();
}

void RTOS_run1ms(void)
{
	LED_run12();
}

void RTOS_run10ms(void)
{
	LED_run13();
}

void RTOS_run100ms(void)
{
	LED_run14();
}

void RTOS_run1000ms(void)
{
	LED_run15();
}

