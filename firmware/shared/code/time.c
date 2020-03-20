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

typedef struct
{
    uint64_t timerOverflow;
} time_data_S;

extern const time_config_S time_config;

static time_data_S time_data;

uint64_t timer_private_getTimerOverflowNumber(void)
{
    uint64_t ret;

    switch(time_config.timerResolution)
    {
        case TIME_TIMER_RESOLUTION_16BITS:
        {
            ret = 0x10000U;
            break;
        }
        case TIME_TIMER_RESOLUTION_32BITS:
        default:
        {
            ret = 0x100000000UL;
            break;
        }
    }

    return ret;
}

void time_init(void)
{
    assert(time_config.TIMPeriph != NULL);

    if(time_config.enablePeripheralsClockCallback != NULL)
    {
        time_config.enablePeripheralsClockCallback();
    }

    TIM_TimeBaseInitTypeDef TIMInit;
    TIM_TimeBaseStructInit(&TIMInit);

    TIMInit.TIM_Prescaler = TIMER_PRESCALER;
    TIMInit.TIM_CounterMode = TIM_CounterMode_Up;
    TIMInit.TIM_Period = timer_private_getTimerOverflowNumber() - 1;
    TIMInit.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(time_config.TIMPeriph, &TIMInit);

	NVIC_SetPriority(time_config.timerInterruptNumber, 7U);
	NVIC_EnableIRQ(time_config.timerInterruptNumber);

    TIM_ITConfig(time_config.TIMPeriph, TIM_IT_Update, ENABLE);
    TIM_Cmd(time_config.TIMPeriph, ENABLE);
}

uint32_t time_getTimeMilliseconds(void)
{   
    return (time_data.timerOverflow + TIM_GetCounter(time_config.TIMPeriph)) / US_IN_MS;
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
        ret = timer_private_getTimerOverflowNumber() - (timeToCompare_ms - currentTime_ms);
    }
    
    return ret;
}

uint32_t time_getTimeMicroseconds(void)
{
    return time_data.timerOverflow + TIM_GetCounter(time_config.TIMPeriph);
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
        ret = timer_private_getTimerOverflowNumber() - (timeToCompare_us - currentTime_us);
    }
    
    return ret;
}

void time_interruptHandler(void)
{
    time_data.timerOverflow += timer_private_getTimerOverflowNumber();
    TIM_ClearFlag(time_config.TIMPeriph, TIM_FLAG_Update);
    TIM_ClearITPendingBit(time_config.TIMPeriph, TIM_IT_Update);
}
