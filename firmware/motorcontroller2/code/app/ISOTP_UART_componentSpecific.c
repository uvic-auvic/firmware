/*
 * ISOTP_UART_componentSpecific.c
 *
 *  Created on: Feb 20, 2020
 *      Author: Poornachander
 */

#include "utils.h"
#include "ISOTP_UART.h"

static void ISOTP_UART_componentSpecific_messageReceivedCallback(const ISOTP_UART_channel_E channel, const protocol_ISOTP_allMessages_U * const data, const uint16_t length);

static const ISOTP_UART_channelConfig_S ISOTP_UART_channelConfig[ISOTP_UART_CHANNEL_COUNT] =
{
	[ISOTP_UART_CHANNEL_LOOPBACK] = { .messageID.MC_ISOTP_MID = MOTORCONTROLLER_ISOTP_MID_LOOPBACK},
};

const ISOTP_UART_config_S ISOTP_UART_config =
{
	.RXMessageID = protocol_MID_POLARIS_MCISOTP,
	.TXMessageID = protocol_MID_MC_ISOTP,
	.messageReceivedCallback = ISOTP_UART_componentSpecific_messageReceivedCallback,
	.channelConfig =
	{
		[ISOTP_UART_CHANNEL_LOOPBACK] = { .messageID.MC_ISOTP_MID = MOTORCONTROLLER_ISOTP_MID_LOOPBACK},
	},
};

static void ISOTP_UART_componentSpecific_messageReceivedCallback(const ISOTP_UART_channel_E channel, const protocol_ISOTP_allMessages_U * const data, const uint16_t length)
{
	UNUSED(data);
	UNUSED(length);
	UNUSED(channel);
	switch(channel)
	{
		case ISOTP_UART_CHANNEL_LOOPBACK:
		{
			(void)ISOTP_UART_sendISOTPMessage(ISOTP_UART_CHANNEL_LOOPBACK, data, length);
			break;
		}
		case ISOTP_UART_CHANNEL_COUNT:
		default:
		{
			break;
		}
	}
}
