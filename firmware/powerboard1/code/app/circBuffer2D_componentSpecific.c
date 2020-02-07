	/*
 * circBuffer2D_componentSpecific.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer2D.h"

#include "UART.h"

static uint8_t circBuffer2D_buffer_UARTRX[UART_RX_BUFFER_LENGTH][UART_RX_BUFFER_COUNT];

const circBuffer2D_config_S circBuffer2D_config[CIRCBUFFER2D_CHANNEL_COUNT] =
{
	[CIRCBUFFER2D_CHANNEL_UART_RX] =
	{
		.buffer = (uint8_t **)&circBuffer2D_buffer_UARTRX[0],
		.length = UART_RX_BUFFER_LENGTH,
		.count = UART_RX_BUFFER_COUNT
	}
};
