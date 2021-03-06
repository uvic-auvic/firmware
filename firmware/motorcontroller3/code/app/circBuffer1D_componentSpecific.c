/*
 * circBuffer1D_componentSpecific.c
 *
 *  Created on: Aug 6, 2020
 *      Author: Poornachander
 */

#include "circBuffer1D.h"

#include "debug.h"

extern const circBuffer1D_channelConfig_S circBuffer1D_channelConfig[CIRCBUFFER1D_CHANNEL_COUNT];
const circBuffer1D_channelConfig_S circBuffer1D_channelConfig[CIRCBUFFER1D_CHANNEL_COUNT] =
{
	[CIRCBUFFER1D_CHANNEL_DEBUG_TX] = { .size = DEBUG_TX_BUFFER_LENGTH },
};
