/*
 * RTOS_componentSpecific.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

#include "stm32f0xx.h"
#include "UART.h"
#include "circBuffer1D.h"
#include "circBuffer2D.h"
#include "LED.h"
#include "ADC.h"
#include "messageHandler.h"
#include "sensors.h"

/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
	LED_init();
	circBuffer1D_init();
	circBuffer2D_init();
	UART_init(); // Creates a RTOS task
	ADC_init();
	messageHandler_init();
	sensors_init();
}

void RTOS_run1ms(void)
{
	messageHandler_run1ms();
}

void RTOS_run10ms(void)
{
	sensors_run10ms();
}

void RTOS_run100ms(void)
{
	LED_run100ms();
}

void RTOS_run1000ms(void)
{

}
