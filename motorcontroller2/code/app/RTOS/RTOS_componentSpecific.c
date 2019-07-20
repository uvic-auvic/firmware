/*
 * RTOS_componentSpecific.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

#include "LED.h"

const RTOS_taskRunFunctions_S RTOS_taskRunFunctions1ms[RTOS_TASK_1MS_COUNT] =
{

};

const RTOS_taskRunFunctions_S RTOS_taskRunFunctions10ms[RTOS_TASK_10MS_COUNT] =
{

};

const RTOS_taskRunFunctions_S RTOS_taskRunFunctions100ms[RTOS_TASK_100MS_COUNT] =
{
		[RTOS_TASK_100MS_LED] = { .initFunction = NULL, .runFunction = LED_run14 }
};

const RTOS_taskRunFunctions_S RTOS_taskRunFunctions1000ms[RTOS_TASK_1000MS_COUNT] =
{
		[RTOS_TASK_1000MS_LED] = { .initFunction = LED_init, .runFunction = LED_run15 }
};

const RTOS_taskConfig_S RTOS_taskConfig[RTOS_TASK_COUNT] =
{
		[RTOS_TASK_1MS] = { .taskRunFunctions = RTOS_taskRunFunctions1ms, .countOfTaskRunFunctions = RTOS_TASK_1MS_COUNT},
		[RTOS_TASK_10MS] = { .taskRunFunctions = RTOS_taskRunFunctions10ms, .countOfTaskRunFunctions = RTOS_TASK_10MS_COUNT},
		[RTOS_TASK_100MS] = { .taskRunFunctions = RTOS_taskRunFunctions100ms, .countOfTaskRunFunctions = RTOS_TASK_100MS_COUNT},
		[RTOS_TASK_1000MS] = { .taskRunFunctions = RTOS_taskRunFunctions1000ms, .countOfTaskRunFunctions = RTOS_TASK_1000MS_COUNT}
};

