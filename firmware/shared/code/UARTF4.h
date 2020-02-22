/*
 * UART.h
 */

#ifndef SHARED_CODE_UART_H_
#define SHARED_CODE_UART_H_

#include "UARTF4_componentSpecific.h"

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
	DMA_Stream_TypeDef *    DMAStreamRX;
	DMA_Stream_TypeDef *    DMAStreamTX;
	uint32_t				DMAChannelRX; // a value of @ref DMA_channel. Example: DMA_Channel_0
	uint32_t				DMAChannelTX; // a value of @ref DMA_channel. Example: DMA_Channel_0
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

#endif /* SHARED_CODE_UART_H_ */
