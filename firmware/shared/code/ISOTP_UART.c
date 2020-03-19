/*
 * ISOTP_UART.c
 */

#include "ISOTP_UART.h"

#include "isotp.h"
#include "RTOS.h"
#include "utils.h"
#include "UART.h"
#include "assert.h"

typedef struct
{
	IsoTpLink linkHandle;
	protocol_ISOTP_message_S RXBuffer;
	protocol_ISOTP_message_S TXBuffer;
	protocol_ISOTP_message_S lastReceivedData;
} ISOTP_UART_data_S;

static ISOTP_UART_data_S ISOTP_UART_data;

extern const ISOTP_UART_config_S ISOTP_UART_config;

void ISOTP_UART_init(void)
{
	memset(&ISOTP_UART_data, 0U, sizeof(ISOTP_UART_data));
	isotp_init_link(&ISOTP_UART_data.linkHandle, ISOTP_UART_config.TXMessageID, (uint8_t *)&ISOTP_UART_data.TXBuffer, sizeof(ISOTP_UART_data.TXBuffer), (uint8_t *)&ISOTP_UART_data.RXBuffer, sizeof(ISOTP_UART_data.RXBuffer));
}

void ISOTP_UART_run1ms(void)
{
	isotp_poll(&ISOTP_UART_data.linkHandle);

	uint16_t receivedDataSize;
	bool ret = isotp_receive(&ISOTP_UART_data.linkHandle, (uint8_t *)&ISOTP_UART_data.lastReceivedData, sizeof(ISOTP_UART_data.lastReceivedData), &receivedDataSize);
	if (ISOTP_RET_OK == ret)
	{
		if(receivedDataSize >= sizeof(protocol_ISOTP_MID_U))
		{
			for(ISOTP_UART_channel_E channel = (ISOTP_UART_channel_E)0U; channel < ISOTP_UART_CHANNEL_COUNT; channel++)
			{
				if(ISOTP_UART_config.channelConfig[channel].messageID.asUint8 == ISOTP_UART_data.lastReceivedData.messageID.asUint8)
				{
					ISOTP_UART_config.messageReceivedCallback(channel, &ISOTP_UART_data.lastReceivedData.message, receivedDataSize);
					break;
				}
			}
		}
	}
}

bool ISOTP_UART_sendISOTPMessage(const ISOTP_UART_channel_E channel, const protocol_ISOTP_allMessages_U * const data, const uint16_t length)
{
	bool ret = false;
	if(channel < ISOTP_UART_CHANNEL_COUNT)
	{
		if((data != NULL) && (length > 0U) && (length <= sizeof(protocol_ISOTP_allMessages_U)))
		{
			if(isotp_send(&ISOTP_UART_data.linkHandle, (uint8_t *)data, length) == ISOTP_RET_OK)
			{
				ret = true;
			}
		}
	}

	return ret;
}

void ISOTP_UART_frameReceivedCallback(const protocol_message_S * const message, const uint8_t length)
{
	if((message != NULL) && (length > 0U) && (length <= 9U))
	{
		if(message->messageID == ISOTP_UART_config.RXMessageID)
		{
			isotp_on_can_message(&ISOTP_UART_data.linkHandle, (uint8_t *)&message->message, (uint8_t)(length - sizeof(protocol_MID_E)));
		}
	}
}

// Shims
uint32_t isotp_user_get_ms(void)
{
	return RTOS_getTimeMilliseconds();
}

int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size)
{
	uint32_t ret = ISOTP_RET_ERROR;
	if((data != NULL) && (size <= 8U) && (size > 0U))
	{
		protocol_message_S frameToSend;
		frameToSend.messageID = arbitration_id;
		memcpy(&frameToSend.message, data, size);
		ret = UART_writeLen((const uint8_t * const)&frameToSend, size + sizeof(frameToSend.messageID)) ? ISOTP_RET_OK : ISOTP_RET_OVERFLOW;
	}

	return ret;
}

void isotp_user_debug(const char* message, ...)
{
	UNUSED(message);
}
