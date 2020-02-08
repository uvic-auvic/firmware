/*
 * UART.c
 */

#include "UART.h"

#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "FreeRTOS.h"
#include "task.h"
#include "circBuffer2D.h"
#include "circBuffer1D.h"

/* TYPEDEFS */
typedef enum
{
	UART_STATE_IDLE,
	UART_START_PROCESS_RECEIVED_DATA
} UART_state_E;

typedef struct
{
	TaskHandle_t taskHandle;
	UART_state_E state;
} UART_data_S;

/* PRIVATE DATA */
static UART_data_S UART_data;
extern UART_config_S UART_config;

/* PRIVATE FUNCTIONS DECLARATION */
static void UART_private_configureGPIO(void);
static void UART_private_configureUARTPeriph(void);
static void UART_private_run(void);

/* PRIVATE FUNCTION DEFINITION */
static void UART_private_configureGPIO(void)
{
	// Check that all configs are valid
	configASSERT(IS_GPIO_PIN_SOURCE(UART_config.HWConfig->rxPin));
	configASSERT(IS_GPIO_PIN_SOURCE(UART_config.HWConfig->txPin));
	configASSERT(IS_GPIO_ALL_PERIPH(UART_config.HWConfig->GPIOPort));

	// Clock should be enabled in the enable clock callback

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	// Configure GPIOs
	GPIO_InitStructure.GPIO_Pin = BITVALUE(UART_config.HWConfig->rxPin) | BITVALUE(UART_config.HWConfig->txPin);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // Input/Output controlled by peripheral
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(UART_config.HWConfig->GPIOPort, &GPIO_InitStructure);

	// Attach GPIO AF to UART. This section needs to change if UART6 needs to be used
	GPIO_PinAFConfig(UART_config.HWConfig->GPIOPort, UART_config.HWConfig->rxPin, UART_config.HWConfig->AFNumber);
	GPIO_PinAFConfig(UART_config.HWConfig->GPIOPort, UART_config.HWConfig->txPin, UART_config.HWConfig->AFNumber);
}

static void UART_private_configureUARTPeriph(void)
{
	configASSERT(IS_USART_ALL_PERIPH(UART_config.HWConfig->UARTPeriph)); // Switch to `IS_USART_APP_PERIPH` if needed

	// Clock should be enabled in the enable clock callback

	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);

	USART_InitStruct.USART_BaudRate = UART_config.HWConfig->baudRate;	// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(UART_config.HWConfig->UARTPeriph, &USART_InitStruct);

	UART_config.HWConfig->UARTPeriph->CR1 |= USART_CR1_RXNEIE; //Enable the receive interrupt

	NVIC_SetPriority(UART_config.HWConfig->UARTInterruptNumber, 7);
	NVIC_EnableIRQ(UART_config.HWConfig->UARTInterruptNumber);

	USART_Cmd(UART_config.HWConfig->UARTPeriph, ENABLE);
}

static void UART_private_initDMA(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);

	uint32_t peripheralBaseAddress = &(UART_config.HWConfig->UARTPeriph->RDR); // This won't work for STM32F4

	DMA_InitStruct.DMA_PeripheralBaseAddr = peripheralBaseAddress
}

static void UART_private_run(void)
{
	while(1)
	{
		switch(UART_data.state)
		{
			case UART_START_PROCESS_RECEIVED_DATA:
			{
				uint8_t receivedData[UART_RX_BUFFER_LENGTH] = {};
				const uint8_t bytesReceived = circBuffer2D_pop(CIRCBUFFER2D_CHANNEL_UART_RX, receivedData);
				if(bytesReceived != 0U)
				{
					if(UART_config.receiveCallback != NULL)
					{
						UART_config.receiveCallback(receivedData, bytesReceived);
					}
				}

				UART_data.state = UART_STATE_IDLE;

				break;
			}
			case UART_STATE_IDLE:
			default:
			{
				if(ulTaskNotifyTake(pdFALSE, portMAX_DELAY) != 0U)
				{
					UART_data.state = UART_START_PROCESS_RECEIVED_DATA;
				}
				break;
			}
		}
	}
}


/* PUBLIC FUNCTIONS */
extern void UART_init()
{
	if(UART_config.HWConfig->enablePeripheralsClockCallback != NULL)
	{
		UART_config.HWConfig->enablePeripheralsClockCallback();
	}
	else
	{
		configASSERT(0U);
	}

	UART_private_configureGPIO();
	UART_private_configureUARTPeriph();

	UART_data.state = UART_STATE_IDLE;

	(void)xTaskCreate((TaskFunction_t)UART_private_run,       /* Function that implements the task. */
					  "UARTTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  tskIDLE_PRIORITY + 1,/* Priority at which the task is created. */
					  &UART_data.taskHandle);      /* Used to pass out the created task's handle. */

}

/*
 * ERROR CODE:
 * -1 = String length is not 1 or greater
 * -2 = OutputBuffer will overflow. Wait some time and retry
 * 1  = Added to buffer successfully
 */
extern bool UART_write(char const * const data)
{
//	return UART_writeLen((uint8_t const * const)data, strnlen(data, UART_TX_BUFFER_SIZE));
	return false;
}

extern bool UART_writeLen(uint8_t const * const data, const uint8_t dataLength)
{
	bool ret = false;
	if((data != NULL) && (dataLength <= UART_TX_BUFFER_SIZE))
	{
		ret = circBuffer1D_push(CIRCBUFFER1D_CHANNEL_UART_TX, data, dataLength);
		UART_config.HWConfig->UARTPeriph->CR1 |= USART_CR1_TXEIE;
	}

	return ret;
}

/* INTERRUPT HANDLER */

// This is handling two cases. The interrupt will run if a character is received
// and when data is moved out from the transmit buffer and the transmit buffer is empty
static inline void UART_commonInterruptHandler(void)
{
	if(USART_GetFlagStatus(UART_config.HWConfig->UARTPeriph, USART_FLAG_RXNE) == SET) //If character is received
	{
		const uint8_t receivedByte = (const uint8_t)USART_ReceiveData(UART_config.HWConfig->UARTPeriph);

		if((receivedByte == '\n') || (receivedByte == '\r'))
		{
			uint8_t dataInInputBuffer[UART_RX_BUFFER_LENGTH];
			const uint8_t bytesInBuffer = circBuffer1D_pop(CIRCBUFFER1D_CHANNEL_UART_RX, dataInInputBuffer);

			if(bytesInBuffer > 0U)
			{
				if(circBuffer2D_push(CIRCBUFFER2D_CHANNEL_UART_RX, dataInInputBuffer, bytesInBuffer))
				{
					BaseType_t xHigherPriorityTaskWoken = pdFALSE;
					vTaskNotifyGiveFromISR(UART_data.taskHandle, &xHigherPriorityTaskWoken);
				}
			}
		}
		else
		{
			(void)circBuffer1D_pushByte(CIRCBUFFER1D_CHANNEL_UART_RX, receivedByte);
		}
	}
	if(USART_GetFlagStatus(UART_config.HWConfig->UARTPeriph, USART_FLAG_TXE) == SET) // If Transmission is complete
	{
		uint8_t dataToSend;
		if(circBuffer1D_popByte(CIRCBUFFER1D_CHANNEL_UART_TX, &dataToSend))
		{
			USART_SendData(UART_config.HWConfig->UARTPeriph, dataToSend);
		}
		else
		{
			UART_config.HWConfig->UARTPeriph->CR1 &= ~USART_CR1_TXEIE;
		}
	}
	else
	{
		// Unexpected interrupt. handle it
	}
}

void USART1_IRQHandler(void)
{
	UART_commonInterruptHandler();
}

void USART2_IRQHandler(void)
{
	UART_commonInterruptHandler();
}

void USART6_IRQHandler(void)
{
	UART_commonInterruptHandler();
}
