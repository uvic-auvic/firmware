/*
 * time_componentSpecific.c
 *
 *  Created on: Mar 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "time.h"

static void time_componentSpecific_enablePeripheralsClockCallback(void);

const time_config_S time_config =
{
    .TIMPeriph = TIM5,
    .timerResolution = TIME_TIMER_RESOLUTION_32BITS,
    .timerInterruptNumber = TIM5_IRQn,
    .enablePeripheralsClockCallback = time_componentSpecific_enablePeripheralsClockCallback,
};

static void time_componentSpecific_enablePeripheralsClockCallback(void)
{
    RCC->APB1ENR |= RCC_APB1Periph_TIM5;
}

void TIM5_IRQHandler(void)
{
    time_interruptHandler();
}