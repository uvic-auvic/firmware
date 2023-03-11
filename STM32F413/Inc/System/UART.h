/*
 * UART.h
 *
 *  Created on: Feb 23, 2023
 *      Author: amann
 */

#ifndef SYSTEM_UART_H_
#define SYSTEM_UART_H_

#include <stm32f413xx.h>
#include <stdint.h>
#include "System/GPIO.h"

typedef enum{
	UART_MODE_TX = 0U,
	UART_MODE_RX,
	UART_MODE_COUNT
} UART_Mode;


typedef struct {
	USART_TypeDef* UARTx;
	UART_Mode OpMode;
	GPIO_Channel Channel;
} UART_Channel;


// we can reduce the pin and gpio address but its best for future designers to know what each pin does.
UART_Channel UART_Init(GPIO_Pin Pin, GPIO_TypeDef* GPIOx, GPIO_AF AF, USART_TypeDef* UARTx, UART_Mode OpMode);

uint8_t UART_RxBuffer(UART_Channel* Channel);

void UART_TxBuffer(UART_Channel* Channel, uint8_t Msg);

#endif /* SYSTEM_UART_H_ */
