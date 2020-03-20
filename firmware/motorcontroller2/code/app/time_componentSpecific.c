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
    .TIMPeriph = TIM9,
    .timerResolution = TIME_TIMER_RESOLUTION_32BITS,
    .timerInterruptNumber = TIM1_BRK_TIM9_IRQn,
    .enablePeripheralsClockCallback = time_componentSpecific_enablePeripheralsClockCallback,
};

static void time_componentSpecific_enablePeripheralsClockCallback(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    time_interruptHandler();
}