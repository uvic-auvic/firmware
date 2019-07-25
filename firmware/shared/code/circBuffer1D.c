/*
 * circBuffer1D.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer1D.h"
#include <stdlib.h>

typedef struct
{
	uint32_t startIndex;
	uint32_t endIndex;
	uint32_t size;
	uint32_t head; // Head is always with reference to 0
	uint32_t tail; // Tail is always with reference to 0
} circBuffer1D_channelData_S;

typedef struct
{
	uint8_t buffer[CIRCBUFFER1D_TOTAL_BUFFER_SIZE];
	circBuffer1D_channelData_S channelData[CIRCBUFFER1D_CHANNEL_COUNT];
} circBuffer1D_data_S;

static circBuffer1D_data_S circBuffer1D_data;

extern const circBuffer1D_channelConfig_S circBuffer1D_channelConfig[CIRCBUFFER1D_CHANNEL_COUNT];
void circBuffer1D_init(void)
{
	uint32_t bufferSize = 0U;
	for(circBuffer1D_channel_E channel = (circBuffer1D_channel_E)0U; channel < CIRCBUFFER1D_CHANNEL_COUNT; channel++)
	{
		if(circBuffer1D_channelConfig[channel].size > 0U)
		{
			circBuffer1D_data.channelData[channel].startIndex = bufferSize;
			circBuffer1D_data.channelData[channel].size = circBuffer1D_channelConfig[channel].size;
			bufferSize += circBuffer1D_channelConfig[channel].size;
			circBuffer1D_data.channelData[channel].endIndex = bufferSize - 1U;
		}

		if(bufferSize > sizeof(circBuffer1D_data.buffer))
		{
			// ASSERT?
		}
	}
}

uint8_t circBuffer1D_getSpaceAvailable(const circBuffer1D_channel_E channel)
{
	uint8_t ret = 0U;
	if(channel < CIRCBUFFER1D_CHANNEL_COUNT)
	{
		circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];

		if(channelData->head > channelData->tail)
		{
			ret = channelData->head - channelData->tail;
		} else
		{
			ret = (channelData->head + channelData->size) - channelData->tail;
		}
	}

	return ret;
}

bool circBuffer1D_popByte(const circBuffer1D_channel_E channel, uint8_t * const returnData)
{
	bool ret = false;
	if((channel < CIRCBUFFER1D_CHANNEL_COUNT) && (returnData != NULL))
	{
		circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];
		if(channelData->head != channelData->tail)
		{
			*returnData = circBuffer1D_data.buffer[channelData->tail + channelData->startIndex];
			channelData->tail = (channelData->tail + 1U) % channelData->size;
			ret = true;
		}
	}

	return ret;
}

bool circBuffer1D_pushByte(const circBuffer1D_channel_E channel, const uint8_t data)
{
	bool ret = false;
	if(channel < CIRCBUFFER1D_CHANNEL_COUNT)
	{
		if(((circBuffer1D_data.channelData[channel].head + 1U) % (circBuffer1D_data.channelData[channel].size)) != circBuffer1D_data.channelData[channel].tail)
		{
			circBuffer1D_data.buffer[circBuffer1D_data.channelData[channel].startIndex + circBuffer1D_data.channelData[channel].head] = data;
			circBuffer1D_data.channelData[channel].head += 1U;
			ret = true;
		}
	}

	return ret;
}

bool circBuffer1D_push(const circBuffer1D_channel_E channel, const uint8_t * const data, const uint8_t size)
{
	bool ret = false;

	if((channel < CIRCBUFFER1D_CHANNEL_COUNT) && (data != NULL) && (size > 0U) && (circBuffer1D_getSpaceAvailable(channel) >= size))
	{
		ret = true;
		for(uint8_t index = 0U; index < size; index++)
		{
			if(circBuffer1D_pushByte(channel, data[index]) == false)
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}
