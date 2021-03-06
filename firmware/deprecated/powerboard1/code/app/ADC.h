/*
 * ADC.h
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define ADC_TO_PIN_VOLTAGE (3.3/4095)

typedef enum
{
    ADC_CHANNEL_WATER_SENSOR,
    ADC_CHANNEL_EXTERNAL_PRESSURE,
    ADC_CHANNEL_LEFT_BATT_VOLTAGE,
    ADC_CHANNEL_RIGHT_BATT_VOLTAGE,
    ADC_CHANNEL_RIGHT_BATT_CURRENT,
    ADC_CHANNEL_LEFT_BATT_CURRENT,
    
    ADC_CHANNEL_COUNT,
} ADC_channel_E;

void ADC_init(void);
uint16_t ADC_getChannelData(const ADC_channel_E channel);

#endif /* ADC_H_ */
