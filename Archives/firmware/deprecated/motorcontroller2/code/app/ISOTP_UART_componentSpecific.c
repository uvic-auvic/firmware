/*
 * ISOTP_UART_componentSpecific.c
 *
 *  Created on: Feb 20, 2020
 *      Author: Poornachander
 */

#include "utils.h"
#include "ISOTP_UART.h"

static void ISOTP_UART_componentSpecific_messageReceivedCallback(const uint8_t * const data, const uint16_t length);

const ISOTP_UART_config_S ISOTP_UART_config =
{
	.RXMessageID = protocol_MID_TRIDENT_MCISOTP,
	.TXMessageID = protocol_MID_MC_ISOTP,
	.messageReceivedCallback = ISOTP_UART_componentSpecific_messageReceivedCallback,
};

static void ISOTP_UART_componentSpecific_messageReceivedCallback(const uint8_t * const data, const uint16_t length)
{
	UNUSED(data);
	UNUSED(length);
}
