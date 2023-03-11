/*
 * UART.h
 *
 *  Created on: Aug 13, 2018
 *      Author: Poornachander
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

#define INPUT_BUFFER_SIZE_BYTES		32
#define OUTPUT_BUFFER_SIZE_BYTES	64

/* COMMUNICATION PROTOCOL DEFINES */
#define MAX_DATA_SIZE_BYTES		1024 //Max packet size will be this value plus the headers and CRC


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


typedef struct commBuffer {
	uint8_t isRecivingHeader;
	uint8_t isRecivingPayload;
	uint8_t previousPacketID;
	uint16_t payloadSize;
	uint8_t payload[MAX_PAYLOAD_SIZE_BYTES];
}commBuffer_t;

extern commBuffer_t * receiveBuffer_ptr;

extern void init_UART();
extern int UART_push_out(char * mesg);
extern int UART_push_out_len(char* mesg, int len);

#ifdef DEBUG

extern void UART_push_out_len_debug(char * message, uint8_t length);
extern void UART_push_out_debug(char * message);

#endif

#endif /* UART_H_ */
