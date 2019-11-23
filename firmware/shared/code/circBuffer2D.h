#ifndef SHARED_CODE_CIRCBUFFER2D_H_
#define SHARED_CODE_CIRCBUFFER2D_H_

#include <stdint.h>
#include <stdbool.h>

#include "circBuffer2D_componentSpecific.h"


typedef struct
{
	uint8_t ** buffer;
	uint16_t length;
	uint16_t count;
} circBuffer2D_config_S;

void circBuffer2D_init(void);
bool circBuffer2D_getSpaceAvailable(const circBuffer2D_channel_E channel);
bool circBuffer2D_push(const circBuffer2D_channel_E channel, uint8_t const * const data, const uint16_t length);
uint8_t circBuffer2D_pop(const circBuffer2D_channel_E channel, uint8_t * const dataToReturn);

#endif /* SHARED_CODE_CIRCBUFFER1D_H_ */
