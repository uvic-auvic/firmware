/*
 * SPI_componentSpecific.c
 *
 *  Created on: Aug 22, 2021
 *      Author: Shuhang Tan
 */

#include "SPI.h"
#include "SPI_componentSpecific.h"
#include "messageHandler.h"

// SPI config information
static const SPI_HWConfig_S SPI_HWConfig =
{
	// SCK pin info
	.SCKPort = GPIOA,
	.SCKPin  = 5,

	// MISO pin info
	.MISOPort = GPIOA,
	.MISOPin  = 6,

	// MOSI pin info
	.MOSIPort = GPIOA,
	.MOSIPin  = 7,

	// CS/SS pin info
	.CSPort = GPIOD,
	.CSPin  = 14,

	// SPI peripheral info
	.SPIPeriph = SPI1,
	.SPIAFNum = 0x05,
};

static void SPI_messageReceivedCallback(const protocol_MID_E messageID, const protocol_allMessages_U * const message)
{
	messageHandler_messageReceivedCallback(messageID, message);
}


extern const SPI_config_S SPI_config;
const SPI_config_S SPI_config =
{
	.HWConfig = &SPI_HWConfig,
	.messageReceivedCallback = SPI_messageReceivedCallback,
};
