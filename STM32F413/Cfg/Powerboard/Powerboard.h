/*
 * Powerboard.h
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 */

#ifndef POWERBOARD_H_
#define POWERBOARD_H_

// RTOS Kernel Includes
#include <FreeRTOS.h>
#include <task.h>

// Application Includes
#include "Application/LED.h"
//#include "Application/Sensors/SI7021.h"
//#include "Application/Sensors/INA226.h"

//#include "System/I2C.h"


/* RTOS Main Timer period is 1 second */
#define mainSOFTWARE_TIMER_PERIOD_MS		( 1000 / portTICK_RATE_MS ) /* 1 sec */


void MainLoop(void);

void HardwareInit(void);

#endif /* POWERBOARD_H_ */
