/*
 * messageHandler.h
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

#include "messageHandler_componentSpecific.h"
#include "protocol.h"

typedef enum
{
    MESSAGE_HANDLER_PERIOD_10MS,
    MESSAGE_HANDLER_PERIOD_100MS,
    MESSAGE_HANDLER_PERIOD_1000MS,
} messageHandler_period_E;

typedef struct
{
    protocol_MID_E  messageID;
    protocol_allMessages_U initValue;
    bool    callbackEnable;
} messageHandler_RXMessageConfig_S;

typedef struct
{
    protocol_MID_E  messageID;
    uint8_t         messageLength;
    messageHandler_period_E messagePeriod;

} messageHandler_TXMessageConfig_S;

typedef struct
{
    messageHandler_RXMessageConfig_S RXMessageConfig[MESSAGE_HANDLER_RX_CHANNEL_COUNT];
    messageHandler_TXMessageConfig_S TXMessageConfig[MESSAGE_HANDLER_TX_CHANNEL_COUNT];
    void (* messageReceivedCallback)(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const message);
    void (* messagePopulateCallback)(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message);
} messageHandler_config_S;

void messageHandler_init(void);
void messageHandler_run1ms(void);
bool messageHandler_getMessage(const messageHandler_RXMessageChannel_E channel, protocol_allMessages_U * const message, uint32_t * const timeReceived);
void messageHandler_dispatchMessage(const messageHandler_TXMessageChannel_E channel);
void messageHandler_messageReceivedCallback(const protocol_MID_E messageID, const protocol_allMessages_U * const message);

#endif /* MESSAGEHANDLER_H_ */
