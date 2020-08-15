/*
 * LED.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Poornachander
 */

#include "LED.h"

#include "stm32f4xx.h"
#include "RCCHelper.h"
#include <stdbool.h>
#include "utils.h"
#include "assert.h"
#include "RTOS.h"

/* DEFINES */
#define NORMAL_CHANGE_PERIOD_MS	(500U)
#define ALERT_CHANGE_PERIOD_MS	(100U)

/* TYPEDEFS */

typedef struct
{
	uint32_t lastChangeTimestamp;
	LED_state_E currentState;
	LED_state_E newState;
} LED_channelData_S;

typedef struct
{
	LED_channelData_S channelData[LED_CHANNEL_COUNT];
} LED_data_S;

/* PRIVATE DATA */
extern const LED_config_S LED_config;
static LED_data_S LED_data;

/* PRIVATE FUNCTION DECLARATION */

/* PRIVATE FUNCTION DEFINITION */

/* PUBLIC FUNCTIONS */
void LED_init(void)
{
	for(LED_channel_E channel = (LED_channel_E)0U; channel < LED_CHANNEL_COUNT; channel++)
	{
		const LED_channelConfig_S * const channelConfig = &LED_config.channelConfig[channel];
		
		// Check that the config was done correctly
		assert(IS_GPIO_ALL_PERIPH(channelConfig->GPIOPort));
		assert(IS_GPIO_PIN_SOURCE(channelConfig->pin));

		// Turn on clock
		RCCHelper_clockCmd(channelConfig->GPIOPort, ENABLE);

		GPIO_InitTypeDef initTypeDef;
		GPIO_StructInit(&initTypeDef);

		initTypeDef.GPIO_Mode = GPIO_Mode_OUT;
		initTypeDef.GPIO_OType = GPIO_OType_PP;
		initTypeDef.GPIO_Speed = GPIO_Low_Speed;
		initTypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;

		// Set the GPIO pin to configure
		initTypeDef.GPIO_Pin = BITVALUE(channelConfig->pin);

		// Write configs to GPIO registers
		GPIO_Init(channelConfig->GPIOPort, &initTypeDef);

		// Set initial LED state
		LED_data.channelData[channel].newState = channelConfig->initState;
	}
}

void LED_run100ms(void)
{
	for(LED_channel_E channel = (LED_channel_E)0U; channel < LED_CHANNEL_COUNT; channel++)
	{
		LED_channelData_S * const channelData = &LED_data.channelData[channel];
		const LED_channelConfig_S * const channelConfig = &LED_config.channelConfig[channel];

		channelData->currentState = channelData->newState;

		switch (channelData->currentState)
		{
			case LED_STATE_ON:
			{
				channelConfig->GPIOPort->BSRRL = (uint16_t)BITVALUE(channelConfig->pin);
				break;
			}
			case LED_STATE_BLINK_NORMAL:
			{
				if(RTOS_getTimeElapsedMilliseconds(channelData->lastChangeTimestamp) >= NORMAL_CHANGE_PERIOD_MS)
				{
					LED_toggleLED(channel);
					channelData->lastChangeTimestamp = RTOS_getTimeMilliseconds();
				}
				break;
			}
			case LED_STATE_BLINK_ALERT:
			{
				if(RTOS_getTimeElapsedMilliseconds(channelData->lastChangeTimestamp) >= ALERT_CHANGE_PERIOD_MS)
				{
					LED_toggleLED(channel);
					channelData->lastChangeTimestamp = RTOS_getTimeMilliseconds();
				}
				break;
			}

			case LED_STATE_OFF:
			default:
			{
				channelConfig->GPIOPort->BSRRH = (uint16_t)BITVALUE(channelConfig->pin);
				break;
			}
		}
	}
}

void LED_setState(const LED_channel_E channel, const LED_state_E newState)
{
	if(channel < LED_CHANNEL_COUNT)
	{
		LED_data.channelData[channel].newState = newState;
	}
}

// Avoid using this function. Only use for debugging
void LED_toggleLED(const LED_channel_E channel)
{
	if(channel < LED_CHANNEL_COUNT)
	{
		LED_config.channelConfig[channel].GPIOPort->ODR ^= BITVALUE(LED_config.channelConfig[channel].pin);
	}
}
