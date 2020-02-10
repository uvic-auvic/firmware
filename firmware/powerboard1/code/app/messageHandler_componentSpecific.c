/*
 * messageHandler_componentSpecific.c
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "messageHandler.h"
#include <stdlib.h>
#include "stm32f0xx.h"

void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const receivedData);

extern const messageHandler_config_S messageHandler_config;
const messageHandler_config_S messageHandler_config =
{
    .RXMessageConfig =
    {
        [MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POWER_ENABLE] = { .messageID = protocol_MID_POLARIS_powerEnable}
    },

    .messageReceivedCallback = messageHandler_componentSpecific_messageReceivedCallback,
};

void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const receivedData)
{
    if(receivedData != NULL)
    {
        switch(channel)
        {
            case MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POWER_ENABLE:
            {
                break;
            }

            case MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT:
            default:
            {

                break;
            }
        }
    }
}
