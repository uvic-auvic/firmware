/*
 * debug.c
 *
 *  Created on: Aug 6, 2020
 *      Author: Poornachander
 */

#include "debug.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "utils.h"
#include "assert.h"
#include "RCCHelper.h"
#include "interruptHelper.h"
#include "LED.h"

/* DEFINES */
#define UART_BAUD_RATE (115200U)

/* TYPEDEFS */
typedef enum
{
    TX_STATE_INIT,
    TX_STATE_READY_TO_SEND
} debug_TXState_E;

typedef struct
{
    uint8_t TXSprintfBuffer[DEBUG_TX_BUFFER_LENGTH];
    uint8_t TXBuffer[DEBUG_TX_BUFFER_LENGTH];
    uint16_t currentLength;
    debug_TXState_E TXState;
} debug_data_S;

/* PRIVATE DATA */
extern const debug_config_S debug_config;
static debug_data_S debug_data;

/* PRIVATE FUNCTIONS DECLARATION */
static void debug_private_configureGPIO(void);
static void debug_private_configureUARTPeriph(void);
static void debug_private_configureDMAPeriph(void);

/* PRIVATE FUNCTION DEFINITION */
static void debug_private_configureGPIO(void)
{
    // Check that all configs are valid
    assert(IS_GPIO_PIN_SOURCE(debug_config.HWConfig->rxPin));
    assert(IS_GPIO_PIN_SOURCE(debug_config.HWConfig->txPin));
    assert(IS_GPIO_ALL_PERIPH(debug_config.HWConfig->GPIOPort));

    // Enable GPIO port clock
    RCCHelper_clockCmd(debug_config.HWConfig->GPIOPort, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    // Configure GPIOs
    GPIO_InitStructure.GPIO_Pin = BITVALUE(debug_config.HWConfig->rxPin) | BITVALUE(debug_config.HWConfig->txPin);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // Input/Output controlled by peripheral
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(debug_config.HWConfig->GPIOPort, &GPIO_InitStructure);

    // Attach GPIO AF to UART
    GPIO_PinAFConfig(debug_config.HWConfig->GPIOPort, debug_config.HWConfig->rxPin, debug_config.HWConfig->AFNumber);
    GPIO_PinAFConfig(debug_config.HWConfig->GPIOPort, debug_config.HWConfig->txPin, debug_config.HWConfig->AFNumber);
}

static void debug_private_configureDMAPeriph(void)
{
    assert(IS_DMA_ALL_CONTROLLER(debug_config.HWConfig->DMAPeriph));
    assert(IS_DMA_ALL_PERIPH(debug_config.HWConfig->TXDMAStream));
    assert(IS_DMA_CHANNEL(debug_config.HWConfig->TXDMAChannel));
    assert(IS_USART_ALL_PERIPH(debug_config.HWConfig->UARTPeriph));

    RCCHelper_clockCmd(debug_config.HWConfig->DMAPeriph, ENABLE);

    DMA_InitTypeDef DMAInitStruct;
    DMA_StructInit(&DMAInitStruct);

    DMAInitStruct.DMA_Channel = debug_config.HWConfig->TXDMAChannel;
    DMAInitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(debug_config.HWConfig->UARTPeriph->DR);
    DMAInitStruct.DMA_Memory0BaseAddr = (uint32_t)debug_data.TXBuffer;
    DMAInitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMAInitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMAInitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMAInitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMAInitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMAInitStruct.DMA_Mode = DMA_Mode_Normal;
    DMAInitStruct.DMA_Priority = DMA_Priority_Low;
    DMAInitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;

    DMA_Init(debug_config.HWConfig->TXDMAStream, &DMAInitStruct);
}

static void debug_private_configureUARTPeriph(void)
{
    assert(IS_USART_ALL_PERIPH(debug_config.HWConfig->UARTPeriph));
    assert(IS_USART_BAUDRATE(UART_BAUD_RATE));

    // Enable UART Periph clock
    RCCHelper_clockCmd(debug_config.HWConfig->UARTPeriph, ENABLE);

    USART_InitTypeDef USART_InitStruct;
    USART_StructInit(&USART_InitStruct);

    USART_InitStruct.USART_BaudRate = UART_BAUD_RATE;	// the baudrate is set to the value set in the debug_config
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
    USART_InitStruct.USART_StopBits = USART_StopBits_1;	// we want 1 stop bit (standard)
    USART_InitStruct.USART_Parity = USART_Parity_No;// we don't want a parity bit (standard)
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
    USART_InitStruct.USART_Mode = USART_Mode_Tx; // Only turn on transmitter for now

    // Writes the setting above into the UART configuration registers
    USART_Init(debug_config.HWConfig->UARTPeriph, &USART_InitStruct);

    // UART DMA Config
    USART_DMACmd(debug_config.HWConfig->UARTPeriph, USART_DMAReq_Tx, ENABLE);

    // Enable the UART transmitter and receiver
    // Peripheral will start reading from/writing to pins after this
    USART_Cmd(debug_config.HWConfig->UARTPeriph, ENABLE);

    // UART is ready to send data at this point
    debug_data.TXState = TX_STATE_READY_TO_SEND;
}

/* PUBLIC FUNCTIONS */
void _debug_init()
{
    assert(debug_config.HWConfig != NULL);

    debug_private_configureGPIO();
    debug_private_configureDMAPeriph();
    debug_private_configureUARTPeriph();

    _debug_writeStringBlocking("debug ready");
}

bool _debug_writeLen(const uint8_t * const data, const uint16_t length, const bool blocking)
{
    bool ret = false;

    if((data != NULL) && (length != 0U))
    {
        DMA_Stream_TypeDef * const DMAStream = debug_config.HWConfig->TXDMAStream;
        if(blocking)
        {
            // Wait until current transfer is completed
            while(DMA_GetCmdStatus(DMAStream) == ENABLE) {}

            for(uint16_t dataIndex = 0U; dataIndex < length; dataIndex++)
            {
                // TODO: Add timeout to this while
                while(USART_GetFlagStatus(debug_config.HWConfig->UARTPeriph, USART_FLAG_TXE) != SET) {}
                debug_config.HWConfig->UARTPeriph->DR = data[dataIndex];
            }

            ret = true;
        }
        else
        {
            uint16_t currentDataCounter;

            if(DMA_GetCmdStatus(DMAStream) == ENABLE) // Currently transmitting
            {
                DMA_Cmd(DMAStream, DISABLE);
                while(DMA_GetCmdStatus(DMAStream) == ENABLE) {}
                currentDataCounter = DMA_GetCurrDataCounter(DMAStream);
            }
            else
            {
                debug_data.currentLength = 0U;
                currentDataCounter = 0U;
            }

            const uint16_t newLength = LIMIT(length, DEBUG_TX_BUFFER_LENGTH - currentDataCounter);
            memmove(debug_data.TXBuffer, &(debug_data.TXBuffer[debug_data.currentLength - currentDataCounter]), currentDataCounter);
            memcpy(&(debug_data.TXBuffer[currentDataCounter]), data, newLength);
            DMA_SetCurrDataCounter(DMAStream, currentDataCounter + newLength);
            // DMAStream->M0AR = (uint32_t)debug_data.TXBuffer;
            debug_data.currentLength = currentDataCounter + newLength;

            DMA_ClearFlag(DMAStream, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);
            DMA_Cmd(DMAStream, ENABLE);

            ret = true;
        }
    }

    return ret;
}

bool _debug_writeString(const char * const format, ...)
{
    bool ret = false;

    if(debug_data.TXState == TX_STATE_READY_TO_SEND)
    {
        va_list args;
        va_start (args, format);
        uint16_t length = vsnprintf((char *)debug_data.TXSprintfBuffer, sizeof(debug_data.TXSprintfBuffer), format, args);

        debug_data.TXSprintfBuffer[length] = (uint8_t)'\n';
        ret = _debug_writeLen(debug_data.TXSprintfBuffer, length + 1, false);
    }

    return ret;
}

bool _debug_writeStringBlocking(const char * const format, ...)
{
    bool ret = false;

    if(debug_data.TXState == TX_STATE_READY_TO_SEND)
    {
        va_list args;
        va_start (args, format);
        uint16_t length = vsnprintf((char *)debug_data.TXSprintfBuffer, sizeof(debug_data.TXSprintfBuffer), format, args);

        debug_data.TXSprintfBuffer[length] = (uint8_t)'\n';
        ret = _debug_writeLen(debug_data.TXSprintfBuffer, length + 1, true);
    }

    return ret;
}
