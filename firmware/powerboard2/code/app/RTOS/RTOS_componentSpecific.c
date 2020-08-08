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

/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
    LED_init();
    circBuffer1D_init();
    debug_init();

    debug_writeString("*** POWER BOARD 2 ***\n");
    debug_writeString("Initialization Complete\n");
}

void RTOS_run1ms(void)
{

}

void RTOS_run10ms(void)
{

}

void RTOS_run100ms(void)
{
    LED_run100ms();
}

void RTOS_run1000ms(void)
{

}

