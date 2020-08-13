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

    debug_writeStringBlocking("*** POWER BOARD 2 ***");
    debug_writeStringBlocking("Initialization Complete");
}

void RTOS_run1ms(void)
{
    messageHandler_run1ms();
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
    protocol_allMessages_U message;
    messageHandler_getMessage(MESSAGE_HANDLER_RX_CHANNEL_TESTER, &message, NULL);
    message.PB_deviceName.name[7U] = 0;
}
