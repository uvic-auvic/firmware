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
#include "ADC.h"
#include "I2C.h"

/*
 * If a new FreeRTOS task is needed, create it here so that there is one place where
 * all the tasks are created
 */
void RTOS_init(void)
{
    LED_init();
    circBuffer1D_init();
    debug_init();
    ADC_init();
    CAN_init();
    messageHandler_init();
    I2C_init();

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
	/*
	 * Test case: Send instructions to the sensor and then read the data from the slave
	 *            First and second data are messages to access and write to the slave's register
	 *            The rest are junk, slave will respond with a NACK
	 *            length = 2 for normal operation, length = 3 to test acknowledge failure error handling
	 */
	uint8_t data[] = {0x0, 0b0100000, 0b1110, 0b0101};
	uint8_t* a = &data[0];
	I2C_send(I2C_CHANNEL_VBATT, a, 4);
	while (!is_idle()){}
	I2C_receive(I2C_CHANNEL_VBATT, 1);
}
