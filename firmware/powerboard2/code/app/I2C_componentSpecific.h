/*
 * I2C_componentSpecific.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Shuhang Tan
 */

#ifndef I2C_COMPONENTSPECIFIC_H_
#define I2C_COMPONENTSPECIFIC_H_

#include <stdint.h>

//To be updated
typedef enum
{
    I2C_CHANNEL_VBATT,
	I2C_CHANNEL_5V,
	I2C_CHANNEL_12V,
	I2C_CHANNEL_16V,

	I2C_CHANNEL_COUNT,
} I2C_channel_E;

//To be updated
static const uint8_t I2C_channelToAddressMapping[I2C_CHANNEL_COUNT]{
	[I2C_CHANNEL_VBATT] = 0x1,
	[I2C_CHANNEL_5V]    = 0x2,
	[I2C_CHANNEL_12V]   = 0x3,
	[I2C_CHANNEL_16V]   = 0x4,
};

#endif /* I2C_COMPONENTSPECIFIC_H_ */
