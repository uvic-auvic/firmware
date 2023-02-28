/*
 * User_Defines.h
 *
 *  Created on: Aug 18, 2018
 *      Author: Poornachander
 */

#ifndef USER_DEFINES_H_
#define USER_DEFINES_H_

#define TRUE	(1)
#define FALSE	(0)

/* DEBUG DEFINES */
#define FORCE_LOAD_APP				FALSE
#define FORCE_LOAD_BOOTLOADER		FALSE //Overrides FORCE_LOAD_APP
#define OVERRIDE_CRC_CHECK			TRUE
#define OVERRIDE_PACKET_ID_CHECK	FALSE

/* CONFIG DEFINES */
#define DEVICEID	"Motor Controller"

/* BOOTLOADER DEFINES */
#define START_COMMAND_WAIT_MS	(2000)

/* MEMORY DEFINES. Should match up with application linker script */
#define BOOTLOADER_START_ADDRESS	0x08000000
#define BOOTLOADER_SECTION_SIZE		0x8000
#define APP_START_ADDRESS 			0x08008000
#define APP_SECTION_SIZE			0x78000

/* UART DEFINES*/
#define UART_TO_USE			(USART1) //Example: USART1 or USART2. Has to match define from ST Library
#define GPIO_PORT_TO_USE 	(GPIOB) //Example: GPIOA or GPIOB. Has to match define from ST Library
#define TX_PIN_TO_USE		(6) // 0 to 15. Example 1 or 2.
#define RX_PIN_TO_USE		(7) // 0 to 15. Example 1 or 2.
#define BAUD_RATE			(9600)
#define DMA					(DMA2) // Only the DMA controller. Either DMA1 or DMA2
#define DMA_STREAM			(DMA2_Stream2) //Has to be DMA Stream typedef Example: DMA2_Stream2, DMA1_Stream4

/* COMMUNICATION PROTOCOL DEFINES */
#define MAX_DATA_SIZE_BYTES		1024 //Max packet size will be this value plus the headers and CRC

/* LED DEFINES */
#define LED_GPIO_PORT 	(GPIOA)
#define LED1			(GPIO_Pin_4)
#define LED2			(GPIO_Pin_5)
#define LED3			(0)
#define LED4			(0)
#define LED_TO_BLINK	(LED2)

/* OTHER DEFINES (Should not be modified by user) */

#define DEVICEID_PTR (char *){DEVICEID}

/* COMMUNICATION PROTOCOL DEFINES (NOT USER MODIFIABLE) */
#define PAYLOAD_INFO_SIZE_BYTES		2
#define PACKET_ID_SIZE_BYTES		2
#define DATA_TYPE_SIZE_BYTES		2
#define CRC_SIZE_BYTES				4
#define MAX_PAYLOAD_SIZE_BYTES		(PACKET_ID_SIZE_BYTES + DATA_TYPE_SIZE_BYTES + MAX_DATA_SIZE_BYTES + CRC_SIZE_BYTES)
#define DATA_TYPE_OFFSET			(PACKET_ID_SIZE_BYTES)
#define PACKET_ID_OFFSET			(0)
#define DATA_OFFSET					(DATA_TYPE_OFFSET + DATA_TYPE_SIZE_BYTES)
#define NOT_DATA_SIZE_BYTES			(PACKET_ID_SIZE_BYTES + DATA_TYPE_SIZE_BYTES + CRC_SIZE_BYTES)
#define PROGRAM_SIZE_SIZE_BYTES		2
#define PROGRAM_SIZE_OFFSET			(DATA_TYPE_OFFSET + DATA_TYPE_SIZE_BYTES)
#define FIRMWARE_VERSION_SIZE_BYTES	8
#define FIRMWARE_VERSION_OFFSET 	(PROGRAM_SIZE_OFFSET + PROGRAM_SIZE_SIZE_BYTES)
#define DEVICE_ID_MAX_SIZE_BYTES	(64)
#define DEVICE_ID_OFFSET			(FIRMWARE_VERSION_OFFSET + FIRMWARE_VERSION_SIZE_BYTES)

/* DATA TYPE DEFINES */

/* CHECKS TO SEE IF DEFINES ARE VALID */

#if (BOOTLOADER_SECTION_SIZE % 4) != 0
#error "BOOTLOADER_SECTION_SIZE must be a multiple of 4"
#endif

#if (CONFIG_SECTION_SIZE % 4) != 0
#error "CONFIG_SECTION_SIZE must be a multiple of 4"
#endif

#if (MAX_PAYLOAD_SIZE_BYTES % 4) != 0
#error "MAX_PAYLOAD_SIZE_BYTES must be a multiple of 4"
#endif

#endif /* USER_DEFINES_H_ */
