/*
 * debug.h
 * 
 *  Created on: Aug 6, 2020
 *      Author: Poornachander
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "debug_componentSpecific.h"

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef UART_DEBUG
#define debug_init(...) _debug_init(__VA_ARGS__)
#define debug_writeLen(...)	_debug_writeLen(__VA_ARGS__)
#define debug_writeString(...)	_debug_writeString(__VA_ARGS__)
#else
#define debug_init(...)
#define debug_writeLen(...)
#define debug_writeString(...)
#endif

typedef struct
{
	// GPIO
	uint8_t 		rxPin;
	uint8_t 		txPin;
	GPIO_TypeDef *	GPIOPort;
	uint8_t			AFNumber;

	// UART
	USART_TypeDef * UARTPeriph;
	IRQn_Type 		UARTInterruptNumber;

	//Common
	void 		(* enablePeripheralsClockCallback)(void);

} debug_HWConfig_S;

typedef struct
{
	const debug_HWConfig_S * const HWConfig;
} debug_config_S;


void _debug_init(void);
bool _debug_writeLen(const uint8_t * const data, const uint16_t length);
bool _debug_writeString(const char * const string);

// Interrupt Handler
void debug_UARTInterruptHandler(void);

#endif /* DEBUG_H_ */
