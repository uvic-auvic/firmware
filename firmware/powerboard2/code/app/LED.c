/*
 * LED.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Poornachander
 */

#include "stm32f4xx.h"
#include "RCCHelper.h"
#include <stdbool.h>

/* DEFINES */
#define RED_LED_PORT	(GPIOB)
#define BLUE_LED_PORT	(GPIOB)
#define GREEN_LED_PORT	(GPIOB)
#define RED_LED_PIN		(GPIO_Pin_14)
#define BLUE_LED_PIN	(GPIO_Pin_7)
#define GREEN_LED_PIN	(GPIO_Pin_0)

/* TYPEDEFS */
typedef enum
{
	LED_CHANNEL_RED,
	LED_CHANNEL_BLUE,
	LED_CHANNEL_GREEN,

	LED_CHANEL_COUNT,
} LED_channel_E;

/* PRIVATE FUNCTION DECLARATION */
static void LED_private_setState(const LED_channel_E channel, const bool state);
static void LED_private_toggleLED(const LED_channel_E channel);

/* PRIVATE FUNCTION DEFINITION */
static void LED_private_toggleLED(const LED_channel_E channel)
{
	switch(channel)
	{
		case LED_CHANNEL_RED:
		{
			RED_LED_PORT->ODR ^= RED_LED_PIN;
			break;
		}
		case LED_CHANNEL_BLUE:
		{
			BLUE_LED_PORT->ODR ^= BLUE_LED_PIN;
			break;
		}
		case LED_CHANNEL_GREEN:
		{
			GREEN_LED_PORT->ODR ^= GREEN_LED_PIN;
			break;
		}
		case LED_CHANEL_COUNT:
		default:
		{
			break;
		}
	}
}

static void LED_private_setState(const LED_channel_E channel, const bool state)
{
	switch(channel)
	{
		case LED_CHANNEL_RED:
		{
			if(state)
			{
				RED_LED_PORT->BSRRL = RED_LED_PIN;
			}
			else
			{
				RED_LED_PORT->BSRRH = RED_LED_PIN;
			}

			break;
		}
		case LED_CHANNEL_BLUE:
		{
			if(state)
			{
				BLUE_LED_PORT->BSRRL = BLUE_LED_PIN;
			}
			else
			{
				BLUE_LED_PORT->BSRRH = BLUE_LED_PIN;
			}

			break;
		}
		case LED_CHANNEL_GREEN:
		{
			if(state)
			{
				GREEN_LED_PORT->BSRRL = GREEN_LED_PIN;
			}
			else
			{
				GREEN_LED_PORT->BSRRH = GREEN_LED_PIN;
			}

			break;
		}
		case LED_CHANEL_COUNT:
		default:
		{
			break;
		}
	}
}

void LED_init(void)
{
	// RCC->AHB1ENR |=  RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOBEN;

	RCCHelper_clockCmd(RED_LED_PORT, ENABLE);
	RCCHelper_clockCmd(BLUE_LED_PORT, ENABLE);
	RCCHelper_clockCmd(GREEN_LED_PORT, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = RED_LED_PIN;
	GPIO_Init(RED_LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GREEN_LED_PIN;
	GPIO_Init(BLUE_LED_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  BLUE_LED_PIN;
	GPIO_Init(GREEN_LED_PORT, &GPIO_InitStructure);

	LED_private_setState(LED_CHANNEL_BLUE, true);
}

void LED_run100ms(void)
{
	// LED_private_toggleLED(LED_CHANNEL_RED);
	// LED_private_toggleLED(LED_CHANNEL_BLUE);
	LED_private_toggleLED(LED_CHANNEL_GREEN);
}
