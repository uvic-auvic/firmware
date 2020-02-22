/*
 * messageHandler_componentSpecific.c
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "messageHandler.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "motorRPMFeedback.h"

static void messageHandler_componentSpecific_messageReceivedCallback(const messageHandler_RXMessageChannel_E channel, const protocol_allMessages_U * const receivedData);
static void messageHandler_componentSpecific_messagePopulateCallback(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message);
static inline messageHandler_TXMessageChannel_E messageHandler_componentSpecific_translatePolarisMessageRequest(const protocol_MCMessageRequest_message_E message);

extern const messageHandler_config_S messageHandler_config;
const messageHandler_config_S messageHandler_config =
{
    .RXMessageConfig =
    {
        [MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POLARIS_REQUEST] =
        {
            .messageID = protocol_MID_POLARIS_MCMessageRequest,
            .initValue = {.POLARIS_MCMessageRequest = {.requestedMessage = PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT}},
        },
        [MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_MOTOR_SPEED] =
        {
            .messageID = protocol_MID_POLARIS_motorSetSpeed,
            .initValue = {.POLARIS_motorSetSpeed = {.motorSpeed = {0U}}},
        },
    },

    .TXMessageConfig =
    {
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID] =
        {
            .messageID = protocol_MID_MC_deviceName,
            .messageLength = sizeof(protocol_deviceName_S),
        },
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_MOTOR_RPM_LOW] =
        {
            .messageID = protocol_MID_MC_motorRPMLow,
            .messageLength = sizeof(protocol_motorRPM_S),
        },
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_MOTOR_RPM_HIGH] =
        {
            .messageID = protocol_MID_MC_motorRPMHigh,
            .messageLength = sizeof(protocol_motorRPM_S),
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
            case MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POLARIS_REQUEST:
            {
                const messageHandler_TXMessageChannel_E TXChannel = messageHandler_componentSpecific_translatePolarisMessageRequest(receivedData->POLARIS_PBMessageRequest.requestedMessage);
                messageHandler_dispatchMessage(TXChannel);

                break;
            }
            case MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_MOTOR_SPEED:
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

static void messageHandler_componentSpecific_messagePopulateCallback(const messageHandler_TXMessageChannel_E channel, protocol_allMessages_U * const message)
{
    if(message != NULL)
    {
        switch(channel)
        {
            case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID:
            {
                memset(message, 0U, sizeof(*message));
                memcpy(message->PB_deviceName.name, "MTR_CONT", 8U);

                break;
            }
            case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_MOTOR_RPM_LOW:
            {
                for(motorDriver_channel_E channel = (motorDriver_channel_E)0U; channel < (motorDriver_channel_E)4U; channel++)
                {
                    message->MC_motorRPMLow.motorSpeed[channel] = motorRPMFeedback_getMotorRPM(channel);
                }
                break;
            }
            case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_MOTOR_RPM_HIGH:
            {
                for(motorDriver_channel_E channel = (motorDriver_channel_E)4U; channel < MOTOR_DRIVER_CHANNEL_COUNT; channel++)
                {
                    message->MC_motorRPMHigh.motorSpeed[channel - 4U] = motorRPMFeedback_getMotorRPM(channel);
                }
                break;
            }
            case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT:
            default:
            {
                break;
            }
        }
    }
}

static inline messageHandler_TXMessageChannel_E messageHandler_componentSpecific_translatePolarisMessageRequest(const protocol_MCMessageRequest_message_E message)
{
    messageHandler_TXMessageChannel_E ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT;
    switch(message)
    {
        case PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RID:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_RID;
            break;
        }
        case PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_LOW:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_MOTOR_RPM_LOW;
            break;
        }
        case PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_HIGH:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_MOTOR_RPM_HIGH;
            break;
        }
        case PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT:
        default:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT;
            break;
        }
    }

    return ret;
}
