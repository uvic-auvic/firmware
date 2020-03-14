/*
 * UART.c
 */

#include "UARTF4.h"

#include "UARTProtocol.h"
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "FreeRTOS.h"
#include "task.h"
#include "circBuffer2D.h"
#include "RTOS.h"

/* DEFINES */

/* TYPEDEFS */
typedef enum
{
	UART_RX_IRQ_STATE_WAITING_FOR_HEADER,
	UART_RX_IRQ_STATE_RECEIVING_PAYLOAD,
	UART_RX_IRQ_STATE_ERROR_INVALID_HEADER,
} UART_RXIRQState_E;

typedef enum
{
	UART_RX_BUFFER_0,
	UART_RX_BUFFER_1,

	UART_RX_BUFFER_COUNT,
} UART_RXBuffer_E;

typedef struct
{
	// RX
	volatile UARTProtocol_protocol_S RXBuffer[UART_RX_BUFFER_COUNT];
	volatile UART_RXBuffer_E 		 RXBufferBeingBuffered;
	volatile UART_RXBuffer_E		 RXBufferToProcess;
	volatile UART_RXIRQState_E		 RXIRQState;
	volatile uint32_t				 timeStartedPayloadReception;

	// TX
	volatile UARTProtocol_protocol_S TXBuffer;
	TaskHandle_t					 taskHandle;
} UART_data_S;

/* PRIVATE DATA */
static UART_data_S UART_data;
extern const UART_config_S UART_config;

/* PRIVATE FUNCTIONS DECLARATION */
static void UART_private_configureGPIO(void);
static void UART_private_configureUARTPeriph(void);
static void UART_private_initDMARX(void);
static void UART_private_initDMATX(void);

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

	// Attach GPIO AF to UART
	GPIO_PinAFConfig(UART_config.HWConfig->GPIOPort, UART_config.HWConfig->rxPin, UART_config.HWConfig->AFNumber);
	GPIO_PinAFConfig(UART_config.HWConfig->GPIOPort, UART_config.HWConfig->txPin, UART_config.HWConfig->AFNumber);
}

static void UART_private_initDMARX(void)
{
	configASSERT(IS_DMA_ALL_PERIPH(UART_config.HWConfig->DMAStreamRX));
	configASSERT(IS_DMA_ALL_CONTROLLER(UART_config.HWConfig->DMAController));
	configASSERT(IS_DMA_CHANNEL(UART_config.HWConfig->DMAChannelRX));

	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);

#if DEVICE_FAMILY_STM32F4
	const uint32_t peripheralBaseAddress = (uint32_t)&(UART_config.HWConfig->UARTPeriph->DR); // This won't work for STM32F4
#else
#error "STM32F0 not supported"
#endif

	DMA_InitStruct.DMA_Channel = UART_config.HWConfig->DMAChannelRX;
	DMA_InitStruct.DMA_PeripheralBaseAddr = peripheralBaseAddress;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(UART_config.HWConfig->DMAStreamRX, &DMA_InitStruct);

	DMA_ITConfig(UART_config.HWConfig->DMAStreamRX, DMA_IT_TC, ENABLE);

	NVIC_SetPriority(UART_config.HWConfig->DMARXInterruptNumber, 7U);
	NVIC_EnableIRQ(UART_config.HWConfig->DMARXInterruptNumber);

	// // Configure DMA to receive HEADER
	UART_data.RXBufferBeingBuffered = UART_RX_BUFFER_0;
	UART_data.RXBufferToProcess = UART_RX_BUFFER_COUNT; // Set to the invalid index
	UART_data.RXIRQState = UART_RX_IRQ_STATE_WAITING_FOR_HEADER;

	UART_config.HWConfig->DMAStreamRX->M0AR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header; 
	UART_config.HWConfig->DMAStreamRX->NDTR = sizeof(UART_data.RXBuffer[0U].header);
	DMA_Cmd(UART_config.HWConfig->DMAStreamRX, ENABLE);
}

static void UART_private_initDMATX(void)
{
	configASSERT(IS_DMA_ALL_PERIPH(UART_config.HWConfig->DMAStreamTX));
	configASSERT(IS_DMA_ALL_CONTROLLER(UART_config.HWConfig->DMAController));
	configASSERT(IS_DMA_CHANNEL(UART_config.HWConfig->DMAChannelTX));

	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);

	const uint32_t peripheralBaseAddress = (uint32_t)&UART_config.HWConfig->UARTPeriph->DR;

	DMA_InitStruct.DMA_Channel = UART_config.HWConfig->DMAChannelTX;
	DMA_InitStruct.DMA_PeripheralBaseAddr = peripheralBaseAddress;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&UART_data.TXBuffer.header;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(UART_config.HWConfig->DMAStreamTX, &DMA_InitStruct);

	DMA_ITConfig(UART_config.HWConfig->DMAStreamTX, DMA_IT_TC, ENABLE);

	NVIC_SetPriority(UART_config.HWConfig->DMATXInterruptNumber, 7U);
	NVIC_EnableIRQ(UART_config.HWConfig->DMATXInterruptNumber);

	// Buffer size and enable will be set in the run task
}

static void UART_private_configureUARTPeriph(void)
{
	configASSERT(IS_USART_ALL_PERIPH(UART_config.HWConfig->UARTPeriph));
	configASSERT(IS_USART_BAUDRATE(UART_config.HWConfig->baudRate));

	// Clock should be enabled in the enable clock callback

	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);

	USART_InitStruct.USART_BaudRate = UART_config.HWConfig->baudRate;	// the baudrate is set to the value set in the UART_config
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Writes the setting above into the UART configuration registers
	USART_Init(UART_config.HWConfig->UARTPeriph, &USART_InitStruct);

	USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

	USART_Cmd(UART_config.HWConfig->UARTPeriph, ENABLE);
}

static void UART_private_run(void)
{
	uint32_t nextRun_ms = 0xFFFFFFFFU;

	while(1)
	{
		const uint32_t ticks = nextRun_ms == 0xFFFFFFFFU ? portMAX_DELAY : pdMS_TO_TICKS(nextRun_ms);
		ulTaskNotifyTake(pdFALSE, ticks);
		nextRun_ms = 0xFFFFFFFFU;

		// RX
		if(UART_data.RXBufferToProcess < UART_RX_BUFFER_COUNT) // This variable will get set by the DMA IRQ when data is ready
		{
			// Copy RXBufferToProcess so that it can be safely changed by the DMA IRQ
			const UART_RXBuffer_E RXBufferToProcess = UART_data.RXBufferToProcess;

			// Copy the data in the DMA buffer so that the buffer can be safely used the DMA peripheral
			UARTProtocol_protocol_S dataToProcess;
			const uint16_t sizeToCopy = sizeof(UART_data.RXBuffer[0U].header) + sizeof(UART_data.RXBuffer[0U].data.crc) + UART_data.RXBuffer[RXBufferToProcess].header.length;
			memcpy(&dataToProcess, (const uint8_t *)&UART_data.RXBuffer[RXBufferToProcess], sizeToCopy);

			UART_data.RXBufferToProcess = UART_RX_BUFFER_COUNT; // Reset this to indicate that the new data has been acknowledged and it is safe for the DMA IRQ to change this variable

			// Verify CRC (do later)

			// Send callback
			if(UART_config.receiveCallback != NULL)
			{
				UART_config.receiveCallback(dataToProcess.data.payload, dataToProcess.header.length);
			}
		}

		// TX
		uint8_t dataToSend[UART_TX_BUFFER_LENGTH];
		const uint8_t dataLength = circBuffer2D_pop(CIRCBUFFER2D_CHANNEL_UART_TX, dataToSend);
		if(dataLength > 0U)
		{
			// New data available to send. Check if DMA is busy. Number for data remaining to transfer should be 0 and the DMA channel should be disabled
			const uint16_t bytesRemaining = DMA_GetCurrDataCounter(UART_config.HWConfig->DMAStreamTX);
			if((bytesRemaining == 0U) && (DMA_GetCmdStatus(UART_config.HWConfig->DMAStreamTX) == DISABLE))
			{
				const uint8_t frameLength = dataLength + sizeof(UART_data.TXBuffer.header) + sizeof(UART_data.TXBuffer.data.crc);
				DMA_SetCurrDataCounter(UART_config.HWConfig->DMAStreamTX, frameLength); // Set the new data length to transfer

				UART_data.TXBuffer.header.length = dataLength;
				UART_data.TXBuffer.data.crc = 0x00U; // TODO: calculate CRC
				memcpy((uint8_t * const)UART_data.TXBuffer.data.payload, dataToSend, dataLength);

				DMA_Cmd(UART_config.HWConfig->DMAStreamTX, ENABLE);
			}
			else
			{
				// Set `nextRun`
				nextRun_ms = 1U; // Try again in 1ms
			}
		}

		// RX ERROR HANDLING
		if(UART_data.RXIRQState == UART_RX_IRQ_STATE_ERROR_INVALID_HEADER)
		{
			// Received invalid header. Re-enable receiver. It should wait for a idle line before starting reception
			USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx, DISABLE); // Disable DMA Requests
			UART_config.HWConfig->UARTPeriph->CR1 &= ~USART_CR1_RE; // Turn off receiver
			DMA_Cmd(UART_config.HWConfig->DMAStreamRX, DISABLE); // Disable SMA channel
			UART_config.HWConfig->DMAStreamRX->M0AR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header;
			UART_config.HWConfig->DMAStreamRX->NDTR = sizeof(UART_data.RXBuffer[0U].header);
			UART_data.RXIRQState = UART_RX_IRQ_STATE_WAITING_FOR_HEADER;
			DMA_Cmd(UART_config.HWConfig->DMAStreamRX, ENABLE);
			USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx, ENABLE); // Enable DMA_reqs from UART periph
			UART_config.HWConfig->UARTPeriph->CR1 |= USART_CR1_RE; // Enable receiver
		}
		else if (UART_data.RXIRQState == UART_RX_IRQ_STATE_RECEIVING_PAYLOAD)
		{
			if(RTOS_getTimeElapsedMilliseconds(UART_data.timeStartedPayloadReception) >= 2U)
			{
				DMA_Cmd(UART_config.HWConfig->DMAStreamRX, DISABLE); // Disable DMA
				USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx, DISABLE); // Disable DMA Requests
				UART_config.HWConfig->UARTPeriph->CR1 &= ~USART_CR1_RE; // Turn off receiver
				UART_config.HWConfig->DMAStreamRX->NDTR = sizeof(UART_data.RXBuffer[0U].header); // Set number of expected bytes
				UART_data.RXIRQState = UART_RX_IRQ_STATE_WAITING_FOR_HEADER;
				DMA_Cmd(UART_config.HWConfig->DMAStreamRX, ENABLE); // Enable DMA
				USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx, ENABLE); // Enable DMA Requests
				UART_config.HWConfig->UARTPeriph->CR1 |= USART_CR1_RE; // Turn on receiver
			}
			else
			{
				nextRun_ms = 1U;
			}
		}
	}
}

/* PUBLIC FUNCTIONS */
void UART_init()
{
	if(UART_config.HWConfig == NULL)
	{
		configASSERT(0U);
	}

	if(UART_config.HWConfig->enablePeripheralsClockCallback != NULL)
	{
		UART_config.HWConfig->enablePeripheralsClockCallback();
	}
	else
	{
		configASSERT(0U);
	}

	UART_private_configureGPIO();
	UART_private_initDMARX();
	UART_private_initDMATX();
	UART_private_configureUARTPeriph();

	configASSERT(UART_config.taskPriority < configMAX_PRIORITIES);
	configASSERT(UART_config.taskPriority > tskIDLE_PRIORITY);

	(void)xTaskCreate((TaskFunction_t)UART_private_run,       /* Function that implements the task. */
					  "UARTTask",          /* Text name for the task. */
					  configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
					  NULL,    /* Parameter passed into the task. */
					  UART_config.taskPriority,/* Priority at which the task is created. */
					  &UART_data.taskHandle);      /* Used to pass out the created task's handle. */
}

bool UART_writeLen(const uint8_t * const data, const uint8_t length)
{
	bool ret = false;
	if((data != NULL) && (length != 0U) && (length <= UART_TX_BUFFER_LENGTH))
	{
		ret = circBuffer2D_push(CIRCBUFFER2D_CHANNEL_UART_TX, data, length);

		xTaskNotifyGive(UART_data.taskHandle);
	}

	return ret;
}

/* INTERRUPT HANDLER */

// Call this function in the DMA interrupt handler for this channel. Place the interrupt handler in the UART_componentSpecific file
void UART_DMAInterruptHandler(void)
{
	if(DMA_GetITStatus(UART_config.HWConfig->DMAStreamRX,  DMA_IT_TCIF5) == SET) // RX
	{
		DMA_ClearITPendingBit(UART_config.HWConfig->DMAStreamRX,  DMA_IT_TCIF5);

		switch(UART_data.RXIRQState)
		{
			case UART_RX_IRQ_STATE_WAITING_FOR_HEADER:
			{
				// F0 does not turn off the DMA channel when the Transfer is completed so turn it off before changing the registers
				DMA_Cmd(UART_config.HWConfig->DMAStreamRX, DISABLE);

				const uint8_t payloadLength = UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header.length;

				if((payloadLength <= UART_PROTOCOL_MAX_PAYLOAD_SIZE) & (payloadLength > 0U))
				{
					// Program the length of data to be expected
					UART_config.HWConfig->DMAStreamRX->NDTR = payloadLength + sizeof(UART_data.RXBuffer[0U].data.crc);
					// Change the address to write the new data to
					UART_config.HWConfig->DMAStreamRX->M0AR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].data.crc;
					UART_data.RXIRQState = UART_RX_IRQ_STATE_RECEIVING_PAYLOAD;
					UART_data.timeStartedPayloadReception = RTOS_getTimeMilliseconds();
					DMA_Cmd(UART_config.HWConfig->DMAStreamRX, ENABLE);
				}
				else
				{
					USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx, DISABLE); // Disable DMA Requests
					UART_config.HWConfig->UARTPeriph->CR1 &= ~USART_CR1_RE; // Turn off receiver

					UART_data.RXIRQState = UART_RX_IRQ_STATE_ERROR_INVALID_HEADER;
				}

				// need to notify the task for watchdog, or to handle error
				BaseType_t higherPriorityTaskWoken = pdFALSE;
				vTaskNotifyGiveFromISR(UART_data.taskHandle, &higherPriorityTaskWoken);
				portYIELD_FROM_ISR(higherPriorityTaskWoken);

				break;
			}

			case UART_RX_IRQ_STATE_RECEIVING_PAYLOAD:
			{
				// F0 does not turn off the DMA channel when the Transfer is completed so turn it off before changing the registers
				DMA_Cmd(UART_config.HWConfig->DMAStreamRX, DISABLE);

				// Received payload, go back to receiving header and switch to second header
				UART_config.HWConfig->DMAStreamRX->NDTR = sizeof(UART_data.RXBuffer[0U].header);
				UART_data.RXBufferToProcess = UART_data.RXBufferBeingBuffered; // The UART task uses this to determine if there is new data in the buffer
				UART_data.RXBufferBeingBuffered = (UART_data.RXBufferBeingBuffered + 1U) % UART_RX_BUFFER_COUNT; // Increment the buffer index to write to next
				UART_config.HWConfig->DMAStreamRX->M0AR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header; // Set the DMA memory address to the new address

				DMA_Cmd(UART_config.HWConfig->DMAStreamRX, ENABLE);

				UART_data.RXIRQState = UART_RX_IRQ_STATE_WAITING_FOR_HEADER;

				BaseType_t higherPriorityTaskWoken = pdFALSE;
				vTaskNotifyGiveFromISR(UART_data.taskHandle, &higherPriorityTaskWoken);
				portYIELD_FROM_ISR(higherPriorityTaskWoken);

				break;
			}

			case UART_RX_IRQ_STATE_ERROR_INVALID_HEADER:
			default:
			{

				break;
			}
		}
	}

	if(DMA_GetITStatus(UART_config.HWConfig->DMAStreamTX, DMA_IT_TCIF7) == SET) // TX
	{
		DMA_ClearITPendingBit(UART_config.HWConfig->DMAStreamTX, DMA_IT_TCIF7);

		// F0 does not turn off the DMA channel when the Transfer is completed so turn it off here
		DMA_Cmd(UART_config.HWConfig->DMAStreamTX, DISABLE);

		BaseType_t higherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_data.taskHandle, &higherPriorityTaskWoken);
		portYIELD_FROM_ISR(higherPriorityTaskWoken);
	}
}
