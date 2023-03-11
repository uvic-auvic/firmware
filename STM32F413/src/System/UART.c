/*
 * UART.c
 *
 *  Created on: Feb 23, 2023
 *      Author: amann
 */

#include "System/UART.h"

UART_Channel UART_Init(GPIO_Pin Pin, GPIO_TypeDef* GPIOx, GPIO_AF AF, USART_TypeDef* UARTx, UART_Mode OpMode){
	UART_Channel UART;
	// Enable RCC CLock

	// Config UART Channel

	// Config GPIO Channel
	GPIO_Init(Pin, GPIOx, MODER(Pin, GPIO_MODER_AF),
			PUPDR(Pin, GPIO_PUDPR_NOPUPD), OSPEEDR(Pin, GPIO_OSPEEDR_HIGH),
			AF(Pin, AF), OTYPER(Pin, GPIO_OTYPER_PP));

	return UART;
}

uint8_t UART_RxBuffer(UART_Channel* Channel){
	return (uint8_t)Channel->UARTx->DR;
}

void UART_TxBuffer(UART_Channel* Channel, uint8_t Msg){

}
