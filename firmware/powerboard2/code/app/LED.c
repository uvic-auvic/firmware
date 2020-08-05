/*
 * LED.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Poornachander
 */

#include "stm32f4xx.h"

#include <stdbool.h>

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
			GPIOD->ODR ^= GPIO_Pin_2;
			break;
		}
		case LED_CHANNEL_BLUE:
		{
			GPIOC->ODR ^= GPIO_Pin_15;
			break;
		}
		case LED_CHANNEL_GREEN:
		{
			GPIOC->ODR ^= GPIO_Pin_14;
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
				GPIOD->BSRRL = GPIO_Pin_2;
			}
			else
			{
				GPIOD->BSRRH = GPIO_Pin_2;
			}

			break;
		}
		case LED_CHANNEL_BLUE:
		{
			if(state)
			{
				GPIOC->BSRRL = GPIO_Pin_15;
			}
			else
			{
				GPIOC->BSRRH = GPIO_Pin_15;
			}

			break;
		}
		case LED_CHANNEL_GREEN:
		{
			if(state)
			{
				GPIOC->BSRRL = GPIO_Pin_14;
			}
			else
			{
				GPIOC->BSRRH = GPIO_Pin_14;
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
	RCC->AHB1ENR |=  RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	LED_private_setState(LED_CHANNEL_BLUE, true);
}

void LED_run100ms(void)
{
	LED_private_toggleLED(LED_CHANNEL_RED);
}
