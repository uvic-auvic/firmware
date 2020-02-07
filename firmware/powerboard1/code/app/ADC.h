/*
 * ADC.h
 *
 *  Created on: Jul 20, 2019
 *      Author: Poornachander
 */

#ifndef POWERBOARD1_CODE_APP_ADC_H_
#define POWERBOARD1_CODE_APP_ADC_H_

#include <stdint.h>

typedef enum
{
    ADC_CHANNEL_WATER_SENSOR,
    ADC_CHANNEL_PRESSURE_SENSOR,
    ADC_CHANNEL_LEFT_BATT_VOLTAGE,
    ADC_CHANNEL_RIGHT_BATT_VOLTAGE,
    ADC_CHANNEL_RIGHT_BATT_CURRENT,
    ADC_CHANNEL_LEFT_BATT_CURRENT,
    
    ADC_CHANNEL_COUNT,
} ADC_channel_E;

void ADC_init(void);
uint16_t ADC_getChannelData(const ADC_channel_E channel);

#endif /* POWERBOARD1_CODE_APP_ADC_H_ */
