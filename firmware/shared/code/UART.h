/*
 * simple_UART.h
 *
 *  Created on: Aug 19, 2017
 *      Author: abates
 */

#ifndef SIMPLE_UART_H_
#define SIMPLE_UART_H_

#include "UART_componentSpecific.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
	// GPIO
	uint8_t rxPin;
	uint8_t txPin;
	GPIO_TypeDef *	GPIOPort;

	// UART
	USART_TypeDef * UARTPeriph;
	IRQn_Type UARTInterruptNumber;
	uint32_t baudRate;

	// DMA
	DMA_Stream_TypeDef * DMAStream;
	uint32_t DMAChannel;
	IRQn_Type DMAInterruptNumber;

	//Common
	void (* enablePeripheralsClockCallback)(void);

} UART_HWConfig_S;

typedef const struct
{
	void (* receiveCallback)(uint8_t const * const receiveData, const uint8_t receiveDataLength);
	const UART_HWConfig_S * const HWConfig;
} UART_config_S;


extern void UART_init();
extern bool UART_write(char const * const data);
extern bool UART_writeLen(uint8_t const * const data, const uint8_t dataLength);

// Interrupt
void UART_DMAInterruptHandler(void);

#endif /* SIMPLE_UART_H_ */
