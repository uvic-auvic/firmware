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

/* DEFINES */

/* TYPEDEFS */

typedef struct
{
    bool newDataAvailable;
    protocol_allMessages_U message;
} messageHandler_RXChannelData_S;

typedef struct
{
    messageHandler_RXChannelData_S RXChannelData[MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT];
} messageHandler_data_S;

/* PRIVATE DATA DEFINITIONS */
static messageHandler_data_S messageHandler_data;

extern const messageHandler_config_S messageHandler_config;

void messageHandler_init(void)
{

}

void messageHandler_run10ms(void)
{

}

void messageHandler_messageReceivedCallback(protocol_message_S const * const receiveData)
{
    if(receiveData != NULL)
    {
        for(messageHandler_RXMessageChannel_E channel = (messageHandler_RXMessageChannel_E)0U; channel < MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_COUNT; channel++)
        {
            if(receiveData->messageID == messageHandler_config.RXMessageConfig[channel].messageID)
            {
                memcpy(&messageHandler_data.RXChannelData[channel].message, &receiveData->message, sizeof(protocol_allMessages_U));
                messageHandler_data.RXChannelData[channel].newDataAvailable = true;

                break;
            }
        }
    }
}
