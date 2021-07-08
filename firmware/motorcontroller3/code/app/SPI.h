/*
 * SPI.h
 *
 *  Created on: Aug 10, 2020
 *      Author: carte
 */

#ifndef MOTORCONTROLLER3_CODE_APP_SPI_H_
#define MOTORCONTROLLER3_CODE_APP_SPI_H_

#include <stdbool.h>
#include "circBuffer2D.h"

typedef enum
{
	SPI_4,

	SPI_CHANNEL_COUNT
} SPI_channel_E;

void SPI_init(void);
void SPI_run(void);
bool SPI_send(circBuffer2D_channel_E, const uint8_t * const data, const uint8_t length);
//bool SPI_receive(circBuffer2D_channel_E,  uint8_t * const data, const uint8_t length);
bool SPI_sendAndReceive(circBuffer2D_channel_E channel,  uint8_t * const data, const uint8_t length);

#endif /* MOTORCONTROLLER2_CODE_APP_SPI_H_ */
