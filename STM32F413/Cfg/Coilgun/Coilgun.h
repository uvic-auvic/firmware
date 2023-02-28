#ifndef COILGUN_H_
#define COILGUN_H_

// RTOS Kernel Includes
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

// Application Includes
#include <Application/LED.h>
#include <Application/Sensors/TempHumid.h>

/* RTOS Main Timer period is 1 second */
#define mainSOFTWARE_TIMER_PERIOD_MS		( 1000 / portTICK_RATE_MS ) /* 1 sec */


void MainLoop(void);
void vTaskInit(void); // An idea would be for this guy to accept tasks from application code and add them to the list.

void HardwareInit(void);
void vQueueInit(void);
void vSemiphoreInit(void);
void vMutexInit(void);


#endif /*COILGUN_H_*/