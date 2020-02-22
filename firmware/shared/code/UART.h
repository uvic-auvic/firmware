/*
 * UART.h
 */

#ifndef UART_H_
#define UART_H_

#include "UART_componentSpecific.h"

#if (DEVICE_FAMILY_STM32F4)
#include "stm32f4xx.h"
#elif (DEVICE_FAMILY_STM32F0)
#include "stm32f0xx.h"
#endif

#include <stdbool.h>
#include <stdint.h>

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
	uint32_t 		baudRate;

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

#endif /* UART_H_ */
