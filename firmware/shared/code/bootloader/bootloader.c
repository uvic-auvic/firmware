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
#include "flashInterface.h"

/* DEFINES */
#define WAIT_FOR_START_SIGNAL_TIME_MS   (2000U)

/* TYPEDEFS */
typedef enum
{
    BOOTLOADER_STATE_WAIT_FOR_START_SIGNAL,
    BOOTLOADER_STATE_VERIFY_FIRMWARE_INFO,
    BOOTLOADER_STATE_JUMP_TO_APPLICATION,
} bootloader_state_E;

typedef struct
{
    bootloader_state_E state;
    uint32_t initTimestamp;
} bootloader_data_S;

/* PRIVATE VARIABLE DECLARATION */
extern const bootloader_config_S bootloader_config;
static bootloader_data_S bootloader_data;

/* PRIVATE FUNCTION DEFINITION */
void bootloader_private_deinitPeripherals(void);

/* PRIVATE FUNCTION DECLARATION */
void bootloader_private_deinitPeripherals(void)
{
    if(bootloader_config.deinitPeripheralsCallback != NULL)
    {
        bootloader_config.deinitPeripheralsCallback();
    }
}

/* PUBLIC FUNCTION DECLATRATION */
void bootloader_private_jumpToApplication(void)
{
    /* Branch to Application */
    typedef void (*pFunction)(void);
    pFunction appEntry;
    uint32_t appStack;

    /* Get the application stack pointer (First entry in the application vector table) */
    appStack = (uint32_t) *((__IO uint32_t*)APP_START_ADDRESS);

    /* Get the application entry point (Second entry in the application vector table) */
    appEntry = (pFunction) *(__IO uint32_t*) (APP_START_ADDRESS + 4);

    __disable_irq();

    // Disable IRQs
    for(uint8_t i = 0;i < 8;i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
    }
    // Clear pending IRQs
    for(uint8_t i = 0;i < 8;i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    __enable_irq();

    SysTick->CTRL =0;
    SysTick->LOAD=0;
    SysTick->VAL=0;

    /* Reconfigure vector table offset register to match the application location */
    SCB->VTOR = APP_START_ADDRESS;

    /* Set the application stack pointer */
    __set_MSP(appStack);

    /* Start the application */
    appEntry();
}

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
        case BOOTLOADER_STATE_VERIFY_FIRMWARE_INFO:
        {

            flashInterface_unlockFlash();
            break;
        }
        case BOOTLOADER_STATE_JUMP_TO_APPLICATION:
        {
            /* Deinit peripherals */
            bootloader_private_deinitPeripherals();

            /* Lock flash */
            flashInterface_lockFlash();


            break;
        }
    }
}
