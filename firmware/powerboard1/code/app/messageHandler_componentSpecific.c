/*
 * messageHandler_componentSpecific.c
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "messageHandler.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f0xx.h"

static void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const receivedData);
static void messageHandler_componentSpecific_messagePopulateCallback(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message);
static inline messageHandler_TXMessageChannel_E messageHandler_componentSpecific_translatePolarisMessageRequest(const protocol_PBMessageRequest_message_E message);

extern const messageHandler_config_S messageHandler_config;
const messageHandler_config_S messageHandler_config =
{
    .RXMessageConfig =
    {
        [MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POWER_ENABLE] = 
        {
            .messageID = protocol_MID_POLARIS_powerEnable,
            .initValue = 
            {
                .POLARIS_powerEnable =
                {
                    .motorPowerEnable = true,
                    .systemPowerEnable = true,
                    ._5VPowerEnable = true,
                    ._9VPowerEnable = true,
                    ._12VPowerEnable = true,
                },
            },
        },
        [MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POLARIS_REQUEST] =
        {
            .messageID = protocol_MID_POLARIS_PBMessageRequest,
            .initValue = {.POLARIS_messageRequest = {0U}},
        }
    },

    .TXMessageConfig =
    {
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID] =
        {
            .messageID = protocol_MID_PB_deviceName,
            .messageLength = sizeof(protocol_deviceName_S),
        },
    },
    .messageReceivedCallback = messageHandler_componentSpecific_messageReceivedCallback,
    .messagePopulateCallback = messageHandler_componentSpecific_messagePopulateCallback,
};

static void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const receivedData)
{
    if(receivedData != NULL)
    {
        switch(channel)
        {
            case MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POWER_ENABLE:
            {
                break;
            }

            case MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POLARIS_REQUEST:
            {
                const messageHandler_TXMessageChannel_E TXChannel = receivedData->POLARIS_messageRequest.requestedMessage;
                messageHandler_dispatchMessage(TXChannel);

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

static void messageHandler_componentSpecific_messagePopulateCallback(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message)
{
    switch(channel)
    {
        case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID:
        {
            memset(message, 0U, sizeof(*message));
            memcpy(message, "PWR_BRD", 7U);

            break;
        }

        case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT:
        default:
        {
            break;
        }
    }
}

static inline messageHandler_TXMessageChannel_E messageHandler_componentSpecific_translatePolarisMessageRequest(const protocol_PBMessageRequest_message_E message)
{
    messageHandler_TXMessageChannel_E ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT;
    switch(message)
    {
        case PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID;
            break;
        }

        case PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_EXT_PRESSURE:
        {

            break;
        }

        case PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT:
        default:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT;
            break;
        }
    }

    return ret;
}