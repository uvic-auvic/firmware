/*
 * circBuffer2D_componentSpecific.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer2D.h"
#include "circBuffer2D_componentSpecific.h"

static uint8_t circBuffer2D_buffer_TEST1[16U][16U];

const circBuffer2D_config_S circBuffer2D_config[CIRCBUFFER2D_CHANNEL_COUNT] =
{
	[SPI_RX] =
	{
		.buffer = (uint8_t **)&circBuffer2D_buffer_TEST1[0U],
		.length = 16U,
		.count = 16U,
	},

	[SPI_TX] =
	{
		.buffer = (uint8_t **)&circBuffer2D_buffer_TEST1[0U],
		.length = 16U,
		.count = 16U,
	}
};
