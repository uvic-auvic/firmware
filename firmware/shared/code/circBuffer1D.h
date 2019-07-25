/*
 * circBuffer1D.h
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#ifndef SHARED_CODE_CIRCBUFFER1D_H_
#define SHARED_CODE_CIRCBUFFER1D_H_

#include "circBuffer1D_componentSpecific.h"
#include <stdbool.h>
#include <stdint.h>


#ifndef CIRCBUFFER1D_TOTAL_BUFFER_SIZE
#error "Need to define 'CIRCBUFFER1D_TOTAL_BUFFER_SIZE' in circBuffer1D_componentSpecific.h"
#endif

typedef struct
{
	const uint32_t size;
} circBuffer1D_channelConfig_S;

void circBuffer1D_init(void);
uint8_t circBuffer1D_getSpaceAvailable(const circBuffer1D_channel_E channel);
bool circBuffer1D_popByte(const circBuffer1D_channel_E channel, uint8_t * const returnData);
bool circBuffer1D_pushByte(const circBuffer1D_channel_E channel, const uint8_t data);
bool circBuffer1D_push(const circBuffer1D_channel_E channel, const uint8_t * const data, const uint8_t size);


#endif /* SHARED_CODE_CIRCBUFFER1D_H_ */
