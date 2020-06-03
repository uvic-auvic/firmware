/*
 * powerManagement.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "powerManagement.h"

#include <stdlib.h>
#include "stm32f0xx.h"
#include "messageHandler.h"
#include "protocol.h"
#include "ADC.h"
#include "RTOS.h"
#include "housingMonitor.h"


#define SYSTEM_POWER_GPIO		(GPIO_Pin_11)
#define MOTOR_POWER_GPIO		(GPIO_Pin_12)
#define _12V_9V_GPIO			(GPIO_Pin_1)
#define _5V_GPIO				(GPIO_Pin_2)

#define ADC_VALUE_TO_BAT_VOLTAGE(x)	((((10090U * 33) / 10U) * (x)) / 4095U) //mV
#define ADC_VALUE_TO_CURRENT(x)	((121000U * (x)) / 4095U) //121000mA of current when pin is at 3.3V

#define DELAY_BEFORE_POWER_ON_MS   (2000U)

typedef enum
{
    POWER_MANAGEMENT_STATE_INIT,
    POWER_MANAGEMENT_STATE_RUN,
} powerManagement_state_E;

typedef struct
{
    uint32_t currents[POWER_MANAGEMENT_CHANNEL_COUNT];
    uint32_t batteryCurrents[POWER_MANAGEMENT_BATTERY_CHANNEL_COUNT];
    uint32_t batteryVoltages[POWER_MANAGEMENT_BATTERY_CHANNEL_COUNT];
    powerManagement_state_E state;
    uint32_t initTimestamp;
} powerManagement_data_S;

static powerManagement_data_S powerManagement_data;

void powerManagement_init(void)
{
	// GPIO Initialization
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = (SYSTEM_POWER_GPIO | MOTOR_POWER_GPIO);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = (_12V_9V_GPIO | _5V_GPIO);
	GPIO_Init(GPIOC, &GPIO_InitStruct);

    powerManagement_data.initTimestamp = RTOS_getTimeMilliseconds();
}

void powerManagement_run100ms(void)
{
    switch(powerManagement_data.state)
    {
        case POWER_MANAGEMENT_STATE_INIT:
        {
            if(RTOS_getTimeElapsedMilliseconds(powerManagement_data.initTimestamp) > DELAY_BEFORE_POWER_ON_MS)
            {
                // Turn on system power by default
                powerManagement_setState(POWER_MANAGEMENT_CHANNEL_SYSTEM, true);

                powerManagement_data.state = POWER_MANAGEMENT_STATE_RUN;
            }
            break;
        }
        case POWER_MANAGEMENT_STATE_RUN:
        {
            protocol_allMessages_U message;
            messageHandler_getMessage(MESSAGE_HANDLER_RX_MESSAGE_CHANNEL_POWER_ENABLE, &message, NULL);

            powerManagement_data.batteryVoltages[POWER_MANAGEMENT_BATTERY_CHANNEL_LEFT] = ADC_VALUE_TO_BAT_VOLTAGE(ADC_getChannelData(ADC_CHANNEL_LEFT_BATT_VOLTAGE));
            powerManagement_data.batteryVoltages[POWER_MANAGEMENT_BATTERY_CHANNEL_RIGHT] = ADC_VALUE_TO_BAT_VOLTAGE(ADC_getChannelData(ADC_CHANNEL_RIGHT_BATT_VOLTAGE));

            powerManagement_data.batteryCurrents[POWER_MANAGEMENT_BATTERY_CHANNEL_LEFT] = ADC_VALUE_TO_CURRENT(ADC_getChannelData(ADC_CHANNEL_LEFT_BATT_CURRENT));
            powerManagement_data.batteryCurrents[POWER_MANAGEMENT_BATTERY_CHANNEL_RIGHT] = ADC_VALUE_TO_CURRENT(ADC_getChannelData(ADC_CHANNEL_RIGHT_BATT_CURRENT));

            //powerManagement_setState(POWER_MANAGEMENT_CHANNEL_MOTOR, message.POLARIS_powerEnable.motorPowerEnable);
            //powerManagement_setState(POWER_MANAGEMENT_CHANNEL_5V, message.POLARIS_powerEnable._5VPowerEnable);
            //powerManagement_setState(POWER_MANAGEMENT_CHANNEL_12V_9V, message.POLARIS_powerEnable._12V9VPowerEnable);
			
			//Ckeck if leakage happens in the main housing before turning on power
			housingMonitor_housing_E Main_housing = Main;
			if (housingMonitor_getHousingStatus(Main_housing) == Leak)
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
			}

            break;
        }
        default:
        {
            break;
        }
    }
}

void powerManagement_setState(const powerManagement_channel_E channel, const bool newState)
{
    if(channel < POWER_MANAGEMENT_CHANNEL_COUNT)
    {
        GPIO_TypeDef * GPIOPort = NULL;
        uint16_t GPIOPin;

        switch(channel)
        {
            case POWER_MANAGEMENT_CHANNEL_MOTOR:
            {
                GPIOPort = GPIOA;
                GPIOPin = MOTOR_POWER_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_SYSTEM:
            {
                GPIOPort = GPIOA;
                GPIOPin = SYSTEM_POWER_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_5V:
            {
                GPIOPort = GPIOC;
                GPIOPin = _5V_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_12V_9V:
            {
                GPIOPort = GPIOC;
                GPIOPin = _12V_9V_GPIO;
                break;
            }
            case POWER_MANAGEMENT_CHANNEL_COUNT:
            default:
            {
                break;
            }
        }

        if(GPIOPort != NULL)
        {
            if(newState == true)
            {
                GPIOPort->ODR |= GPIOPin;
            }
            else
            {
                GPIOPort->ODR &= ~(GPIOPin);
            }
        }
    }
}

uint32_t powerManagement_getCurrent_mA(const powerManagement_channel_E channel)
{
    uint32_t ret = 0U;

    if(channel < POWER_MANAGEMENT_CHANNEL_COUNT)
    {
        ret = powerManagement_data.currents[channel];
    }

    return ret;
}

uint32_t powerManagement_getBatteryCurrent_mA(const powerManagement_batteryChannel_E channel)
{
    uint32_t ret = 0U;

    if(channel < POWER_MANAGEMENT_BATTERY_CHANNEL_COUNT)
    {
        ret = powerManagement_data.batteryCurrents[channel];
    }

    return ret;
}

uint16_t powerManagement_getBatteryVoltage_mV(const powerManagement_batteryChannel_E channel)
{
    uint16_t ret = 0U;

    if(channel < POWER_MANAGEMENT_BATTERY_CHANNEL_COUNT)
    {
        ret = powerManagement_data.batteryVoltages[channel];
    }

    return ret;
}
