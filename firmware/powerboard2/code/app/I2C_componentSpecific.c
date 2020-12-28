/*
 * I2C_componentSpecific.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Shuhang Tan
 */

#include "I2C_componentSpecific.h"
#include "stm32f4xx.h"

//To be updated
static const uint8_t I2C_Address[I2C_CHANNEL_COUNT] = {
	[I2C_CHANNEL_VBATT] = 0x11,
	[I2C_CHANNEL_5V]    = 0x2,
	[I2C_CHANNEL_12V]   = 0x3,
	[I2C_CHANNEL_16V]   = 0x4,
};

uint8_t I2C_channelToAddressMapping(I2C_channel_E channel){
	return I2C_Address[channel];
}
