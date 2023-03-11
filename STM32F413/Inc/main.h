/*
 * main.h
 *
 *  Created on: Feb 17, 2023
 *      Author: amann
 */

#ifndef MAIN_H_
#define MAIN_H_

// Include only the device you want to configure
#define POWERBOARD
#ifdef POWERBOARD
#include "Powerboard/Powerboard.h"
#elif COILGUN
#include "Coilgun/Coilgun.h"
#endif

// Prototypes
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName );

//

#endif /* MAIN_H_ */
