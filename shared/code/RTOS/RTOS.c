/*
 * RTOS.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define RTOS_TASK_1000MS_PRIORITY	(tskIDLE_PRIORITY + 1U)
#define RTOS_TASK_100MS_PRIORITY	(RTOS_TASK_1000MS_PRIORITY + 1U)
#define RTOS_TASK_10MS_PRIORITY     (RTOS_TASK_100MS_PRIORITY + 1U)
#define RTOS_TASK_1MS_PRIORITY		(RTOS_TASK_10MS_PRIORITY + 1U)


static void RTOS_runTask(RTOS_task_E RTOSTask);
static void RTOS_run1ms(void);
static void RTOS_run10ms(void);
static void RTOS_run100ms(void);
static void RTOS_run1000ms(void);

// Configs
extern const RTOS_taskConfig_S RTOS_taskConfig[RTOS_TASK_COUNT];

extern void RTOS_init(void);
void RTOS_init(void)
{
	(void)xTaskCreate((TaskFunction_t)RTOS_run1ms,       /* Function that implements the task. */
					  "1msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_1MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */


	(void)xTaskCreate((TaskFunction_t)RTOS_run10ms,       /* Function that implements the task. */
					  "10msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_10MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */


	(void)xTaskCreate((TaskFunction_t)RTOS_run100ms,       /* Function that implements the task. */
					  "100msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_100MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */


	(void)xTaskCreate((TaskFunction_t)RTOS_run1000ms,       /* Function that implements the task. */
					  "1000msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_1000MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */

	for(RTOS_task_E RTOSTask = (RTOS_task_E)0U; RTOSTask < RTOS_TASK_COUNT; RTOSTask++)
	{
		if(RTOS_taskConfig[RTOSTask].taskRunFunctions != NULL)
		{
			for(uint8_t task = 0U; task < RTOS_taskConfig[RTOSTask].countOfTaskRunFunctions; task++)
			{
				if(RTOS_taskConfig[RTOSTask].taskRunFunctions[task].initFunction != NULL)
				{
					RTOS_taskConfig[RTOSTask].taskRunFunctions[task].initFunction();
				}
			}
		}
	}

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}

static void RTOS_runTask(RTOS_task_E RTOSTask)
{
	if(RTOSTask < RTOS_TASK_COUNT)
	{
		for(RTOS_task100ms_E task = (RTOS_task100ms_E)0U; task < RTOS_taskConfig[RTOSTask].countOfTaskRunFunctions; task++)
		{
			if((RTOS_taskConfig[RTOSTask].taskRunFunctions != NULL) && (RTOS_taskConfig[RTOSTask].taskRunFunctions[task].runFunction != NULL))
			{
				RTOS_taskConfig[RTOSTask].taskRunFunctions[task].runFunction();
			}
		}
	}
}


static void RTOS_run1ms(void)
{
	while(1)
	{
		RTOS_runTask(RTOS_TASK_1MS);
		vTaskDelay(pdMS_TO_TICKS(1U));
	}
}

static void RTOS_run10ms(void)
{
	while(1)
	{
		RTOS_runTask(RTOS_TASK_10MS);
		vTaskDelay(pdMS_TO_TICKS(10U));
	}
}

static void RTOS_run100ms(void)
{
	while(1)
	{
		RTOS_runTask(RTOS_TASK_100MS);
		vTaskDelay(pdMS_TO_TICKS(100U));
	}
}

static void RTOS_run1000ms(void)
{
	while(1)
	{
		RTOS_runTask(RTOS_TASK_1000MS);
		vTaskDelay(pdMS_TO_TICKS(1000U));
	}
}


void vApplicationTickHook(void)
{

}

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
