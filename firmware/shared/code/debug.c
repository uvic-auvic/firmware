/*
 * debug.c
 * 
 *  Created on: Aug 6, 2020
 *      Author: Poornachander
 */

#include "debug.h"

#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "assert.h"
#include "circBuffer1D.h"

/* DEFINES */
#define UART_BAUD_RATE (115200U)

/* TYPEDEFS */

/* PRIVATE DATA */
extern const debug_config_S debug_config;

/* PRIVATE FUNCTIONS DECLARATION */
static void debug_private_configureGPIO(void);
static void debug_private_configureUARTPeriph(void);

/* PRIVATE FUNCTION DEFINITION */
static void debug_private_configureGPIO(void)
{
	// Check that all configs are valid
	assert(IS_GPIO_PIN_SOURCE(debug_config.HWConfig->rxPin));
	assert(IS_GPIO_PIN_SOURCE(debug_config.HWConfig->txPin));
	assert(IS_GPIO_ALL_PERIPH(debug_config.HWConfig->GPIOPort));

	// Clock should be enabled in the enable clock callback

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	// Configure GPIOs
	GPIO_InitStructure.GPIO_Pin = BITVALUE(debug_config.HWConfig->rxPin) | BITVALUE(debug_config.HWConfig->txPin);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // Input/Output controlled by peripheral
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(debug_config.HWConfig->GPIOPort, &GPIO_InitStructure);

	// Attach GPIO AF to UART
	GPIO_PinAFConfig(debug_config.HWConfig->GPIOPort, debug_config.HWConfig->rxPin, debug_config.HWConfig->AFNumber);
	GPIO_PinAFConfig(debug_config.HWConfig->GPIOPort, debug_config.HWConfig->txPin, debug_config.HWConfig->AFNumber);
}

static void debug_private_configureUARTPeriph(void)
{
	assert(IS_USART_ALL_PERIPH(debug_config.HWConfig->UARTPeriph));
	assert(IS_USART_BAUDRATE(UART_BAUD_RATE));

	// Clock should be enabled in the enable clock callback

	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);

	USART_InitStruct.USART_BaudRate = UART_BAUD_RATE;	// the baudrate is set to the value set in the debug_config
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Writes the setting above into the UART configuration registers
	USART_Init(debug_config.HWConfig->UARTPeriph, &USART_InitStruct);

	// Setup interrupt
	NVIC_SetPriority(debug_config.HWConfig->UARTInterruptNumber, 7);
	NVIC_EnableIRQ(debug_config.HWConfig->UARTInterruptNumber);

	USART_Cmd(debug_config.HWConfig->UARTPeriph, ENABLE);
}


/* PUBLIC FUNCTIONS */
void _debug_init()
{
	if(debug_config.HWConfig == NULL)
	{
		assert(0U);
	}

	if(debug_config.HWConfig->enablePeripheralsClockCallback != NULL)
	{
		debug_config.HWConfig->enablePeripheralsClockCallback();
	}
	else
	{
		assert(0U);
	}

	debug_private_configureGPIO();
	debug_private_configureUARTPeriph();
}

bool _debug_writeLen(const uint8_t * const data, const uint16_t length)
{
	bool ret = false;
	if((data != NULL) && (length != 0U) && (length <= DEBUG_TX_BUFFER_LENGTH))
	{
		ret = circBuffer1D_push(CIRCBUFFER1D_CHANNEL_DEBUG_TX, data, length);
		if(ret)
		{
			// Enable the TXNE interrupt. The IRQ will take care of starting the transmission
			USART_ITConfig(debug_config.HWConfig->UARTPeriph, USART_IT_TXE, ENABLE);
		}
	}

	return ret;
}

bool _debug_writeString(const char * const string)
{
	return _debug_writeLen((const uint8_t * const)string, strnlen(string, DEBUG_TX_BUFFER_LENGTH));
}

/* INTERRUPT HANDLER */

// Call this function in the UART interrupt handler for this channel. Place the interrupt handler in the debug_componentSpecific file
void debug_UARTInterruptHandler(void)
{
	if(USART_GetFlagStatus(debug_config.HWConfig->UARTPeriph, USART_FLAG_RXNE) == SET)
	{
		// Do nothing for now. RX not yet implemented
		USART_ITConfig(debug_config.HWConfig->UARTPeriph, USART_IT_RXNE, DISABLE);
	}
	else if(USART_GetFlagStatus(debug_config.HWConfig->UARTPeriph, USART_FLAG_TXE) == SET)
	{
		uint8_t byteToSend;
		if(circBuffer1D_popByte(CIRCBUFFER1D_CHANNEL_DEBUG_TX, &byteToSend))
		{
			debug_config.HWConfig->UARTPeriph->DR = byteToSend;
		}
		else
		{
			// Turn off TXE interrupt
			USART_ITConfig(debug_config.HWConfig->UARTPeriph, USART_IT_TXE, DISABLE);
		}
	}
	else
	{
		// Do nothing
	}
}
