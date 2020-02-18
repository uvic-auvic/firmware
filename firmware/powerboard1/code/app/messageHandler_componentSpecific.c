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
#include "sensors.h"
#include "powerManagement.h"

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
                    ._5VPowerEnable = true,
                    ._12V9VPowerEnable = true,
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
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_ENV_DATA] =
        {
            .messageID = protocol_MID_PB_envData,
            .messageLength = sizeof(protocol_PBEnvData_S),
        },
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_BATT_VOLTAGES] =
        {
            .messageID = protocol_MID_PB_battVoltages,
            .messageLength = sizeof(protocol_PBBattVoltages_S),
        },
        [MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_BATT_CURRENTS] =
        {
            .messageID = protocol_MID_PB_battCurrents,
            .messageLength = sizeof(protocol_PBBattCurrents_S),
        }
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
                const messageHandler_TXMessageChannel_E TXChannel = messageHandler_componentSpecific_translatePolarisMessageRequest(receivedData->POLARIS_messageRequest.requestedMessage);
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
            memcpy(message->PB_deviceName.name, "PWR_BRD", 7U);

            break;
        }

        case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_ENV_DATA:
        {
            message->PB_envData.extPressure = sensors_getExternalPressure();
            break;
        }

        case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_BATT_VOLTAGES:
        {
            message->PB_battVoltages.leftBattVoltage = powerManagement_getBatteryVoltage_mV(POWER_MANAGEMENT_BATTERY_CHANNEL_LEFT);
            message->PB_battVoltages.rightBattVoltage = powerManagement_getBatteryVoltage_mV(POWER_MANAGEMENT_BATTERY_CHANNEL_RIGHT);
            break;
        }
        case MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_BATT_CURRENTS:
        {
            message->PB_battCurrents.leftBattCurrent = powerManagement_getBatteryCurrent_mA(POWER_MANAGEMENT_BATTERY_CHANNEL_LEFT);
            message->PB_battCurrents.rightBattCurrent = powerManagement_getBatteryCurrent_mA(POWER_MANAGEMENT_BATTERY_CHANNEL_RIGHT);
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
        case PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_ENV_DATA;
            break;
        }
        case PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_VOLTAGES:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_BATT_VOLTAGES;
            break;
        }
        case PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_CURRENTS:
        {
            ret = MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_BATT_CURRENTS;
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
