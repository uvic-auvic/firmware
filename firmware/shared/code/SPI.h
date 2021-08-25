/*
 * SPI.h
 *
 *  Created on: Aug 10, 2020
 *      Author: carte
 * Modified by: Shuhang
 *
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "protocol.h"
#include "circBuffer2D.h"
#include "SPI_componentSpecific.h"

typedef struct
{
	// SCK pin info
	GPIO_TypeDef*	SCKPort;
	uint8_t 		SCKPin;

	// MISO pin info
	GPIO_TypeDef*	MISOPort;
	uint8_t 		MISOPin;

	// MOSI pin info
	GPIO_TypeDef*	MOSIPort;
	uint8_t 		MOSIPin;

	// CS/SS pin info
	GPIO_TypeDef*	CSPort;
	uint8_t 		CSPin;

	// SPI peripheral info
	SPI_TypeDef*    SPIPeriph;
	uint8_t         SPIAFNum;

} SPI_HWConfig_S;

typedef struct
{
	const SPI_HWConfig_S * const HWConfig;
	void(*messageReceivedCallback)(const protocol_MID_E messageID, const protocol_allMessages_U * const message);
} SPI_config_S;

/*typedef struct
{
	SPI_4,

	SPI_CHANNEL_COUNT
} SPI_channel_E;*/

void SPI_init(void);
void SPI_run(void);
bool SPI_send(circBuffer2D_channel_E, const uint8_t * const data, const uint8_t length);
bool SPI_sendAndReceive(circBuffer2D_channel_E channel,  uint8_t * const data, const uint8_t length);

#endif /* SPI_H_ */
