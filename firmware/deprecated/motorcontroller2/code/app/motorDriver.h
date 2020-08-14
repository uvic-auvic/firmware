/*
 * motorDriver.h
 *
 *  Created on: Feb 21, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <stdint.h>

#define MOTOR_DRIVER_PERCENT_BASE   (1000U)

typedef enum
{
    MOTOR_DRIVER_CHANNEL_0,
    MOTOR_DRIVER_CHANNEL_1,
    MOTOR_DRIVER_CHANNEL_2,
    MOTOR_DRIVER_CHANNEL_3,
    MOTOR_DRIVER_CHANNEL_4,
    MOTOR_DRIVER_CHANNEL_5,
    MOTOR_DRIVER_CHANNEL_6,
    MOTOR_DRIVER_CHANNEL_7,

    MOTOR_DRIVER_CHANNEL_COUNT,    
} motorDriver_channel_E;

typedef enum
{
    MOTOR_DRIVER_DIRECTION_FORWARD,
    MOTOR_DRIVER_DIRECTION_BACKWARD,
} motorDriver_direction_E;

void motorDriver_init(void);
void motorDriver_stopAllMotors(void);
void motorDriver_setPowerLevelPercent(const motorDriver_channel_E motor_x, uint16_t percent, const motorDriver_direction_E dir);

#endif /* MOTORDRIVER_H_ */
