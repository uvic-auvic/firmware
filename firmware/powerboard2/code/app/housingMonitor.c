/*
 * housingMonitor.c
 *
 *  Created on: Sep 7, 2020
 *      Author: Shuhang Tan
 */

#include "housingMonitor.h"
#include "ADC.h"


#define THERSHOLD (2048)

typedef struct
{
    housingMonitor_housingStatus_E housingStatus;
} housingMonitor_data_S;

static housingMonitor_data_S housingMonitor_data;

void housingMonitor_init(void)
{
	// Initialization of variable: no leakage.
	housingMonitor_data.housingStatus = HOUSINGMONITOR_HOUSINGSTATUS_NO_LEAK;
}


housingMonitor_housingStatus_E housingMonitor_getHousingStatus(void)
{
	return housingMonitor_data.housingStatus;
}

// Actual detection of leakage
void housingMonitor_run(void)
{
	// Obtain water sensor data
	uint16_t sensorValue = 0;
	sensorValue = ADC_getChannelData(ADC_CHANNEL_ADC_WATER_SENSE);

	if (sensorValue >= THERSHOLD)
	{
		housingMonitor_data.housingStatus = HOUSINGMONITOR_HOUSINGSTATUS_LEAK;
	}
}
