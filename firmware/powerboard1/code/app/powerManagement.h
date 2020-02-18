/*
 * powerManagement.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef POWERBOARD1_CODE_APP_POWERMANAGEMENT_H_
#define POWERBOARD1_CODE_APP_POWERMANAGEMENT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    POWER_MANAGEMENT_CHANNEL_MOTOR,
    POWER_MANAGEMENT_CHANNEL_SYSTEM,
    POWER_MANAGEMENT_CHANNEL_5V,
    POWER_MANAGEMENT_CHANNEL_12V_9V,

    POWER_MANAGEMENT_CHANNEL_COUNT,
} powerManagement_channel_E;

typedef enum
{
    POWER_MANAGEMENT_BATTERY_CHANNEL_LEFT,
    POWER_MANAGEMENT_BATTERY_CHANNEL_RIGHT,

    POWER_MANAGEMENT_BATTERY_CHANNEL_COUNT,
} powerManagement_batteryChannel_E;

void powerManagement_init(void);
void powerManagement_run100ms(void);
void powerManagement_setState(const powerManagement_channel_E channel, const bool newState);
uint32_t powerManagement_getCurrent_mA(const powerManagement_channel_E channel);
uint32_t powerManagement_getBatteryCurrent_mA(const powerManagement_batteryChannel_E channel);
uint16_t powerManagement_getBatteryVoltage_mV(const powerManagement_batteryChannel_E channel);

#endif /* POWERBOARD1_CODE_APP_POWERMANAGEMENT_H_ */
