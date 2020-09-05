/*
 * powerManagement.h
 *
 *  Created on: Aug 22, 2020
 *      Original Author: Poorna Chander Kalidas
 *      Modified by:Shuhang Tan
 */

#ifndef POWERMANAGEMENT_H_
#define POWERMANAGEMENT_H_

#include <stdint.h>
#include <stdbool.h>

//Four power enable pins: 12V, 5V, VBatt, and 16V
typedef enum
{
    POWER_MANAGEMENT_CHANNEL_VBATT,
    POWER_MANAGEMENT_CHANNEL_5V,
    POWER_MANAGEMENT_CHANNEL_12V,
    POWER_MANAGEMENT_CHANNEL_16V,

    POWER_MANAGEMENT_CHANNEL_COUNT,
} powerManagement_channel_E;

void powerManagement_init(void);
void powerManagement_run100ms(void);
void powerManagement_setState(const powerManagement_channel_E channel, const bool newState);
uint32_t powerManagement_getCurrent_mA(const powerManagement_channel_E channel);


#endif /* POWERMANAGEMENT_H_ */
