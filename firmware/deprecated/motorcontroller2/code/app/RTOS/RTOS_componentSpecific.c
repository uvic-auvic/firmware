/*
 * RTOS_componentSpecific.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

#include "LED.h"
#include "circBuffer1D.h"
#include "circBuffer2D.h"
#include "UARTF4.h"
#include "messageHandler.h"
#include "motorDriver.h"
#include "motorRPMFeedback.h"
#include "motorInterface.h"
#include "ISOTP_UART.h"

/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
	LED_init();
	circBuffer1D_init();
	circBuffer2D_init();
	UART_init(); // Creates an RTOS task
	messageHandler_init();
	motorDriver_init();
	motorRPMFeedback_init();
	motorInterface_init();
	ISOTP_UART_init();
}

void RTOS_run1ms(void)
{
	messageHandler_run1ms();
	ISOTP_UART_run1ms();
}

void RTOS_run10ms(void)
{
	motorRPMFeedback_run10ms();
	motorInterface_run10ms();
}

void RTOS_run100ms(void)
{
	LED_run100ms();
}

void RTOS_run1000ms(void)
{

}

