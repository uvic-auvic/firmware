/*
 * RTOS.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#include "RTOS.h"

/* INCLUDES */

/* DEFINES */

/* TYPEDEFS */
typedef struct
{
	uint32_t timeSinceInit;
} RTOS_data_S;

/* PRIVATE DATA DECLARATION */
static RTOS_data_S RTOS_data;

/* PRIVATE FUNCTION DECLARATIONS */
static void RTOS_private_task1ms(void);
static void RTOS_private_task10ms(void);
static void RTOS_private_task100ms(void);
static void RTOS_private_task1000ms(void);

/* PRIVATE FUNCTION DEFINITIONS */
static void RTOS_private_task1ms(void)
{
	TickType_t lastRunTime = xTaskGetTickCount();
	while(1)
	{
		RTOS_run1ms();
		vTaskDelayUntil(&lastRunTime, pdMS_TO_TICKS(1U));
		RTOS_data.timeSinceInit += 1U; // Improve this
	}
}

static void RTOS_private_task10ms(void)
{
	TickType_t lastRunTime = xTaskGetTickCount();
	while(1)
	{
		RTOS_run10ms();
		vTaskDelayUntil(&lastRunTime, pdMS_TO_TICKS(10U));
	}
}

static void RTOS_private_task100ms(void)
{
	TickType_t lastRunTime = xTaskGetTickCount();
	while(1)
	{
		RTOS_run100ms();
		vTaskDelayUntil(&lastRunTime, pdMS_TO_TICKS(100U));
	}
}

static void RTOS_private_task1000ms(void)
{
	TickType_t lastRunTime = xTaskGetTickCount();
	while(1)
	{
		RTOS_run1000ms();
		vTaskDelayUntil(&lastRunTime, pdMS_TO_TICKS(1000U));
	}
}

/* PUBLIC FUNCTIONS */
uint32_t RTOS_getTimeMilliseconds(void)
{
	return RTOS_data.timeSinceInit;
}

uint32_t RTOS_getTimeElapsedMilliseconds(const uint32_t timeToCompare)
{
	uint32_t ret = 0U;
	const uint32_t timeNow = RTOS_data.timeSinceInit;

	if(timeNow >= timeToCompare)
	{
		ret = timeNow - timeToCompare;
	}
	else
	{
		ret = ~(uint32_t)0U - (timeToCompare - timeNow);
	}

	return ret;
}

extern void RTOS_launch(void);
void RTOS_launch(void)
{
	(void)xTaskCreate((TaskFunction_t)RTOS_private_task1ms,       /* Function that implements the task. */
					  "1msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_1MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */

	(void)xTaskCreate((TaskFunction_t)RTOS_private_task10ms,       /* Function that implements the task. */
					  "10msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_10MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */

	(void)xTaskCreate((TaskFunction_t)RTOS_private_task100ms,       /* Function that implements the task. */
					  "100msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_100MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */

	(void)xTaskCreate((TaskFunction_t)RTOS_private_task1000ms,       /* Function that implements the task. */
					  "1000msTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  RTOS_TASK_1000MS_PRIORITY,/* Priority at which the task is created. */
					  NULL);      /* Used to pass out the created task's handle. */

	RTOS_init();

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}


/* FreeRTOS Hooks */
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
