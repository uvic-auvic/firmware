/*
 * RTOS.h
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#ifndef MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_H_
#define MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_H_

#include "RTOS_componentSpecific.h"
#include "FreeRTOS.h"
#include "task.h"

#define RTOS_TASK_1000MS_PRIORITY	(tskIDLE_PRIORITY + 1U)
#define RTOS_TASK_100MS_PRIORITY	(RTOS_TASK_1000MS_PRIORITY + 1U)
#define RTOS_TASK_10MS_PRIORITY     (RTOS_TASK_100MS_PRIORITY + 1U)
#define RTOS_TASK_1MS_PRIORITY		(RTOS_TASK_10MS_PRIORITY + 1U)

typedef enum
{
	RTOS_TASK_1MS,
	RTOS_TASK_10MS,
	RTOS_TASK_100MS,
	RTOS_TASK_1000MS,

	RTOS_TASK_COUNT
} RTOS_task_E;

/* PUBLIC FUNCTIONS */
uint32_t RTOS_getTimeMilliseconds(void);
uint32_t RTOS_getTimeElapsedMilliseconds(const uint32_t timeToCompare);

#endif /* MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_H_ */
