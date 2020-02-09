/*
 * messageHandler.h
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef SHARED_CODE_MESSAGEHANDLER_H_
#define SHARED_CODE_MESSAGEHANDLER_H_

#include "messageHandler_componentSpecific.h"
#include "protocol.h"

typedef struct
{
    protocol_MID_E  messageID;
} messageHandler_RXMessageConfig_S;

typedef struct
{
    messageHandler_RXMessageConfig_S RXMessageConfig[MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT];
} messageHandler_config_S;


void messageHandler_init(void);
void messageHandler_run10ms(void);
void messageHandler_messageReceivedCallback(protocol_message_S const * const receiveData);

#endif /* SHARED_CODE_MESSAGEHANDLER_H_ */
