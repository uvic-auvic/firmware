/*
 * LED_componentSpecific.c
 *
 *  Created on: Aug 14, 2020
 *      Author: Poornachander
 */

#include "LED.h"

extern const LED_config_S LED_config;
const LED_config_S LED_config =
{
    .channelConfig =
    {
        [LED_CHANNEL_RED] =
        {
            .pin = 14,
            .GPIOPort = GPIOB,
            .initState = LED_STATE_ON,
        },
        [LED_CHANNEL_BLUE] =
        {
            .pin = 7,
            .GPIOPort = GPIOB,
            .initState = LED_STATE_ON,
        },
        [LED_CHANNEL_GREEN] =
        {
            .pin = 0,
            .GPIOPort = GPIOB,
            .initState = LED_STATE_BLINK_NORMAL,
        },
    },
};
