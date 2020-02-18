#ifndef SHARED_DATA_UARTPROTOCOL_H_
#define SHARED_DATA_UARTPROTOCOL_H_

#include <stdint.h>

#define UART_PROTOCOL_MAX_PAYLOAD_SIZE  (10U)

#if UART_PROTOCOL_MAX_PAYLOAD_SIZE > 255U
#error "UART_PROTOCOL_MAX_PAYLOAD_SIZE must be smaller than 255"
#endif

typedef struct __attribute__((packed))
{
	// Sent first
	struct
	{
		uint8_t		length; // Length does not include crc
	} header;

	// Sent second
	struct
	{
		uint16_t	crc;
		uint8_t 	payload[UART_PROTOCOL_MAX_PAYLOAD_SIZE];
	} data;
} UARTProtocol_protocol_S;

#endif // SHARED_DATA_UARTPROTOCOL_H_
