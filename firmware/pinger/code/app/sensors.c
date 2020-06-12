/*
 * sensors.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "sensors.h"
#include "ADC.h"

// psi = (2500/819)*ADC_VALUE + 220  for 3.3V supply to sensor
// psi = (2500/1241)*ADC_VALUE + 220  for 5V supply to sensor
#define ADC_VALUE_TO_PRESSURE(x)	(((float)2500/1241)*(x) + 220) //10^(-2) psi, centi-psi

typedef struct
{
    uint16_t extPressure; //10^(-2) psi, centi-psi
} sensors_data_S;

static sensors_data_S sensors_data;

void sensors_init(void)
{

}

void sensors_run10ms(void)
{
    sensors_data.extPressure =  ADC_VALUE_TO_PRESSURE(ADC_getChannelData(ADC_CHANNEL_EXTERNAL_PRESSURE));
}

uint16_t sensors_getExternalPressure(void)
{
    return sensors_data.extPressure;
}
