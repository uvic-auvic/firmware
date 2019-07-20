/*
 * RTOS_componentSpecific.h
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#ifndef MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_COMPONENTSPECIFIC_H_
#define MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_COMPONENTSPECIFIC_H_

typedef enum
{
	RTOS_TASK_1000MS_LED,

	RTOS_TASK_1000MS_COUNT
} RTOS_task1000ms_E;

typedef enum
{
	RTOS_TASK_100MS_LED,

	RTOS_TASK_100MS_COUNT
} RTOS_task100ms_E;

typedef enum
{
	RTOS_TASK_10MS_COUNT
} RTOS_task10ms_E;

typedef enum
{
	RTOS_TASK_1MS_COUNT
} RTOS_task1ms_E;

#endif /* MOTORCONTROLLER2_CODE_APP_RTOS_RTOS_COMPONENTSPECIFIC_H_ */
