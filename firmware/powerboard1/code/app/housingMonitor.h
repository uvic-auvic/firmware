/*
 * housingMonitor.h
 *
 *  Created on: Feb 14, 2020
 *      Author: me
 */

#ifndef POWERBOARD1_CODE_APP_HOUSINGMONITOR_H_
#define POWERBOARD1_CODE_APP_HOUSINGMONITOR_H_

typedef enum
{
	No_leak = 0,
	Leak = 1
} housingMonitor_housingStatus_E;


typedef enum
{
	Main = 0
} housingMonitor_housing_E;

//void housingMonitor_init(void);
housingMonitor_housingStatus_E housingMonitor_getHousingStatus(const housingMonitor_housing_E housing);
void housingMonitor_run(void);

#endif /* POWERBOARD1_CODE_APP_HOUSINGMONITOR_H_ */


