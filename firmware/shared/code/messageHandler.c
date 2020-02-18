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
#include "UART.h"

/* DEFINES */

/* TYPEDEFS */
typedef struct
{
    bool newDataAvailable;
    protocol_allMessages_U message;
    uint32_t    timeReceived;
} messageHandler_RXChannelData_S;

typedef struct
{
    bool dispatchRequested;
} messageHandler_TXChannelData_S;

typedef struct
{
    messageHandler_RXChannelData_S RXChannelData[MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT];
    messageHandler_TXChannelData_S TXChannelData[MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT];
} messageHandler_data_S;

/* PRIVATE DATA DEFINITIONS */
static messageHandler_data_S messageHandler_data;

extern const messageHandler_config_S messageHandler_config;

/* PRIVATE FUNCTION DEFINITIONS */



/* PUBLIC FUNCTION DEFINITIONS */
void messageHandler_init(void)
{
    memset(&messageHandler_data, 0U, sizeof(messageHandler_data));
    for (messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT; channel++)
    {
        // Init message
        memcpy(&messageHandler_data.RXChannelData[channel].message, &messageHandler_config.RXMessageConfig[channel].initValue, sizeof(messageHandler_config.RXMessageConfig[channel].initValue));
    }
}

void messageHandler_run1ms(void)
{
    // RX
    for(messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT; channel++)
    {
        messageHandler_RXChannelData_S * const channelData = &messageHandler_data.RXChannelData[channel];

        if(channelData->newDataAvailable)
        {
            if(messageHandler_config.messageReceivedCallback != NULL)
            {
                messageHandler_config.messageReceivedCallback(channel, &channelData->message);
            }

            channelData->newDataAvailable = false;
        }
    }

    // TX
    for(messageHandler_TXMessageChannel_E channel = (messageHandler_TXMessageChannel_E)0U; channel < MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT; channel++)
    {
        messageHandler_TXChannelData_S * const channelData = &messageHandler_data.TXChannelData[channel];
        const messageHandler_TXMessageConfig_S * const channelConfig = &messageHandler_config.TXMessageConfig[channel];
        if(channelData->dispatchRequested)
        {
            if(messageHandler_config.messagePopulateCallback != NULL)
            {
                // Fill message
                protocol_message_S messageToSend;
                messageHandler_config.messagePopulateCallback(channel, &(messageToSend.message));
                messageToSend.messageID = channelConfig->messageID;

                // Add message to the UART queue
                UART_writeLen((const uint8_t * const)&messageToSend, channelConfig->messageLength + sizeof(messageToSend.messageID));
            }

            channelData->dispatchRequested = false;
        }
    }
}

bool messageHandler_getMessage(const messageHandler_RXMessageChannel_E channel, protocol_allMessages_U * const message)
{
    bool ret = false;

    if((channel < MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT) && (message != NULL))
    {
        *message = messageHandler_data.RXChannelData[channel].message;
    }

    return ret;
}

void messageHandler_dispatchMessage(const messageHandler_TXMessageChannel_E channel)
{
    if(channel < MESSAGE_HANDLER_TX_MESSAGE_CHANNEL_COUNT)
    {
        messageHandler_data.TXChannelData[channel].dispatchRequested = true;
    }
}

void messageHandler_messageReceivedCallback(protocol_message_S const * const receiveData)
{
    if(receiveData != NULL)
    {
        for(messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT; channel++)
        {
            if(receiveData->messageID == messageHandler_config.RXMessageConfig[channel].messageID)
            {
                messageHandler_data.RXChannelData[channel].timeReceived = RTOS_getTimeMilliseconds();
                memcpy(&messageHandler_data.RXChannelData[channel].message, &receiveData->message, sizeof(protocol_allMessages_U));
                messageHandler_data.RXChannelData[channel].newDataAvailable = true;

                break;
            }
        }
    }
}
