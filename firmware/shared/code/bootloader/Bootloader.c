/*
 * Bootloader.c
 *
 *  Created on: Aug 18, 2018
 *      Author: Poornachander
 */

#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

#include "User_Defines.h"
#include "Bootloader.h"

#include "../UARTDma.h"
#include "Flash_Interface.h"

/* DEFINES */
#define BOOTLOADER_QUEUE_LENGTH	4
#define BOOTLOADER_QUEUE_SIZE	1

/* GLOBAL VARIABLES */
QueueHandle_t Bootloader_Queue_Handle = NULL;

static flashBuffer_t flashBuffer_var;
flashBuffer_t * flashBuffer_ptr = &flashBuffer_var;

static firmwareInfo_t firmwareInfo_var;
firmwareInfo_t * firmwareInfo_ptr = &firmwareInfo_var;

/**
 * Initialize flash buffer
 */
static void init_flash_buffer() {
	flashBuffer_ptr->length = 0;
	flashBuffer_ptr->startingAddress = APP_START_ADDRESS;
	flashBuffer_ptr->mutex = xSemaphoreCreateMutex();
}

/**
 * Checks that the app sector data is intact
 * Returns:  1 - App data intact
 * 			-1 - App data corrupt
 */
static int8_t is_app_sector_valid() {
	//Do checksum
#pragma message "TO DO:Need to do app integrity checking on startup"
	return 1;
}

/**
 * Programs the data currently in the flash buffer into the flash and verifies that it was programmed correctly
 * Returns:  1 - Programming success
 * 			-1 - Attempted write to restricted flash sector
 * 			-2 - Verify failed
 * 			-3 - Mutex error
 */
static int8_t program_code_in_buffer() {

	if (xSemaphoreTake(flashBuffer_ptr->mutex, FLASHBUFFER_MUTEX_TIMEOUT)) {
		//Start writing to flash
	 	for(uint16_t i = 0; i < flashBuffer_ptr->length; i += 4) {
			if(write_word(flashBuffer_ptr->startingAddress + i, *(uint32_t *)(flashBuffer_ptr->data + i)) == -1) {
				//ERROR: Accessing restricted flash sector
#pragma message "TO DO:Return error through UART"
				xSemaphoreGive(flashBuffer_ptr->mutex);
				return -1;
			}
		}

		//Verify that data has been written properly
		for(uint16_t i = 0; i < flashBuffer_ptr->length; i += 4) {

			if(read_word(flashBuffer_ptr->startingAddress + i) != *(uint32_t *)(flashBuffer_ptr->data + i)) {
				//ERROR: Flash data does not match expected data
#pragma message "TO DO:Return error through UART"
				xSemaphoreGive(flashBuffer_ptr->mutex);
				return -2;
			}
		}

	} else {
		//Could not get mutex
#pragma message "TO DO:Return error through UART"
		return -3;
	}

	xSemaphoreGive(flashBuffer_ptr->mutex);
	return 1;
}

/* Sets all peripheral registers to reset value and turns off clock*/
static void deinit_peripherals() {
	//Reset all peripherals
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST | RCC_AHB1RSTR_DMA2RST | RCC_AHB1RSTR_GPIOARST | RCC_AHB1RSTR_GPIOBRST | RCC_AHB1RSTR_GPIOCRST
			| RCC_AHB1RSTR_GPIODRST | RCC_AHB1RSTR_GPIOERST | RCC_AHB1RSTR_GPIOHRST | RCC_AHB1RSTR_CRCRST;
	RCC->AHB2RSTR |= RCC_AHB2RSTR_OTGFSRST;
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST | RCC_APB1RSTR_I2C2RST | RCC_APB1RSTR_I2C3RST | RCC_APB1RSTR_USART2RST | RCC_APB1RSTR_SPI2RST
			 | RCC_APB1RSTR_SPI3RST | RCC_APB1RSTR_TIM5RST | RCC_APB1RSTR_TIM4RST | RCC_APB1RSTR_TIM3RST | RCC_APB1RSTR_TIM2RST;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST | RCC_APB2RSTR_SPI4RST | RCC_APB2RSTR_SPI5RST | RCC_APB2RSTR_TIM1RST | RCC_APB2RSTR_TIM10RST
			| RCC_APB2RSTR_TIM11RST | RCC_APB2RSTR_TIM9RST | RCC_APB2RSTR_USART1RST | RCC_APB2RSTR_USART6RST | RCC_APB2RSTR_SDIORST | RCC_APB2RSTR_ADCRST;

	//Write bits back to 0
	RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_DMA1RST | RCC_AHB1RSTR_DMA2RST | RCC_AHB1RSTR_GPIOARST | RCC_AHB1RSTR_GPIOBRST | RCC_AHB1RSTR_GPIOCRST
			| RCC_AHB1RSTR_GPIODRST | RCC_AHB1RSTR_GPIOERST | RCC_AHB1RSTR_GPIOHRST);
	RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_OTGFSRST);
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_I2C1RST | RCC_APB1RSTR_I2C2RST | RCC_APB1RSTR_I2C3RST | RCC_APB1RSTR_USART2RST | RCC_APB1RSTR_SPI2RST
			 | RCC_APB1RSTR_SPI3RST | RCC_APB1RSTR_TIM5RST | RCC_APB1RSTR_TIM4RST | RCC_APB1RSTR_TIM3RST | RCC_APB1RSTR_TIM2RST);
	RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST | RCC_APB2RSTR_SPI4RST | RCC_APB2RSTR_SPI5RST | RCC_APB2RSTR_TIM1RST | RCC_APB2RSTR_TIM10RST
			| RCC_APB2RSTR_TIM11RST | RCC_APB2RSTR_TIM9RST | RCC_APB2RSTR_USART1RST | RCC_APB2RSTR_USART6RST | RCC_APB2RSTR_SDIORST | RCC_APB2RSTR_ADCRST);

	//Turn off clock to all peripherals
	RCC->AHB1ENR &= ~(RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN
			| RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOHEN | RCC_AHB1ENR_CRCEN);
	RCC->AHB2ENR &= ~(RCC_AHB2ENR_OTGFSEN);
	RCC->APB1ENR &= ~(RCC_APB1ENR_I2C1EN | RCC_APB1ENR_I2C2EN | RCC_APB1ENR_I2C3EN | RCC_APB1ENR_USART2EN | RCC_APB1ENR_SPI2EN
			| RCC_APB1ENR_SPI3EN | RCC_APB1ENR_TIM5EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM2EN);
	RCC->APB2ENR &= ~(RCC_APB2ENR_SPI1EN | RCC_APB2ENR_SPI4EN | RCC_APB2ENR_SPI5EN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_TIM10EN
			| RCC_APB2ENR_TIM11EN | RCC_APB2ENR_TIM9EN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_USART6EN | RCC_APB2ENR_SDIOEN | RCC_APB2ENR_ADC1EN);
}

// Starts the application
static void startApplication() {

	/* Deinit peripherals */
	deinit_peripherals();

	/* Lock flash */
	deinit_flash();

	/* Branch to Application */
	typedef void (*pFunction)(void);
	pFunction appEntry;
	uint32_t appStack;

	/* Get the application stack pointer (First entry in the application vector table) */
	appStack = (uint32_t) *((__IO uint32_t*)APP_START_ADDRESS);

	/* Get the application entry point (Second entry in the application vector table) */
	appEntry = (pFunction) *(__IO uint32_t*) (APP_START_ADDRESS + 4);

	__disable_irq();

	// Disable IRQs
	for(uint8_t i = 0;i < 8;i++)
	{
	    NVIC->ICER[i] = 0xFFFFFFFF;
	}
	// Clear pending IRQs
	for(uint8_t i = 0;i < 8;i++)
	{
	    NVIC->ICPR[i] = 0xFFFFFFFF;
	}

	__enable_irq();

	SysTick->CTRL =0;
	SysTick->LOAD=0;
	SysTick->VAL=0;

	/* Reconfigure vector table offset register to match the application location */
	SCB->VTOR = APP_START_ADDRESS;

	/* Set the application stack pointer */
	__set_MSP(appStack);

	/* Start the application */
	appEntry();
}

static void Bootloader_Main_Task() {

	Bootloader_Queue_Handle = xQueueCreate(BOOTLOADER_QUEUE_LENGTH, BOOTLOADER_QUEUE_SIZE);
	char queueCommand = 0;

	{
		uint8_t queueExitStatus = xQueueReceive(Bootloader_Queue_Handle, &queueCommand, pdMS_TO_TICKS(START_COMMAND_WAIT_MS));

		if(queueExitStatus == pdFALSE && FORCE_LOAD_BOOTLOADER == 0) {

			if(is_app_sector_valid() == 1 || FORCE_LOAD_APP == 1) {

				startApplication();
			} else {
#pragma message "TO DO:Return error through UART"
				//ERROR: App sector corrupt. Cannot boot.
			}
		}
	}

	/* BEGIN BOOTLOADING */
	{
		//Send out device ID and max data size
		while(UART_push_out("DEVICE_ID:") == -2);
		while(UART_push_out(DEVICEID) == -2);
		while(UART_push_out("\r\n") == -2);
		while(UART_push_out("MAX_PACKET_SIZE:") == -2);
		char maxPacketSizeSTR[6] = {};
		itoa(MAX_PAYLOAD_SIZE_BYTES, maxPacketSizeSTR, 10);
		while(UART_push_out(maxPacketSizeSTR) == -2);
		while(UART_push_out("\r\n") == -2);
	}

	//Erase app sector before loading program
#pragma message "TO DO:This need to be removed once first packet is implemented"
	erase_app_sector(); // Temporary

	while(1) {

		xQueueReceive(Bootloader_Queue_Handle, &queueCommand, portMAX_DELAY);

		switch (queueCommand) {
		case 'F':

			//Check if firmware is compatible with this device
			if(strncmp(DEVICEID, firmwareInfo_ptr->deviceID, DEVICE_ID_MAX_SIZE_BYTES)) {
				//ERROR: DEVICE ID DOES NOT MATCH
				UART_push_out("ERR:FIRMWARE_INCOMPATIBLE\r\n");
			} else if(firmwareInfo_ptr->programSize > APP_SECTION_SIZE) {
				char appSize[6];
				itoa(APP_SECTION_SIZE, appSize, 10);
				UART_push_out("ERR:FIRMWARE_TOO_LARGE: Available app section size is ");
				UART_push_out(appSize);
				UART_push_out(" bytes.\r\n");
			} else {
				//TO DO: Program firmware data into config section
				erase_app_sector();
			}
			break;

		case 'M':
			program_code_in_buffer();
			break;

		case 'L':
			break;

		default:
			//ERROR: No Matches
			break;
		}

	}

}

static void init_Bootloader() {

	init_flash_buffer();
	init_UART();
	init_flash();

	// Create the Bootloader Main task
	xTaskCreate(Bootloader_Main_Task,
		(const char *)"Bootloader_Main_Task",
		configMINIMAL_STACK_SIZE,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL              ); // pvCreatedTask */

}

extern void Bootloader_Start() {

	init_Bootloader();
	vTaskStartScheduler();

	//Should never reach this
	while(1);

}
