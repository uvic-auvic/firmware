/*
 * I2C_componentSpecific.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Shuhang Tan
 */

#include "I2C.h"
#include "I2C_componentSpecific.h"
#include "stm32f4xx.h"

// I2C config information (To be updated)
static const I2C_HWConfig_S I2C_HWConfig =
{
		// SDA Pin Info
		.SDAPort   = GPIOF,
		.SDAPin    = GPIO_Pin_0,

		// SCL Pin Info
		.SCLPort   = GPIOF,
		.SCLPin    = GPIO_Pin_1,

		// I2C Peripheral
		.I2CPeriph = I2C2,
		.I2CAFNum  = GPIO_AF_I2C2,
		.I2CEVIRQ  = I2C2_EV_IRQn,
		.I2CERIRQ  = I2C2_ER_IRQn,
};

extern const I2C_config_S I2C_config;
const I2C_config_S I2C_config =
{
	.HWConfig = &I2C_HWConfig
};

//To be updated
static const uint8_t I2C_Address[I2C_CHANNEL_COUNT] = {
	[I2C_CHANNEL_VBATT] = 0b1001101,
	[I2C_CHANNEL_5V]    = 0x2,
	[I2C_CHANNEL_12V]   = 0x3,
	[I2C_CHANNEL_16V]   = 0x4,
};

uint8_t I2C_channelToAddressMapping(I2C_channel_E channel){
	return I2C_Address[channel];
}
