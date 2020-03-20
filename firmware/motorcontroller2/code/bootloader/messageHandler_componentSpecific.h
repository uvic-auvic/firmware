/*
 * messageHandler_componentSpecific.h
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef MESSAGEHANDLER_COMPONENTSPECIFIC_H_
#define MESSAGEHANDLER_COMPONENTSPECIFIC_H_

typedef enum
{
    MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POLARIS_REQUEST,

    MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT,
} messageHandler_RXMessageChannel_E;

typedef enum
{
    MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID,

    MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT,
} messageHandler_TXMessageChannel_E;


#endif /* MESSAGEHANDLER_COMPONENTSPECIFIC_H_ */