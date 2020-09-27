/*
 * powerManagement.c
 *
 *  Created on: Feb 18, 2020
 *      Original Author: Poorna Chander Kalidas
 *      Modified by: Shuhang Tan
 */

#include "powerManagement.h"

#include <stdlib.h>
#include "stm32f4xx.h"
#include "messageHandler.h"
#include "protocol.h"
#include "ADC.h"
#include "RTOS.h"
#include "housingMonitor.h"


#define _VBATT_ENABLE_GPIO		(GPIO_Pin_12)
#define _5V_ENABLE_GPIO		    (GPIO_Pin_13)
#define _12V_ENABLE_GPIO		(GPIO_Pin_14)
#define _16V_ENABLE_GPIO		(GPIO_Pin_15)
#define Pulse_GPIO		        (GPIO_Pin_2)

#define ADC_VALUE_TO_CURRENT(x)	((121000U * (x)) / 4095U) //121000mA of current when pin is at 3.3V

// Delay time to turn the power on after the board is on
#define DELAY_BEFORE_POWER_ON_MS   (1000U)

typedef enum
{
    POWER_MANAGEMENT_STATE_INIT,
    POWER_MANAGEMENT_STATE_RUN,
} powerManagement_state_E;

typedef struct
{
    uint32_t currents[POWER_MANAGEMENT_CHANNEL_COUNT];
    powerManagement_state_E state;
    uint32_t initTimestamp;
} powerManagement_data_S;

static powerManagement_data_S powerManagement_data;
static void powerManagement_setState(const powerManagement_channel_E channel, const bool newState);

void powerManagement_init(void)
{
	// GPIO Initialization
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = _VBATT_ENABLE_GPIO | _5V_ENABLE_GPIO | _12V_ENABLE_GPIO | _16V_ENABLE_GPIO;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//PD2 is used for giving a pulse to the register
	GPIO_InitStruct.GPIO_Pin = Pulse_GPIO;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//Keep PD2 output low after initialization
	GPIOD->ODR &= ~(Pulse_GPIO);

	powerManagement_data.state = POWER_MANAGEMENT_STATE_INIT;
	powerManagement_data.initTimestamp = RTOS_getTimeMilliseconds();
}

void powerManagement_run100ms(void)
{
    switch(powerManagement_data.state)
    {
        case POWER_MANAGEMENT_STATE_INIT:
        {
            // Waiting for 1s to pass
        	if(RTOS_getTimeElapsedMilliseconds(powerManagement_data.initTimestamp) > DELAY_BEFORE_POWER_ON_MS)
            {
                powerManagement_data.state = POWER_MANAGEMENT_STATE_RUN;
            }
            break;
        }
        case POWER_MANAGEMENT_STATE_RUN:
        {
        	// PD2 pulse: set PD2 to low at the beginning of run state
        	GPIOD->ODR &= ~(Pulse_GPIO);

			//Ckeck if leakage happens in the main housing before turning on power
			if (housingMonitor_getHousingStatus() == HOUSINGMONITOR_HOUSINGSTATUS_LEAK)
			{
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_VBATT, false);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, false);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V, false);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_16V, false);
			}
			//No leakage. Turn on the power.
			else 
			{
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_VBATT, true);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, true);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V, true);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_16V, true);
			}

            // PD2 pulse: transition from low to high to write to the register
            GPIOD->ODR |= Pulse_GPIO;

            break;
        }
        default:
        {
            break;
        }
    }
}

// Function that turns on and off the power channels
static void powerManagement_setState(const powerManagement_channel_E channel, const bool newState)
{
    if(channel < POWER_MANAGEMENT_CHANNEL_COUNT)
    {
        uint16_t GPIOPin = 0;

        switch(channel)
        {
            case POWER_MANAGEMENT_CHANNEL_VBATT:
            {
                GPIOPin = _VBATT_ENABLE_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_5V:
            {
                GPIOPin = _5V_ENABLE_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_12V:
            {
                GPIOPin = _12V_ENABLE_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_16V:
            {
                GPIOPin = _16V_ENABLE_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_COUNT:
            default:
            {
                break;
            }
        }

        if(GPIOPin != 0)
        {
            if(newState == true)
            {
                GPIOC->ODR |= GPIOPin;
            }
            else
            {
                GPIOC->ODR &= ~(GPIOPin);
            }
        }
    }
}

//Waiting for implementing I2C code to measure the current
//Makes no sense before that don't use it
uint32_t powerManagement_getCurrent_mA(const powerManagement_channel_E channel)
{
    uint32_t ret = 0U;

    if(channel < POWER_MANAGEMENT_CHANNEL_COUNT)
    {
        ret = powerManagement_data.currents[channel];
    }

    return ret;
}
