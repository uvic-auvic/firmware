/*
 * LED.c
 *
 *  Created on: Feb  7, 2020
 *      Author: Poornachander
 */

/* INCLUDES */
#include "stm32f0xx.h"
#include "RTOS.h"
#include <stdbool.h>

/* DEFINES */
#define LED_TOGGLE_PEROD_MS 			(500U)
#define LED_STAY_ON_AFTER_INIT_PERIOD	(10000U)

/* TYPEDEFS */
typedef enum
{
	LED_CHANNEL_BLUE,
	LED_CHANNEL_GREEN,

	LED_CHANNEL_COUNT
} LED_channel_E;

typedef struct
{
	uint32_t initTimestamp;
	uint32_t lastToggleTimestamp;
} LED_data_S;

/* PRIVATE DATA DECLARAION */
static LED_data_S LED_data;

/* PRIVATE FUNCTION DECLARATION */
static void LED_private_setState(const LED_channel_E channel, const bool state);
//static void LED_private_toggle(const LED_channel_E channel);

/* PRIVATE FUNCTION DEFINITION */
static void LED_private_setState(const LED_channel_E channel, const bool state)
{
	switch(channel)
	{
		case LED_CHANNEL_BLUE:
		{
			if(state == true)
			{
				GPIOC->BSRR = GPIO_BSRR_BS_9;
			}
			else
			{
				GPIOC->BSRR = GPIO_BSRR_BR_9;
			}

			break;
		}

		case LED_CHANNEL_GREEN:
		{
			if(state == true)
			{
				GPIOC->BSRR = GPIO_BSRR_BS_8;
			}
			else
			{
				GPIOC->BSRR = GPIO_BSRR_BR_8;
			}
			
			break;
		}

		case LED_CHANNEL_COUNT:
		default:
		{
			break;
		}
	}
}

//static void LED_private_toggle(const LED_channel_E channel)
//{
//	switch(channel)
//	{
//		case LED_CHANNEL_BLUE:
//		{
//			GPIOC->ODR ^= GPIO_Pin_9;
//			break;
//		}
//
//		case LED_CHANNEL_GREEN:
//		{
//			GPIOC->ODR ^= GPIO_Pin_8;
//			break;
//		}
//
//		case LED_CHANNEL_COUNT:
//		default:
//		{
//			break;
//		}
//	}
//}

/* PUBLIC FUNCTIONS */
void LED_init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;

	LED_private_setState(LED_CHANNEL_BLUE, true);
	LED_private_setState(LED_CHANNEL_GREEN, true);

	LED_data.initTimestamp = RTOS_getTimeMilliseconds();
	LED_data.lastToggleTimestamp = RTOS_getTimeMilliseconds();
}

void LED_run100ms(void)
{
	for(LED_channel_E channel = (LED_channel_E)0U; channel < LED_CHANNEL_COUNT; channel++)
	{
		switch(channel)
		{
			case LED_CHANNEL_BLUE:
			{
				if(RTOS_getTimeElapsedMilliseconds(LED_data.lastToggleTimestamp) > LED_TOGGLE_PEROD_MS)
				{
//					LED_private_toggle(LED_CHANNEL_BLUE);
					LED_data.lastToggleTimestamp = RTOS_getTimeMilliseconds();
				}
				break;
			}

			case LED_CHANNEL_GREEN:
			{
				if(RTOS_getTimeElapsedMilliseconds(LED_data.initTimestamp) > LED_STAY_ON_AFTER_INIT_PERIOD)
				{
//					LED_private_setState(LED_CHANNEL_GREEN, false);
				}
				break;
			}

			case LED_CHANNEL_COUNT:
			default:
			{

				break;
			}
		}
	}
}
