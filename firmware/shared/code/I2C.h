/*
 * I2C.h
 *
 *  Created on: Oct 5, 2020
 *      Author: Shuhang Tan
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "I2C_componentSpecific.h"

//initialize I2C pins and peripheral for component
void I2C_init(void);

//to run in one of the RTOS tasks - sends messages from send buffer - reads messages on receive buffer
void I2C_run_test(void);

//places data on send buffer - returns true/false based on success/fail
bool I2C_send(I2C_channel_E channel, const uint8_t * const data, const uint8_t length);

//receives I2C message and places on receive buffer
bool I2C_receive(I2C_channel_E channel, const uint8_t * const data, const uint8_t length);

bool is_idle();

#endif /* I2C_H_ */
