/*
 * LED.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "stm32f4xx.h"

#include <stdbool.h>

typedef enum
{
	LED_CHANNEL_RED,
	LED_CHANNEL_BLUE,

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
			GPIOA->ODR ^= GPIO_Pin_4;
			break;
		}
		case LED_CHANNEL_BLUE:
		{
			GPIOA->ODR ^= GPIO_Pin_5;
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
				GPIOA->BSRRL = GPIO_Pin_4;
			}
			else
			{
				GPIOA->BSRRH = GPIO_Pin_4;
			}

			break;
		}
		case LED_CHANNEL_BLUE:
		{
			if(state)
			{
				GPIOA->BSRRL = GPIO_Pin_5;
			}
			else
			{
				GPIOA->BSRRH = GPIO_Pin_5;
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
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIO_InitTypeDef GPIO_InitStructure;

	//Configure pin PA4(red LED) and pin PA5(blue LED) as output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	LED_private_setState(LED_CHANNEL_BLUE, true);
}

void LED_run100ms(void)
{
	LED_private_toggleLED(LED_CHANNEL_RED);
}
