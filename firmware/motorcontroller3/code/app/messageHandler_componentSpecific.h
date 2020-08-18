/*
 * messageHandler_componentSpecific.h
 *
 *  Created on: Aug 12, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef MESSAGEHANDLER_COMPONENTSPECIFIC_H_
#define MESSAGEHANDLER_COMPONENTSPECIFIC_H_

typedef enum
{
    MESSAGE_HANDLER_RX_CHANNEL_TESTER,

    MESSAGE_HANDLER_RX_CHANNEL_COUNT,

} messageHandler_RXMessageChannel_E;

typedef enum
{
    MESSAGE_HANDLER_TX_CHANNEL_RID,

    MESSAGE_HANDLER_TX_CHANNEL_COUNT,
} messageHandler_TXMessageChannel_E;


#endif /* MESSAGEHANDLER_COMPONENTSPECIFIC_H_ */
