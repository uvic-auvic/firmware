/*
 * ISOTP_UART.c
 */

#include "ISOTP_UART.h"

#include "isotp.h"
#include "RTOS.h"

#define ISOTP_UART_MAX_SIZE	(1000U)

typedef struct
{
	IsoTpLink linkHandle;
	uint8_t RXBuffer[ISOTP_UART_MAX_SIZE];
	uint8_t TXBuffer[ISOTP_UART_MAX_SIZE];
	uint8_t lastReceivedData[ISOTP_UART_MAX_SIZE]
} ISOTP_UART_data_S;

static ISOTP_UART_data_S ISOTP_UART_data;

void ISOTP_UART_init(void)
{
	memset(&ISOTP_UART_data, 0U, sizeof(ISOTP_UART_data));
	isotp_init_link(&ISOTP_UART_data.linkHandle, , ISOTP_UART_data.TXBuffer, sizeof(ISOTP_UART_data.TXBuffer, ISOTP_UART_data.RXBuffer, sizeof(ISOTP_UART_data.RXBuffer)));
}

void ISOTP_UART_run1ms(void)
{
	isotp_poll(&ISOTP_UART_data.linkHandle);

	uint16_t receivedDataSize;

	bool ret = isotp_receive(&ISOTP_UART_data.linkHandle, ISOTP_UART_data.lastReceivedData, sizeof(ISOTP_UART_data.lastReceivedData), &receivedDataSize);
	if (ISOTP_RET_OK == ret) {
		/* Handle received message */
	}
}

void ISOTP_UART_messageReceivedCallback()
{
	isotp_on_can_message(&ISOTP_UART_data.linkHandle, message, sizeof(message));
}

// Shims
uint32_t isotp_user_get_ms(void)
{
	return RTOS_getTimeMilliseconds();
}

int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size)
{

}
