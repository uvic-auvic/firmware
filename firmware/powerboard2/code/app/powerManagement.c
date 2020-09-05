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
//#include "housingMonitor.h"


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

void powerManagement_init(void)
{
	// GPIO Initialization
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = _VBATT_ENABLE_GPIO | _5V_ENABLE_GPIO | _12V_ENABLE_GPIO | _16V_ENABLE_GPIO;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//PD2 is used for giving a pulse to the register
	GPIO_InitStruct.GPIO_Pin = Pulse_GPIO;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//Keep PD2 output low after initialization
	GPIOD->ODR &= ~(Pulse_GPIO);

    // Timer2 Initialization
    // Timer2 is used for generating pulses on PD2, the frequency is 8MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM2_TimeBaseInitStruct;
	TIM2_TimeBaseInitStruct.TIM_Prescaler         = 12;
	TIM2_TimeBaseInitStruct.TIM_Period            = 0;
	TIM2_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM2_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM2_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseInitStruct);
	TIM_Cmd(TIM2, ENABLE);

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
                // Turn on all the power
                powerManagement_setState(POWER_MANAGEMENT_CHANNEL_VBATT, true);
                powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, true);
                powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V, true);
                powerManagement_setState(POWER_MANAGEMENT_CHANNEL_16V, true);

                powerManagement_data.state = POWER_MANAGEMENT_STATE_RUN;
            }
            break;
        }
        case POWER_MANAGEMENT_STATE_RUN:
        {
            protocol_allMessages_U message;
            // Obtain Trident's instruction about power
            messageHandler_getMessage(MESSAGE_HANDLER_RX_CHANNEL_POWER_ENABLE, &message, NULL);

            // Control the power according to Trident's orders
            powerManagement_setState(POWER_MANAGEMENT_CHANNEL_VBATT, message.TRIDENT_powerEnable.VBattPowerEnable);
            powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, message.TRIDENT_powerEnable._5VPowerEnable);
            powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V, message.TRIDENT_powerEnable._12VPowerEnable);
            powerManagement_setState(POWER_MANAGEMENT_CHANNEL_16V, message.TRIDENT_powerEnable._16VPowerEnable);
			
			/*Implement this after housingMonitor code is done
			//Ckeck if leakage happens in the main housing before turning on power
			housingMonitor_housing_E Main_housing = HOUSINGMONITOR_HOUSING_MAIN;
			if (housingMonitor_getHousingStatus(Main_housing) == HOUSINGMONITOR_HOUSINGSTATUS_LEAK)
			{
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_MOTOR, false);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, false);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V_9V, false);
			}
			//No leakage. If power enable is true, turn on the power.
			else 
			{
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_MOTOR, message.POLARIS_powerEnable.motorPowerEnable);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, message.POLARIS_powerEnable._5VPowerEnable);
				powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V_9V, message.POLARIS_powerEnable._12V9VPowerEnable);
			}*/

            break;
        }
        default:
        {
            break;
        }
    }
}

// Function that turns on and off the power channels
void powerManagement_setState(const powerManagement_channel_E channel, const bool newState)
{
    if(channel < POWER_MANAGEMENT_CHANNEL_COUNT)
    {
        GPIO_TypeDef * GPIOPort = GPIOC;
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
                GPIOPort->ODR |= GPIOPin;
            }
            else
            {
                GPIOPort->ODR &= ~(GPIOPin);
            }
            // Generating a pulse on pin PD2 to change the values in the register
            GPIOD->ODR |= Pulse_GPIO;
            GPIOD->ODR &= ~(Pulse_GPIO);
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
