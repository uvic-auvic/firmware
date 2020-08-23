/*
 * circBuffer1D.c
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#include "circBuffer1D.h"
#include <stdlib.h>
#include "utils.h"
#include "assert.h"
#include <string.h>

typedef struct
{
	uint32_t startIndex;
	uint32_t endIndex;
	uint32_t size;
	uint32_t head; // Head is always with reference to 0
	uint32_t tail; // Tail is always with reference to 0
	bool empty;
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
			circBuffer1D_data.channelData[channel].empty = true;
		}

		assert(bufferSize <= sizeof(circBuffer1D_data.buffer));
	}
}

uint32_t circBuffer1D_getSpaceAvailable(const circBuffer1D_channel_E channel)
{
	uint32_t ret = 0U;
	if(channel < CIRCBUFFER1D_CHANNEL_COUNT)
	{
		circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];

		if(channelData->empty == false && channelData->head == channelData->tail ){
			ret = 0;
		}
		else if (channelData->empty == true){
			ret = channelData->size;
		}
		else if(channelData->head > channelData->tail)
		{
			ret = channelData->size - (channelData->head - channelData->tail);
		}
		else if(channelData->tail > channelData->head)
		{
			ret =channelData->tail - channelData->head;
		}else {
		}
	}

	return ret;
}



bool circBuffer1D_pushByte(const circBuffer1D_channel_E channel, const uint8_t data)
{
	bool ret = false;
	if(channel < CIRCBUFFER1D_CHANNEL_COUNT)
	{
		circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];
		if(circBuffer1D_getSpaceAvailable(channel) > 0U)
		{
			circBuffer1D_data.buffer[channelData->startIndex + channelData->head] = data;
			channelData->head = (channelData->head + 1U) % channelData->size;
			ret = true;
			channelData->empty = false;
		}
	}

	return ret;
}

bool circBuffer1D_push(const circBuffer1D_channel_E channel, const uint8_t * const data, const uint32_t size)
{
	bool ret = false;
	circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];
	if((channel < CIRCBUFFER1D_CHANNEL_COUNT) && (data != NULL) && (size > 0U))
	{
		if((circBuffer1D_getSpaceAvailable(channel) > 0U) && (channelData->size >= size))
		{
			ret = true;
			for(uint32_t i = 0U; i < size; i++)
			{
				circBuffer1D_pushByte(channel, data[i]);
				
			}
		}
	}
	return ret;
}

bool circBuffer1D_popByte(const circBuffer1D_channel_E channel, uint8_t * const returnData)
{
	bool ret = false;
	if(channel < CIRCBUFFER1D_CHANNEL_COUNT)
	{
		circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];
		if(channelData->head != channelData->tail || channelData->empty == false )
		{
			*returnData = circBuffer1D_data.buffer[channelData->tail + channelData->startIndex];
			channelData->tail = (channelData->tail + 1U) % channelData->size;
			if (returnData != NULL)
			{
				ret = true;
			} else
			{
				ret = false;
			}
			if(channelData->head == channelData->tail)
			{
				channelData->empty = true;
			}
		}
		
	}

	return ret;
}

uint8_t circBuffer1D_pop(const circBuffer1D_channel_E channel, uint8_t * const dataToReturn)
{
	uint8_t ret = 0;
	uint8_t *returnData = 0;
	circBuffer1D_channelData_S * channelData = &circBuffer1D_data.channelData[channel];
	if (channel < CIRCBUFFER1D_CHANNEL_COUNT && channelData->empty == false)
	{
		uint32_t data_size = channelData->size - circBuffer1D_getSpaceAvailable(channel);
		memset(dataToReturn, 0U, data_size );
		for (uint32_t i = 0U; i < data_size; i++)
		{
			if (circBuffer1D_popByte(channel, returnData ) == true)
			{
			dataToReturn[i]= *returnData;
			ret++;
			}
		}
		
		
	}
	return ret;
}