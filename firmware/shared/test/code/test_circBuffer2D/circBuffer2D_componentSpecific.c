	/*
 * circBuffer2D_componentSpecific.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer2D.h"

static uint8_t circBuffer2D_buffer_TEST1[5U][5U];

const circBuffer2D_config_S circBuffer2D_config[CIRCBUFFER2D_CHANNEL_COUNT] =
{
	[CIRCBUFFER2D_CHANNEL_TEST_1] =
	{
		.buffer = (uint8_t **)&circBuffer2D_buffer_TEST1[0U],
		.length = 5U,
		.count = 5U,
	}
};
