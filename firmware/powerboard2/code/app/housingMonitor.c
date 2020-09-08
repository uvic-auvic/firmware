/*
 * housingMonitor.c
 *
 *  Created on: Sep 7, 2020
 *      Author: Shuhang Tan
 */

#include "housingMonitor.h"
#include "ADC.h"


#define threshold (2048)
housingMonitor_housingStatus_E HStatus;

housingMonitor_housingStatus_E housingMonitor_getHousingStatus(void)
{
	return HStatus;
}

// Actual detection of leakage
void housingMonitor_run(void)
{
	HStatus = HOUSINGMONITOR_HOUSINGSTATUS_NO_LEAK;

	// Obtain water sensor data
	uint16_t sensorValue = 0;
	sensorValue = ADC_getChannelData(ADC_CHANNEL_ADC_WATER_SENSE);

	if (sensorValue >= threshold)
	{
		HStatus = HOUSINGMONITOR_HOUSINGSTATUS_LEAK;
	}
}
