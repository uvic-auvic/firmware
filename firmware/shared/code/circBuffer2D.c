/*Buffer - Circular FIFO Buffer implementation*/

#include "circBuffer2D.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	uint8_t ** buffer;
	uint16_t  head;
	uint16_t  tail;
	uint16_t  length;   //Length of slots
	uint16_t  count;    //Number of slots in a channel
	bool empty;

} circBuffer2D_channelData_S;

typedef struct
{
	circBuffer2D_channelData_S	channelData[CIRCBUFFER2D_CHANNEL_COUNT];
} circBuffer2D_data_S;

circBuffer2D_data_S circBuffer2D_data;

extern circBuffer2D_config_S circBuffer2D_config[CIRCBUFFER2D_CHANNEL_COUNT];
void circBuffer2D_init(void)
{
	for(circBuffer2D_channel_E channel = (circBuffer2D_channel_E)0U; channel < CIRCBUFFER2D_CHANNEL_COUNT; channel++)
	{
		circBuffer2D_config_S const * const channelConfig = &circBuffer2D_config[channel];
		circBuffer2D_channelData_S * const channelData = &circBuffer2D_data.channelData[channel];
		if(channelConfig->buffer != NULL)
		{
			channelData->buffer = channelConfig->buffer;
			channelData->length = channelConfig->length;
			channelData->count = channelConfig->count;
			channelData->empty = true;
		} else
		{
			//ASSERT???
		}
	}
}

uint8_t circBuffer2D_getSpaceAvailable(const circBuffer2D_channel_E channel)
{
	uint8_t ret = 0;
	if(channel < CIRCBUFFER2D_CHANNEL_COUNT)
	{

		circBuffer2D_channelData_S * channelData = &circBuffer2D_data.channelData[channel];
		if(channelData->empty == false && channelData->head == channelData->tail ){
			ret = 0;
		}
		else if (channelData->empty == true){
			ret = channelData->count;
		}
		else if(channelData->head > channelData->tail)
		{
			ret = channelData->count - channelData->head - channelData->tail;
		}
		else if(channelData->tail > channelData->head)
		{
			ret = (channelData->head + channelData->count) - channelData->tail;
		}else {
		}
	}

	return ret;
}

bool circBuffer2D_push(const circBuffer2D_channel_E channel, uint8_t const * const data, const uint16_t length)
{

	bool ret = false;

	if((channel < CIRCBUFFER2D_CHANNEL_COUNT) && (data != NULL) && (length > 0))
	{
		circBuffer2D_channelData_S * channelData = &circBuffer2D_data.channelData[channel];
		if((circBuffer2D_getSpaceAvailable(channel) > 0) && (channelData->length >= length))
		{
			memset(&channelData->buffer[channelData->head], 0U, channelData->length);
			memcpy(&channelData->buffer[channelData->head], data, length);
			channelData->head = (channelData->head + 1) % (channelData->count);
			ret = true;
			channelData->empty = false;
		}
	}

	return ret;
}

uint8_t circBuffer2D_pop(const circBuffer2D_channel_E channel, uint8_t * const dataToReturn)
{
	uint8_t ret = 0U;
	if((channel < CIRCBUFFER2D_CHANNEL_COUNT) && (dataToReturn != NULL))
	{
		
		circBuffer2D_channelData_S * channelData = &circBuffer2D_data.channelData[channel];
		if(channelData->head != channelData->tail || channelData->empty == false)
		{
			
			memset(dataToReturn, 0U, channelData->length);
			memcpy(dataToReturn, &channelData->buffer[channelData->tail], channelData->length);
			channelData->tail = (channelData->tail + 1U)  % (channelData->count);

			if(channelData->head == channelData->tail)
			{
				channelData->empty = true;
			}

			ret = channelData->length;
		}
	}

	return ret;
}
