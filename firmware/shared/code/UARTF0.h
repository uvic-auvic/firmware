/*
 * UART.h
 */

#ifndef UART_H_
#define UART_H_

#include "UARTF0_componentSpecific.h"

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

	// DMA
	DMA_TypeDef *   	 	DMAController;
	DMA_Channel_TypeDef *   DMAChannelRX;
	DMA_Channel_TypeDef *   DMAChannelTX;
	IRQn_Type				DMARXInterruptNumber;
	IRQn_Type				DMATXInterruptNumber;

	//Common
	void 		(* enablePeripheralsClockCallback)(void);

} UART_HWConfig_S;

typedef struct
{
	uint32_t	taskPriority;
	void (* receiveCallback)(uint8_t const * const receiveData, const uint8_t receiveDataLength);
	const UART_HWConfig_S * const HWConfig;
} UART_config_S;


void UART_init();
bool UART_writeLen(const uint8_t * const data, const uint8_t length);

// Interrupt Handler
void UART_DMAInterruptHandler(void);

#endif /* UART_H_ */
