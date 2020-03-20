/*
 * time.h
 *
 *  Created on: Mar 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef TIME_H_
#define TIME_H_

#include "time_componentSpecific.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

typedef enum
{
    TIME_TIMER_RESOLUTION_16BITS,
    TIME_TIMER_RESOLUTION_32BITS,
} time_timerResolution_E;

typedef struct
{
    TIM_TypeDef * TIMPeriph; // Must be a 32-bit timer
    time_timerResolution_E timerResolution;
    uint32_t timerInterruptNumber;
    void 		  (* enablePeripheralsClockCallback)(void);
} time_config_S;

void time_init(void);
uint32_t time_getTimeMilliseconds(void);
uint32_t time_getTimeElapsedMilliseconds(const uint32_t timeToCompare_ms);
uint32_t time_getTimeMicroseconds(void);
uint32_t time_getTimeElapsedMicroseconds(const uint32_t timeToCompare_us);

// Interrupt Handler. Call this function in the interrupt handler in component specific
void time_interruptHandler(void);

#endif /* TIME_H_ */
