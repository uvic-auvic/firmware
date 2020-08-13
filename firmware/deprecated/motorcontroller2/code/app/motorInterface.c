/*
 * motorInterface.c
 *
 *  Created on: Feb 22, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "motorInterface.h"

#include <stdlib.h>
#include "motorDriver.h"
#include "messageHandler.h"
#include "RTOS.h"

#define MESSAGE_MIA_THRESHOLD_MS    (2000U)

void motorInterface_init(void)
{

}

void motorInterface_run10ms(void)
{
    protocol_allMessages_U motorSpeedMessage;
    uint32_t timeReceived = 0U;
    messageHandler_getMessage(MESSAGE_HANDLER_RX_CHANNEL_MOTOR_SPEED, &motorSpeedMessage, &timeReceived);

    const uint32_t timeSinceLastUpdate = RTOS_getTimeElapsedMilliseconds(timeReceived);

    if((timeReceived != 0U) && (timeSinceLastUpdate < MESSAGE_MIA_THRESHOLD_MS))
    {
        for(motorDriver_channel_E channel = (motorDriver_channel_E)0U; channel < MOTOR_DRIVER_CHANNEL_COUNT; channel++)
        {
            const uint16_t motorPowerLevelPercent = (abs(motorSpeedMessage.POLARIS_motorSetSpeed.motorSpeed[channel]) * MOTOR_DRIVER_PERCENT_BASE) / 127U;
            motorDriver_direction_E direction = MOTOR_DRIVER_DIRECTION_FORWARD;

            if(motorSpeedMessage.POLARIS_motorSetSpeed.motorSpeed[channel] < 0)
            {
                direction = MOTOR_DRIVER_DIRECTION_BACKWARD;
            }

            motorDriver_setPowerLevelPercent(channel, motorPowerLevelPercent, direction);
        }
    }
    else
    {
        motorDriver_stopAllMotors();
    }
}


