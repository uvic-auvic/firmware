/*
 * ISOTP_UART.h
 */

#ifndef ISOTP_UART_H_
#define ISOTP_UART_H_

#include "ISOTP_UART_componentSpecific.h"

#include <stdint.h>
#include "protocol.h"
#include "protocol_ISOTP.h"


/* Max number of messages the receiver can receive at one time, this value 
 * is affectied by can driver queue length
 */
#define ISO_TP_DEFAULT_BLOCK_SIZE   8

/* The STmin parameter value specifies the minimum time gap allowed between 
 * the transmission of consecutive frame network protocol data units
 */
#define ISO_TP_DEFAULT_ST_MIN       0

/* This parameter indicate how many FC N_PDU WTs can be transmitted by the 
 * receiver in a row.
 */
#define ISO_TP_MAX_WFT_NUMBER       1

/* Private: The default timeout to use when waiting for a response during a
 * multi-frame send or receive.
 */
#define ISO_TP_DEFAULT_RESPONSE_TIMEOUT 100

/* Private: Determines if by default, padding is added to ISO-TP message frames.
 */
// #define ISO_TP_FRAME_PADDING

typedef struct
{
    protocol_ISOTP_MID_U messageID;
} ISOTP_UART_channelConfig_S;

typedef struct
{
    uint16_t RXMessageID;
    uint16_t TXMessageID;
    void (* messageReceivedCallback)(const ISOTP_UART_channel_E channel, const protocol_ISOTP_allMessages_U * const data, const uint16_t length);
    ISOTP_UART_channelConfig_S channelConfig[ISOTP_UART_CHANNEL_COUNT];
} ISOTP_UART_config_S;

void ISOTP_UART_init(void);
void ISOTP_UART_run1ms(void);
bool ISOTP_UART_sendISOTPMessage(const ISOTP_UART_channel_E channel, const protocol_ISOTP_allMessages_U * const data, const uint16_t length);
void ISOTP_UART_frameReceivedCallback(const protocol_message_S * const message, const uint8_t length);

#endif /* ISOTP_UART_H_ */
