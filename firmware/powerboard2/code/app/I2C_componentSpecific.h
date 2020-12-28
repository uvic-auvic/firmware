/*
 * I2C_componentSpecific.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Shuhang Tan
 */

#ifndef I2C_COMPONENTSPECIFIC_H_
#define I2C_COMPONENTSPECIFIC_H_

#include <stdint.h>
#include <stdlib.h>

//To be updated
typedef enum
{
    I2C_CHANNEL_VBATT,
	I2C_CHANNEL_5V,
	I2C_CHANNEL_12V,
	I2C_CHANNEL_16V,

	I2C_CHANNEL_COUNT,
} I2C_channel_E;

uint8_t I2C_channelToAddressMapping(I2C_channel_E channel);

#endif /* I2C_COMPONENTSPECIFIC_H_ */
