/*
 * circBuffer1D_componentSpecific.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer1D.h"
#include "UARTF4.h"

extern const circBuffer1D_channelConfig_S circBuffer1D_channelConfig[CIRCBUFFER1D_CHANNEL_COUNT];
const circBuffer1D_channelConfig_S circBuffer1D_channelConfig[CIRCBUFFER1D_CHANNEL_COUNT] =
{
	[CIRCBUFFER1D_CHANNEL_NULL] = { .size = 0 },
};
