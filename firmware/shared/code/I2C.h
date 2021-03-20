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
#include "stm32f4xx.h"
#include "protocol.h"
#include "I2C_componentSpecific.h"

typedef struct
{
	// SDA Pin Info
	GPIO_TypeDef*	SDAPort;
	uint8_t 		SDAPin;

	// SCL Pin Info
	GPIO_TypeDef*	SCLPort;
	uint8_t 		SCLPin;

	// I2C Peripheral
	I2C_TypeDef*    I2CPeriph;
	uint8_t         I2CAFNum;
	IRQn_Type       I2CEVIRQ;
	IRQn_Type       I2CERIRQ;

} I2C_HWConfig_S;

typedef struct
{
	const I2C_HWConfig_S * const HWConfig;
	void(*messageReceivedCallback)(const protocol_MID_E messageID, const protocol_allMessages_U * const message);
} I2C_config_S;

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
