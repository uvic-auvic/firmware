/*
 * bootloader.c
 *
 *  Created on: Mar 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "bootloader.h"
#include <stdlib.h>
#include <stdint.h>
#include "time.h"

#define WAIT_FOR_START_SIGNAL_TIME_MS   (2000U)

typedef enum
{
    BOOTLOADER_STATE_WAIT_FOR_START_SIGNAL,
    BOOTLOADER_STATE_JUMP_TO_APPLICATION,
} bootloader_state_E;

typedef struct
{
    bootloader_state_E state;
    uint32_t initTimestamp;
} bootloader_data_S;

static bootloader_data_S bootloader_data;

void bootloader_init(void)
{
    bootloader_data.initTimestamp = time_getTimeMilliseconds();
}

void bootloader_run(void)
{
    switch(bootloader_data.state)
    {
        default:
        case BOOTLOADER_STATE_WAIT_FOR_START_SIGNAL:
        {
            if(time_getTimeElapsedMilliseconds(bootloader_data.initTimestamp) > WAIT_FOR_START_SIGNAL_TIME_MS)
            {
                bootloader_data.state = BOOTLOADER_STATE_JUMP_TO_APPLICATION;
            }
            break;
        }
        case BOOTLOADER_STATE_JUMP_TO_APPLICATION:
        {

            break;
        }
    }
}
