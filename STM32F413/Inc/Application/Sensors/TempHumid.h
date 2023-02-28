/*
 * TempHumid.h
 *
 *  Created on: Feb 23, 2023
 *      Author: amann
 */

#ifndef APPLICATION_TEMPHUMID_H_
#define APPLICATION_TEMPHUMID_H_

#include "System/ADC.h"

#define SENSIRION

#ifdef SENSIRON
// Enable This code
#elif MICROCHIP

#endif

// Generic Code

// Internal STM32F413 Temperature Sensor
// This gets the internal temperature of the IC
// Supported temperature range: –40 to 125 °C
//	 Precision: ±1.5 °C


// the temperature
//To use the sensor: See page 348

void InternalTemperatureSensor_Init();

#endif /* APPLICATION_TEMPHUMID_H_ */
