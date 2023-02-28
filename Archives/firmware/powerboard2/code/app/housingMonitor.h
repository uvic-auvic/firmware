/*
 * housingMonitor.h
 *
 *  Created on: Sep 7, 2020
 *      Author: Shuhang Tan
 */

#ifndef HOUSINGMONITOR_H_
#define HOUSINGMONITOR_H_

typedef enum
{
	HOUSINGMONITOR_HOUSINGSTATUS_NO_LEAK = 0,
	HOUSINGMONITOR_HOUSINGSTATUS_LEAK = 1,
} housingMonitor_housingStatus_E;

void housingMonitor_init(void);
housingMonitor_housingStatus_E housingMonitor_getHousingStatus(void);
void housingMonitor_run(void);

#endif /* HOUSINGMONITOR_H_ */
