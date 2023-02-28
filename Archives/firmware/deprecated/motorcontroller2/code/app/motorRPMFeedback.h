/*
 * motorRPMFeedback.h
 *
 *  Created on: Feb 21, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef MOTORRPMFEEDBACK_H_
#define MOTORRPMFEEDBACK_H_

#include "motorDriver.h"

void motorRPMFeedback_init(void);
void motorRPMFeedback_run10ms(void);
int16_t motorRPMFeedback_getMotorRPM(const motorDriver_channel_E channel);


#endif /* MOTORRPMFEEDBACK_H_ */
