/*
 * messageHandler.c
 *
 *  Created on: Feb 9, 2020
 *      Author: Poorna Chander Kalidas
 */


#include "messageHandler.h"

/* INCLUDES */
#include <stdlib.h>
#include <string.h>
#include "RTOS.h"
#if USE_UART
#include "UART.h"
#endif
#if USE_CAN
#include "CAN.h"
#endif

#include "debug.h"

/* DEFINES */

/* TYPEDEFS */
typedef struct
{
    protocol_allMessages_U message;
    uint32_t    timeReceived;
    bool newDataAvailable;
} messageHandler_RXChannelData_S;

typedef struct
{
    uint32_t timeDispatched;
    bool dispatchRequested;
} messageHandler_TXChannelData_S;

typedef struct
{
    messageHandler_RXChannelData_S RXChannelData[MESSAGE_HANDLER_RX_CHANNEL_COUNT];
    messageHandler_TXChannelData_S TXChannelData[MESSAGE_HANDLER_TX_CHANNEL_COUNT];
} messageHandler_data_S;

/* PRIVATE DATA DEFINITIONS */
static messageHandler_data_S messageHandler_data;

extern const messageHandler_config_S messageHandler_config;

/* PRIVATE FUNCTIONS DECLARATION */
static uint32_t messageHandler_private_getMessagePeriodms(const messageHandler_TXMessageChannel_E channel);

/* PRIVATE FUNCTION DEFINITIONS */
static uint32_t messageHandler_private_getMessagePeriodms(const messageHandler_TXMessageChannel_E channel)
{
    uint32_t ret = 0U;
    if(channel < MESSAGE_HANDLER_TX_CHANNEL_COUNT)
    {
        switch(messageHandler_config.TXMessageConfig[channel].messagePeriod)
        {
            case MESSAGE_HANDLER_PERIOD_10MS:
            {
                ret = 10U;
                break;
            }
            case MESSAGE_HANDLER_PERIOD_100MS:
            {
                ret = 100U;
                break;
            }
            case MESSAGE_HANDLER_PERIOD_1000MS:
            {
                ret = 1000U;
                break;
            }
            default:
            {
                ret = 0U;
                break;
            }
        }
    }

    return ret;
}

/* PUBLIC FUNCTION DEFINITIONS */
void messageHandler_init(void)
{
    memset(&messageHandler_data, 0U, sizeof(messageHandler_data));
    for (messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_CHANNEL_COUNT; channel++)
    {
        const messageHandler_RXMessageConfig_S * const RXChannelConfig = &messageHandler_config.RXMessageConfig[channel];
        messageHandler_RXChannelData_S * const RXChannelData = &messageHandler_data.RXChannelData[channel];
        
        // Init message
        memcpy(&RXChannelData->message, &RXChannelConfig->initValue, sizeof(RXChannelConfig->initValue));

        // Add RX message to filter, use RX message channel as filter number
        CAN_filterAdd(RXChannelConfig->messageID, (uint16_t)channel);
    }

    for (messageHandler_TXMessageChannel_E channel = (messageHandler_TXMessageChannel_E)0U; channel < MESSAGE_HANDLER_TX_CHANNEL_COUNT; channel++)
    {
        messageHandler_TXChannelData_S * const TXChannelData = &messageHandler_data.TXChannelData[channel];

        TXChannelData->dispatchRequested = true;
    }
}

void messageHandler_run1ms(void)
{
    // RX
    for(messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_CHANNEL_COUNT; channel++)
    {
        messageHandler_RXChannelData_S * const channelData = &messageHandler_data.RXChannelData[channel];
        const messageHandler_RXMessageConfig_S * const channelConfig = &messageHandler_config.RXMessageConfig[channel];

        if(channelData->newDataAvailable)
        {
            if(channelConfig->callbackEnable)
            {
                if(messageHandler_config.messageReceivedCallback != NULL)
                {
                    messageHandler_config.messageReceivedCallback(channel, &channelData->message);
                }
            }

            channelData->newDataAvailable = false;
        }
    }

    // TX
    for(messageHandler_TXMessageChannel_E channel = (messageHandler_TXMessageChannel_E)0U; channel < MESSAGE_HANDLER_TX_CHANNEL_COUNT; channel++)
    {
        messageHandler_TXChannelData_S * const channelData = &messageHandler_data.TXChannelData[channel];
        const messageHandler_TXMessageConfig_S * const channelConfig = &messageHandler_config.TXMessageConfig[channel];

        if(RTOS_getTimeElapsedMilliseconds(channelData->timeDispatched) >= messageHandler_private_getMessagePeriodms(channel))
        {
            channelData->dispatchRequested = true;
        }

        if(channelData->dispatchRequested)
        {
            if(messageHandler_config.messagePopulateCallback != NULL)
            {
                // Fill message
                protocol_allMessages_U message; // A separate variable is passed into the function below to get around the memory alignment problem
                messageHandler_config.messagePopulateCallback(channel, &message);
#if USE_UART
                protocol_message_S frame;
                memcpy(&frame.message, &message, sizeof(message));
                frame.messageID = channelConfig->messageID;

                // Add message to the UART queue
                UART_writeLen((const uint8_t * const)&frame, channelConfig->messageLength + sizeof(frame.messageID));
                channelData->dispatchRequested = false;
                channelData->timeDispatched = RTOS_getTimeMilliseconds();
#endif

#if USE_CAN
                if(CAN_sendMessage(channelConfig->messageID, &message, channelConfig->messageLength))
                {
                    channelData->dispatchRequested = false;
                    channelData->timeDispatched = RTOS_getTimeMilliseconds();
                }
#endif
            }
        }
    }
}

bool messageHandler_getMessage(const messageHandler_RXMessageChannel_E channel, protocol_allMessages_U * const message, uint32_t * const timeReceived)
{
    bool ret = false;

    if((channel < MESSAGE_HANDLER_RX_CHANNEL_COUNT) && (message != NULL))
    {
        *message = messageHandler_data.RXChannelData[channel].message;
        ret = true;

        if(timeReceived != NULL)
        {
            *timeReceived = messageHandler_data.RXChannelData[channel].timeReceived;
        }
    }

    return ret;
}

void messageHandler_dispatchMessage(const messageHandler_TXMessageChannel_E channel)
{
    if(channel < MESSAGE_HANDLER_TX_CHANNEL_COUNT)
    {
        messageHandler_data.TXChannelData[channel].dispatchRequested = true;
    }
}

void messageHandler_messageReceivedCallback(const protocol_MID_E messageID, const protocol_allMessages_U * const message)
{
    if(message != NULL)
    {
        for(messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_CHANNEL_COUNT; channel++)
        {
            if(messageID == messageHandler_config.RXMessageConfig[channel].messageID)
            {
                messageHandler_data.RXChannelData[channel].timeReceived = RTOS_getTimeMilliseconds();
                memcpy(&messageHandler_data.RXChannelData[channel].message, message, sizeof(protocol_allMessages_U));
                messageHandler_data.RXChannelData[channel].newDataAvailable = true;

                break;
            }
        }
    }
}
