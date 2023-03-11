/*
 * PowerBoard.c
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 */

#include "Powerboard.h"

// Volatile tells the compiler that this value will change during runtime.
volatile LED_Channel gLED_PWR;
volatile LED_Channel gLED_1;
volatile LED_Channel gLED_2;
//volatile I2C_Channel gI2C_3;


void MainLoop(void){
	// Initialize Channels
	HardwareInit();
	// Priorities are 0 to 5. Reserve 5 for reed switch
	xTaskCreate(vBlinkyLedTask, "LED1", configMINIMAL_STACK_SIZE, (void *)&gLED_1, 0, 0);
	xTaskCreate(vBlinkyLedTask, "LED2", configMINIMAL_STACK_SIZE, (void *)&gLED_2, 0, 0);
	xTaskCreate(vBlinkyLedTask, "LEDpwr", configMINIMAL_STACK_SIZE, (void *)&gLED_PWR, 0, 0);
//	xTaskCreate(vI2cSensorSI7021Task, "SI7021", configMINIMAL_STACK_SIZE, (void *)&gI2C3, 1, 0); //Pass in I2C_Channel

	/* Start the kernel.  From here on, only tasks and interrupts will run. */
	vTaskStartScheduler();
	for(;;);
}

/***************************************************************************//**
 * HardwareInit - Initialize the hardware
 ******************************************************************************/
void HardwareInit(void){
	NVIC_SetPriorityGrouping( 0 );
	gLED_PWR =  LED_Init(P0, GPIOB, LED_GREEN, BLINK_SLOW);
	gLED_1 =  LED_Init(P14, GPIOB, LED_RED, BLINK_NORMAL);
	gLED_2 =  LED_Init(P7, GPIOB, LED_BLUE, BLINK_FAST);
//	gI2C_3 = I2C_Init(P8, P9, I2C3);
}
