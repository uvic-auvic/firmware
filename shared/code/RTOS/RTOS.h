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

typedef void (*RTOS_task)(void);

typedef enum
{
	RTOS_TASK_1MS,
	RTOS_TASK_10MS,
	RTOS_TASK_100MS,
	RTOS_TASK_1000MS,

	RTOS_TASK_COUNT
} RTOS_task_E;

typedef struct
{
	RTOS_task initFunction;
	RTOS_task runFunction;
} RTOS_taskRunFunctions_S;

typedef struct
{
	const RTOS_taskRunFunctions_S * taskRunFunctions;
	const uint8_t					countOfTaskRunFunctions;
} RTOS_taskConfig_S;

typedef struct
{
	RTOS_taskConfig_S	taskConfig[RTOS_TASK_COUNT];
} RTOS_config_S;

#endif /* MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_H_ */
