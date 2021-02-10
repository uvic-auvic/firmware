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

// Initialize I2C pins and peripheral for component
void I2C_init(void);

// A part of initializtion - set up I2C peripharal
void I2C_setup(void);

// Place data on send buffer - returns true/false based on success/fail
bool I2C_send(I2C_channel_E channel, const uint8_t * const data, const uint8_t length);

// Receive I2C message and places on receive buffer
bool I2C_receive(I2C_channel_E channel, const uint8_t * const data, const uint8_t length);

// Function to check if I2C is available
bool is_idle();

#endif /* I2C_H_ */
