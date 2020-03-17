/*
 * time.c
 *
 *  Created on: Mar 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "time.h"
#include "assert.h"
#include "utils.h"

#define TARGET_FREQUENCY    (1000000.0f)
#define TIMER_PRESCALER     ((uint32_t)((SystemCoreClock / TARGET_FREQUENCY) - 1U))

extern const time_config_S time_config;

void time_init(void)
{
    assert(time_config.TIMPeriph != NULL);

    if(time_config.initializationRequired)
    {
        if(time_config.enablePeripheralsClockCallback != NULL)
        {
            time_config.enablePeripheralsClockCallback();
        }

        TIM_TimeBaseInitTypeDef TIMInit;
        TIM_TimeBaseStructInit(&TIMInit);

        TIMInit.TIM_Prescaler = TIMER_PRESCALER;
        TIMInit.TIM_CounterMode = TIM_CounterMode_Up;
        TIMInit.TIM_Period = 0xFFFFFFFFU;
        TIMInit.TIM_ClockDivision = TIM_CKD_DIV1;

        TIM_TimeBaseInit(time_config.TIMPeriph, &TIMInit);

        TIM_Cmd(time_config.TIMPeriph, ENABLE);

    }
}

uint32_t time_getTimeMilliseconds(void)
{
    return TIM_GetCounter(time_config.TIMPeriph) / US_IN_MS;
}

uint32_t time_getTimeElapsedMilliseconds(const uint32_t timeToCompare_ms)
{
    uint32_t ret = 0U;
    const uint32_t currentTime_ms = time_getTimeMilliseconds();
    if(currentTime_ms >= timeToCompare_ms)
    {
        ret = currentTime_ms - timeToCompare_ms;
    }
    else
    {
        ret = 0x100000000UL - (timeToCompare_ms - currentTime_ms);
    }
    
    return ret;
}

uint32_t time_getTimeMicroseconds(void)
{
    return TIM_GetCounter(time_config.TIMPeriph);
}

uint32_t time_getTimeElapsedMicroseconds(const uint32_t timeToCompare_us)
{
    uint32_t ret = 0U;
    const uint32_t currentTime_us = time_getTimeMicroseconds();
    if(currentTime_us >= timeToCompare_us)
    {
        ret = currentTime_us - timeToCompare_us;
    }
    else
    {
        ret = 0x100000000UL - (timeToCompare_us - currentTime_us);
    }
    
    return ret;
}


