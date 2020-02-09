/*
 * UART.c
 */

#include "UARTv2.h"

#include "UARTProtocol.h"
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "FreeRTOS.h"
#include "task.h"
#include "circBuffer2D.h"
#include "circBuffer1D.h"

/* DEFINES */

/* TYPEDEFS */
typedef enum
{
	UART_RX_IRQ_STATE_WAITING_FOR_HEADER,
	UART_RX_IRQ_STATE_RECEIVING_PAYLOAD,
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
	UART_RXBuffer_E 				 RXBufferBeingBuffered;
	volatile UART_RXBuffer_E		 RXBufferToProcess;
	UART_RXIRQState_E				 RXIRQState;

	// TX
	volatile UARTProtocol_protocol_S TXBuffer;
	TaskHandle_t					 taskHandle;
} UART_data_S;

/* PRIVATE DATA */
static UART_data_S UART_data;
extern UART_config_S UART_config;

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

	// Attach GPIO AF to UART. This section needs to change if UART6 needs to be used
	GPIO_PinAFConfig(UART_config.HWConfig->GPIOPort, UART_config.HWConfig->rxPin, UART_config.HWConfig->AFNumber);
	GPIO_PinAFConfig(UART_config.HWConfig->GPIOPort, UART_config.HWConfig->txPin, UART_config.HWConfig->AFNumber);
}

static void UART_private_initDMARX(void)
{
	configASSERT(IS_DMA_ALL_PERIPH(UART_config.HWConfig->DMAChannelRX));
	configASSERT(IS_DMA_ALL_LIST(UART_config.HWConfig->DMAController));

	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);

#if DEVICE_FAMILY_STM32F0
	const uint32_t peripheralBaseAddress = (uint32_t)&(UART_config.HWConfig->UARTPeriph->RDR); // This won't work for STM32F4
#else
#error "STM32F4 not supported"
#endif

	DMA_InitStruct.DMA_PeripheralBaseAddr = peripheralBaseAddress;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(UART_config.HWConfig->DMAChannelRX, &DMA_InitStruct);

	DMA_ITConfig(UART_config.HWConfig->DMAChannelRX, DMA_IT_TC, ENABLE);

	NVIC_SetPriority(UART_config.HWConfig->DMARXInterruptNumber, 7U);
	NVIC_EnableIRQ(UART_config.HWConfig->DMARXInterruptNumber);

	// Configure DMA to receive HEADER
	UART_data.RXBufferBeingBuffered = UART_RX_BUFFER_0;
	UART_data.RXBufferToProcess = UART_RX_BUFFER_COUNT; // Set to the invalid index
	UART_data.RXIRQState = UART_RX_IRQ_STATE_WAITING_FOR_HEADER;
	UART_config.HWConfig->DMAChannelRX->CMAR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header;
	UART_config.HWConfig->DMAChannelRX->CNDTR = sizeof(UART_data.RXBuffer[0U].header);
	DMA_Cmd(UART_config.HWConfig->DMAChannelRX, ENABLE);
}

static void UART_private_initDMATX(void)
{
	configASSERT(IS_DMA_ALL_PERIPH(UART_config.HWConfig->DMAChannelTX));
	configASSERT(IS_DMA_ALL_LIST(UART_config.HWConfig->DMAController));

	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);

	uint32_t peripheralBaseAddress = (uint32_t)&UART_config.HWConfig->UARTPeriph->TDR;

	DMA_InitStruct.DMA_PeripheralBaseAddr = peripheralBaseAddress;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&UART_data.TXBuffer.header;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(UART_config.HWConfig->DMAChannelTX, &DMA_InitStruct);

	DMA_ITConfig(UART_config.HWConfig->DMAChannelTX, DMA_IT_TC, ENABLE);

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
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver

	// Writes the setting above into the UART configuration registers
	USART_Init(UART_config.HWConfig->UARTPeriph, &USART_InitStruct);

	USART_DMACmd(UART_config.HWConfig->UARTPeriph, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

	USART_Cmd(UART_config.HWConfig->UARTPeriph, ENABLE);
}

static void UART_private_run(void)
{
	while(1)
	{
		uint32_t nextRun_ms = 0xFFFFFFFFU;
		const uint32_t ticks = nextRun_ms == 0xFFFFFFFFU ? portMAX_DELAY : pdMS_TO_TICKS(nextRun_ms);
		ulTaskNotifyTake(pdFALSE, ticks);

		if(UART_data.RXBufferToProcess < UART_RX_BUFFER_COUNT)
		{
			// Copy RXBufferToProcess so that it can be safely changed by the DMA IRQ
			const UART_RXBuffer_E RXBufferToProcess = UART_data.RXBufferToProcess;
			UART_data.RXBufferToProcess = UART_RX_BUFFER_COUNT; // Reset this so that the DMA IRQ can set it again

			// Copy the data in the DMA buffer so that the buffer can be safely used the DMA peripheral
			UARTProtocol_protocol_S dataToProcess;
			const uint16_t sizeToCopy = sizeof(UART_data.RXBuffer[0U].header) + sizeof(UART_data.RXBuffer[0U].data.crc) + UART_data.RXBuffer[RXBufferToProcess].header.length;
			memcpy(&dataToProcess, (const uint8_t *)&UART_data.RXBuffer[RXBufferToProcess], sizeToCopy);

			// Verify CRC (do later)


			// Send callback
			if(UART_config.receiveCallback != NULL)
			{
				UART_config.receiveCallback(dataToProcess.data.payload, dataToProcess.header.length);
			}
		}

		uint8_t dataToSend[UART_TX_BUFFER_LENGTH];
		const uint8_t dataLength = circBuffer2D_pop(CIRCBUFFER2D_CHANNEL_UART_TX, dataToSend);
		if(dataLength != 0U)
		{
			// New data available to send. Check if DMA is busy
			if((DMA_GetCurrDataCounter(UART_config.HWConfig->DMAChannelTX) == 0U) && ((UART_config.HWConfig->DMAChannelTX->CCR & DMA_CCR_EN) == DMA_CCR_EN))
			{
				const uint8_t frameLength = dataLength + sizeof(UART_data.TXBuffer.header) + sizeof(UART_data.TXBuffer.data.crc);
				DMA_SetCurrDataCounter(UART_config.HWConfig->DMAChannelTX, frameLength);

				UART_data.TXBuffer.header.length = dataLength;
				UART_data.TXBuffer.data.crc = 0x00U; // TODO: calculate CRC
				memcpy((uint8_t * const)UART_data.TXBuffer.data.payload, dataToSend, dataLength);

				DMA_Cmd(UART_config.HWConfig->DMAChannelTX, ENABLE);
			}
			else
			{
				// Set `nextRun`
			}
		}
	}
}

/* PUBLIC FUNCTIONS */
extern void UART_init()
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

	return ret;
}

/* INTERRUPT HANDLER */

/* DMA Interrupt Handler */
void DMA1_CH2_3_DMA2_CH1_2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3) == SET) // RX
	{
		DMA_ClearITPendingBit(DMA1_IT_TC3);

		switch(UART_data.RXIRQState)
		{
			default:
			case UART_RX_IRQ_STATE_WAITING_FOR_HEADER:
			{
				// F0 does not turn off the DMA channel when the Transfer is completed so turn it off before changing the registers
				DMA_Cmd(UART_config.HWConfig->DMAChannelRX, DISABLE);

				UART_config.HWConfig->DMAChannelRX->CNDTR = UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header.length + sizeof(UART_data.RXBuffer[0U].data.crc);
				UART_config.HWConfig->DMAChannelRX->CMAR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].data.crc;
				UART_data.RXIRQState = UART_RX_IRQ_STATE_RECEIVING_PAYLOAD;

				DMA_Cmd(UART_config.HWConfig->DMAChannelRX, ENABLE);
				break;
			}

			case UART_RX_IRQ_STATE_RECEIVING_PAYLOAD:
			{
				// F0 does not turn off the DMA channel when the Transfer is completed so turn it off before changing the registers
				DMA_Cmd(UART_config.HWConfig->DMAChannelRX, DISABLE);

				// Received payload, go back to receiving header and switch to second header
				UART_config.HWConfig->DMAChannelRX->CNDTR = sizeof(UART_data.RXBuffer[0U].header);
				UART_data.RXBufferToProcess = UART_data.RXBufferBeingBuffered; // The UART task uses this to determine if there is new data in the buffer
				UART_data.RXBufferBeingBuffered = (UART_data.RXBufferBeingBuffered + 1U) % UART_RX_BUFFER_COUNT; // Increment the buffer index to write to next
				UART_config.HWConfig->DMAChannelRX->CMAR = (uint32_t)&UART_data.RXBuffer[UART_data.RXBufferBeingBuffered].header; // Set the DMA memory address to the new address

				DMA_Cmd(UART_config.HWConfig->DMAChannelRX, ENABLE);

				UART_data.RXIRQState = UART_RX_IRQ_STATE_WAITING_FOR_HEADER;

				BaseType_t higherPriorityTaskWoken = pdFALSE;
				vTaskNotifyGiveFromISR(UART_data.taskHandle, &higherPriorityTaskWoken);
				portYIELD_FROM_ISR(higherPriorityTaskWoken);

				break;
			}
		}
	}

	if(DMA_GetITStatus(DMA1_IT_TC2) == SET) // TX
	{
		DMA_ClearITPendingBit(DMA1_IT_TC2);

		// F0 does not turn off the DMA channel when the Transfer is completed so turn it off before changing the registers
		DMA_Cmd(UART_config.HWConfig->DMAChannelRX, DISABLE);

		BaseType_t higherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_data.taskHandle, &higherPriorityTaskWoken);
		portYIELD_FROM_ISR(higherPriorityTaskWoken);
	}
}
