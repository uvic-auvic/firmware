/*
 * housingMonitor.c
 *
 *  Created on: Feb 14, 2020
 *      Author: Shuhang Tan
*/

#include "housingMonitor.h"
#include "ADC.h"


#define threshold 2048
static housingMonitor_housingStatus_E HStatus;

housingMonitor_housingStatus_E housingMonitor_getHousingStatus(const housingMonitor_housing_E housing)
{
	housingMonitor_housingStatus_E HStatus_copy = HOUSINGMONITOR_HOUSINGSTATUS_NO_LEAK;
	if (housing == HOUSINGMONITOR_HOUSING_MAIN)
	{
		HStatus_copy = HStatus;
	}
	return HStatus_copy;
}

//Actual detection of leakage
void housingMonitor_run(const housingMonitor_housing_E housing)
{
	uint16_t sensorValue = 0;
	if (housing == HOUSINGMONITOR_HOUSING_MAIN)
	{
		sensorValue = ADC_getChannelData(ADC_CHANNEL_WATER_SENSOR);
	}
	HStatus = HOUSINGMONITOR_HOUSINGSTATUS_NO_LEAK;
	if (sensorValue >= threshold)
	{
		HStatus = HOUSINGMONITOR_HOUSINGSTATUS_LEAK;
	}
	/*//Testing code for leakage detection
	  housingMonitor_housing_E housing = HOUSINGMONITOR_HOUSING_MAIN;
	  if (housingMonitor_getHousingStatus(housing) == HOUSINGMONITOR_HOUSINGSTATUS_LEAK)
	{
		//Do something here.
		//Testing: using GPIO, if leaking happened Pin PA2 output high, else is low
		//GPIOA->ODR |= GPIO_PIN_2;

	}
	//else GPIOA->ODR &= ~GPIO_PIN_2;
	 */
}
