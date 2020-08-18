/*
 * messageHandler_componentSpecific.c
 *
 *  Created on: Aug 12, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "messageHandler.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"

static void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const message);
static void messageHandler_componentSpecific_messagePopulateCallback(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message);

extern const messageHandler_config_S messageHandler_config;
const messageHandler_config_S messageHandler_config =
{
    .RXMessageConfig =
    {

        [MESSAGE_HANDLER_RX_CHANNEL_TESTER] =
        {
            .messageID = protocol_MID_MC_deviceName,
            .callbackEnable = true,
            .initValue = 
            {  
                .MC_deviceName =
                {
                    .name = "TEST"
                },
            },
        },
    },

    .TXMessageConfig =
    {
        [MESSAGE_HANDLER_TX_CHANNEL_RID] =
        {
            .messageID = protocol_MID_MC_deviceName,
            .messageLength = sizeof(protocol_deviceName_S),
            .messagePeriod = MESSAGE_HANDLER_PERIOD_1000MS,
        },
    },
    .messageReceivedCallback = messageHandler_componentSpecific_messageReceivedCallback,
    .messagePopulateCallback = messageHandler_componentSpecific_messagePopulateCallback,
};

static void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const message)
{
    if(message != NULL)
    {
        switch(channel)
        {
            case MESSAGE_HANDLER_RX_CHANNEL_TESTER:
            {
                debug_writeString("REC: %s", message->MC_deviceName.name);
                break;
            }

            case MESSAGE_HANDLER_RX_CHANNEL_COUNT:
            default:
            {

                break;
            }
        }
    }
}

static void messageHandler_componentSpecific_messagePopulateCallback(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message)
{
    if(message != NULL)
    {
        switch(channel)
        {
            case MESSAGE_HANDLER_TX_CHANNEL_RID:
            {
                memcpy(message->MC_deviceName.name, "MTR_BRD", 7U);

                break;
            }
            case MESSAGE_HANDLER_TX_CHANNEL_COUNT:
            default:
            {
                break;
            }
        }
    }
}
