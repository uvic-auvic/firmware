/*
 * UART_componentSpecific.c
 *
 *  Created on: Jul 23, 2019
 *      Author: Poornachander
 */

#include <stdlib.h>
#include "stm32f4xx.h"
#include "UART.h"

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength);
static void UART_enablePeripheralsClockCallback(void);

static const UART_HWConfig_S UART_HWConfig = 
{
	.rxPin = GPIO_PinSource6,
	.txPin = GPIO_PinSource5,
	.GPIOPort = GPIOD,

	.UARTPeriph = USART2,
	.UARTInterruptNumber = USART2_IRQn,
	.baudRate = 115200,

	.enablePeripheralsClockCallback = UART_enablePeripheralsClockCallback
};

static const UART_HWConfig_S UART_HWConfig = 
{
	.rxPin = GPIO_PinSource6,
	.txPin = GPIO_PinSource5,
	.GPIOPort = GPIOD,

	.UARTPeriph = USART2,
	.UARTInterruptNumber = USART2_IRQn,
	.baudRate = 115200,

	.enablePeripheralsClockCallback = UART_enablePeripheralsClockCallback
};

extern UART_config_S UART_config;
UART_config_S UART_config =
{
	.receiveCallback = UART_receiveCallback,
	.HWConfig = &UART_HWConfig
};

static void UART_enablePeripheralsClockCallback(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}

static void UART_receiveCallback(uint8_t const * const receiveData, const uint8_t receiveDataLength)
{
#if USE_PROTOBUFS
#error "Protobufs not supported yet"
	pb_istream_t istream = pb_istream_from_buffer(receiveData, UART_RX_BUFFER_LENGTH);
	UART_TO_BOARD_MESSAGE_TYPE decodedMessage;
	if(pb_decode(&istream, UART_TO_BOARD_MESSAGE_FIELDS, &decodedMessage))
	{
		
	}
#else

	if(receiveData != NULL)
	{
		const protocol_message_S * const message = (protocol_message_S *)receiveData;
		switch(message->messageID)
		{
			case protocol_MID_POLARIS_motorSetSpeed:
			{
				
				break;
			}

			//Can't receive message sent by this board
			case protocol_MID_MC_deviceName:
			{
				break;
			}

			// unexpected and unhandled message
			case protocol_RESERVED:
			case protocol_MID_POLARIS_deviceName:
			case protocol_MID_PB_deviceName:
			default:
			{
				break;
			}
			
		}

		UART_writeLen(receiveData, receiveDataLength);
	}

#endif
}

void DMA2_Stream5_IRQHandler(void)
{
	UART_DMAInterruptHandler();
}
