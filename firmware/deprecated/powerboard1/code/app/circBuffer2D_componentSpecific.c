	/*
 * circBuffer2D_componentSpecific.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer2D.h"

#include "UARTF0.h"

static uint8_t circBuffer2D_buffer_UARTTX[UART_TX_BUFFER_LENGTH][UART_TX_BUFFER_COUNT];

const circBuffer2D_config_S circBuffer2D_config[CIRCBUFFER2D_CHANNEL_COUNT] =
{
	[CIRCBUFFER2D_CHANNEL_UART_TX] =
	{
		.buffer = (uint8_t **)&circBuffer2D_buffer_UARTTX[0U],
		.length = UART_TX_BUFFER_LENGTH,
		.count = UART_TX_BUFFER_COUNT,
	}
};
