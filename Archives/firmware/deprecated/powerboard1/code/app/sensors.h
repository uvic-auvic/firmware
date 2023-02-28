/*
 * sensors.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdlib.h>
#include <stdint.h>

void sensors_init(void);
void sensors_run10ms(void);
uint16_t sensors_getExternalPressure(void);

#endif /* SENSORS_H_ */
