/*
 * housingMonitor.c
 *
 *  Created on: Feb 14, 2020
 *      Author: Shuhang Tan
*/

#include "housingMonitor.h"
#include "ADC.h"



uint16_t threshold = 2048;


housingMonitor_housingStatus_E housingMonitor_getHousingStatus(const housingMonitor_housing_E housing)
{
	uint16_t SensorValue = 0;
	if (housing == Main)
	{
		SensorValue = ADC_getChannelData(ADC_CHANNEL_WATER_SENSOR);
	}
	housingMonitor_housingStatus_E HStatus = No_leak;
	if (SensorValue >= threshold)
	{
		HStatus = Leak;
	}
	return HStatus;
}

void housingMonitor_run(void)
{
	housingMonitor_housing_E housing = Main;
	if (housingMonitor_getHousingStatus(housing) == Leak)
	{
		//Do something here.
		//Testing: using GPIO, if leaking happened Pin PA2 output high, else is low
		//GPIOA->ODR |= GPIO_PIN_2;

	}
	//else GPIOA->ODR &= ~GPIO_PIN_2;
}
