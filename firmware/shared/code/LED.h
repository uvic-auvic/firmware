/*
 * LED.h
 *
 *  Created on: Aug 5, 2020
 *      Author: Poornachander
 */

#ifndef LED_H_
#define LED_H_

#include "LED_componentSpecific.h"

#include "stm32f4xx.h"

typedef enum
{
    LED_STATE_OFF,
    LED_STATE_ON,
    LED_STATE_BLINK_NORMAL,
    LED_STATE_BLINK_1000MS = LED_STATE_BLINK_NORMAL,
    LED_STATE_BLINK_ALERT,
    LED_STATE_BLINK_200MS = LED_STATE_BLINK_ALERT,
} LED_state_E;

typedef struct
{
    GPIO_TypeDef *  GPIOPort;
    uint8_t         pin;
    LED_state_E     initState;
} LED_channelConfig_S;
    

typedef struct
{
    LED_channelConfig_S channelConfig[LED_CHANNEL_COUNT];
} LED_config_S;

void LED_init(void);
void LED_run100ms(void);
void LED_setState(const LED_channel_E channel, const LED_state_E newState);
void LED_toggleLED(const LED_channel_E channel);

#endif /* LED_H_ */
