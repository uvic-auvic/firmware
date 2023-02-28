/*
 * PowerBoard.c
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 *
 * Changelog:
 * - PR 1: Initial PR.
 *
 */

#include "Powerboard.h"

// Volatile informs compiler not to optimize this code. It would remove it otherwise.
static volatile LED_Channel gLED_PWR;
static volatile LED_Channel gLED_CAN;
static volatile LED_Channel gLED_UART;
static volatile LED_Channel gLED_USB;
static volatile UART_Channel gUART_DEBUG_RX;
static volatile UART_Channel gUART_DEBUG_TX;
static volatile UART_Channel gUART_RX;
static volatile UART_Channel gUART_TX;


void MainLoop(void){
	// Initialize Channels
	HardwareInit();

	// Create Queues Semiphores, mutexes, etc for RTOS
	vQueueInit();
	vSemiphoreInit();
	vMutexInit();

	// Create the tasks for the scheduler
	vTaskInit();

	/* Start the kernel.  From here on, only tasks and interrupts will run. */
	vTaskStartScheduler();
	for(;;);
}

void HardwareInit(void){
	NVIC_SetPriorityGrouping( 0 );
	gLED_PWR =  LED_Init(P0, GPIOB, LED_GREEN, BLINK_NORMAL);
	gLED_UART =  LED_Init(P7, GPIOB, LED_BLUE, BLINK_NORMAL);
	gLED_CAN =  LED_Init(P14, GPIOB, LED_RED, BLINK_NORMAL);
	gUART_DEBUG_RX = UART_Init(P12, GPIOA, AF11, UART4, UART_MODE_RX);
	gUART_DEBUG_TX = UART_Init(P11, GPIOA, AF11, UART4, UART_MODE_TX);
	gUART_RX = UART_Init(P12, GPIOB, UART5, AF11, UART_MODE_RX);
	gUART_TX = UART_Init(P13, GPIOB, UART5, AF11, UART_MODE_TX);
}

/*
 *
 * RTOS Tools
 *
 * */
void vQueueInit(void){

}
void vSemiphoreInit(void){

}
void vMutexInit(void){

}

/*
 *
 * RTOS Tasks
 * Tasks are actions in the scheduler. For example, one task can be to poll a potentiometer value,
 * another can be to update a shift register from this changed value.
 *
 * */

void vTaskInit(void){

}
