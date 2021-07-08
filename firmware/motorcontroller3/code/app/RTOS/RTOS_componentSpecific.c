/*
 * RTOS_componentSpecific.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Poornachander
 */

#include "RTOS.h"

#include "LED.h"
#include "circBuffer1D.h"
#include "debug.h"
#include "CAN.h"
#include "messageHandler.h"
#include "string.h"
#include "SPI.h"
/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
    LED_init();
    circBuffer1D_init();
    debug_init();
    CAN_init();
    messageHandler_init();

    circBuffer2D_init();
    SPI_init();

    debug_writeStringBlocking("*** MOTOR CONTROLLER 3 ***");
    debug_writeStringBlocking("Initialization Complete");
}

void RTOS_run1ms(void)
{
    messageHandler_run1ms();
}

void RTOS_run10ms(void)
{
	SPI_run();

}

void RTOS_run100ms(void)
{
    LED_run100ms();
}

void RTOS_run1000ms(void)
{

}
