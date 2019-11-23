/*
 * UART_componentSpecific.c
 *
 *  Created on: Jul 21, 2019
 *      Author: Poornachander
 */

#include <stdbool.h>
#include <string.h>

#include "UARTDma.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Bootloader.h"

/* COMMAND DEFINES */
// RID Command
#define CMD_RID_STR		"RID"
#define CMD_RID_LENGTH	3

// START command
#define CMD_START_STR		"START"
#define CMD_START_LENGTH	5

// Packet Type
#define DATA_TYPE_ASCII		'A'
#define DATA_TYPE_FIRST		'F'
#define DATA_TYPE_MID		'M'
#define DATA_TYPE_LAST		'L'

/*
 * Checks that the packet ID is in order
 * Checks that the CRC is correct
 * Returns: 1 = Actual CRC matches expected CRC
 * 			0 = Actual CRC does not match expected CRC
 */
static uint8_t check_payload_CRC() {

    return 0;
}

void UART_receiveCallback(uint8_t const * const payload, uint8_t const payloadSize)
{
    {
        //Check if command is in ASCII
        if( *(char *)(payload + DATA_TYPE_OFFSET) == DATA_TYPE_ASCII)
        {

            // RID Command
            if(strncmp(CMD_RID_STR, (char *)(payload + DATA_OFFSET), CMD_RID_LENGTH) == 0)
            {
                UART_push_out(DEVICEID);
                UART_push_out("\r\n");

            }

            // START command
            else if(strncmp(CMD_START_STR, (char *)(payload + DATA_OFFSET), CMD_START_LENGTH) == 0)
            {
                char charToSend = 'S';
                UART_push_out("ACK\r\n");
                xQueueSend(Bootloader_Queue_Handle, &charToSend, portMAX_DELAY);
            }

            //No matches
            else
            {
#pragma message "TO DO:Return error through UART"
                //No matches for ASCII commands
            }

        }

        //Non-ASCII command
        else
        {

            if(check_payload_CRC() == 0 && !OVERRIDE_CRC_CHECK) {
#pragma message "TO DO:Return error through UART"
                //ERROR: Error in payload
            } else
            {

                // First program packet
                if(*(char *)(payload + DATA_TYPE_OFFSET) == DATA_TYPE_FIRST)
                {
                    uint8_t deviceIDSize = payloadSize - PACKET_ID_SIZE_BYTES - DATA_TYPE_SIZE_BYTES
                            - PROGRAM_SIZE_SIZE_BYTES - FIRMWARE_VERSION_SIZE_BYTES - CRC_SIZE_BYTES;

                    firmwareInfo_ptr->programSize = *(uint16_t *)(payload + PROGRAM_SIZE_OFFSET);
                    memcpy(firmwareInfo_ptr->firmwareVersion, payload + FIRMWARE_VERSION_OFFSET, FIRMWARE_VERSION_SIZE_BYTES);
                    memcpy(firmwareInfo_ptr->deviceID, payload + DEVICE_ID_OFFSET, deviceIDSize);

                    //Notify Bootloader Task
                    xQueueSend(Bootloader_Queue_Handle, "F", portMAX_DELAY);

                    //Send success through UART
                    UART_push_out("ACK\r\n");

                }

                // Middle programs packets
                else if(*(char *)(payload + DATA_TYPE_OFFSET) == DATA_TYPE_MID)
                {

                    if(xSemaphoreTake(flashBuffer_ptr->mutex, portMAX_DELAY))
                    {

                        flashBuffer_ptr->startingAddress += flashBuffer_ptr->length; //Set starting address
                        flashBuffer_ptr->length = payloadSize - NOT_DATA_SIZE_BYTES; //Set length of data
                        memcpy(flashBuffer_ptr->data, payload + DATA_OFFSET, flashBuffer_ptr->length); //Copy data into buffer

                        xSemaphoreGive(flashBuffer_ptr->mutex);
                        xQueueSend(Bootloader_Queue_Handle, "M", portMAX_DELAY); //Notify bootloader task

                        //Send success through UART
                        UART_push_out("ACK\r\n");

                    } else
                    {
                        //ERROR: Mutex error
                    }


                }

                //Last program packet
                else if(*(char *)(payload + DATA_TYPE_OFFSET) == DATA_TYPE_LAST)
                {
                    //Send success through UART
                    UART_push_out("ACK\r\n");
                }

                //No Matches
                else
                {
#pragma message "TO DO:Return error through UART"
                }

            }

        }

    }

}
