/*
 * ISOTP_UART.c
 */

#include "ISOTP_UART.h"

#include "isotp.h"
#include "RTOS.h"
#include "utils.h"
#include "UART.h"

#define ISOTP_UART_MAX_SIZE	(1024U)

typedef struct
{
	IsoTpLink linkHandle;
	uint8_t RXBuffer[ISOTP_UART_MAX_SIZE];
	uint8_t TXBuffer[ISOTP_UART_MAX_SIZE];
	uint8_t lastReceivedData[ISOTP_UART_MAX_SIZE];
} ISOTP_UART_data_S;

static ISOTP_UART_data_S ISOTP_UART_data;

extern const ISOTP_UART_config_S ISOTP_UART_config;

void ISOTP_UART_init(void)
{
	memset(&ISOTP_UART_data, 0U, sizeof(ISOTP_UART_data));
	isotp_init_link(&ISOTP_UART_data.linkHandle, ISOTP_UART_config.TXMessageID, ISOTP_UART_data.TXBuffer, sizeof(ISOTP_UART_data.TXBuffer), ISOTP_UART_data.RXBuffer, sizeof(ISOTP_UART_data.RXBuffer));
}

void ISOTP_UART_run1ms(void)
{
	isotp_poll(&ISOTP_UART_data.linkHandle);

	uint16_t receivedDataSize;
	bool ret = isotp_receive(&ISOTP_UART_data.linkHandle, ISOTP_UART_data.lastReceivedData, sizeof(ISOTP_UART_data.lastReceivedData), &receivedDataSize);
	if (ISOTP_RET_OK == ret) {
		/* Handle received message */
		ISOTP_UART_config.messageReceivedCallback(ISOTP_UART_data.lastReceivedData, receivedDataSize);
	}
}

bool ISOTP_UART_sendISOTPMessage(const uint8_t * const data, const uint16_t length)
{
	bool ret = false;

	if((data != NULL) && (length > 0U) && (length <= ISOTP_UART_MAX_SIZE))
	{
		if(isotp_send(&ISOTP_UART_data.linkHandle, data, length) == ISOTP_RET_OK)
		{
			ret = true;
		}
	}

	return ret;
}

bool ISOTP_UART_frameReceivedCallback(const protocol_message_S * const message, const uint8_t length)
{
	if((message != NULL) && (length > 0U) && (length <= 8U))
	{
		if(message->messageID == ISOTP_UART_config.RXMessageID)
		{
			isotp_on_can_message(&ISOTP_UART_data.linkHandle, (uint8_t *)&message->message, (uint8_t)length);
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
	uint32_t ret = 0U;
	if((data != NULL) && (size <= 8U) && (size > 0U))
	{
		protocol_message_S frameToSend;
		frameToSend.messageID = arbitration_id;
		memcpy(&frameToSend.message, data, size);
		ret = UART_writeLen((const uint8_t * const)&frameToSend, size) ? 1U : 0U;
	}

	return ret;
}

void isotp_user_debug(const char* message, ...)
{
	UNUSED(message);
}
