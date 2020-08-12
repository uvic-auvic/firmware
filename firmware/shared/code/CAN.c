/*
 * CAN.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Poornachander
 */

#include "CAN.h"

/* INCLUDES */
#include "assert.h"
#include "utils.h"
#include "string.h"
#include <stdbool.h>
#include "debug.h"

/* DEFINES */
#define BAUD_RATE   (500000U)
#define NUMBER_OF_TIME_QUANTA   (10U)
#define STDID_BIT0_LOCATION (5U) // Bit 0 of the STDID starts at Bit 5 of the filter registers
#define PCLK    (SystemCoreClock/2)
#define MAX_RX_FILTERS  (14U)

/* TYPEDEFS */
typedef struct
{
    bool initSuccessful;
} CAN_data_S;

// CANx_FRx register layout
typedef struct
{
    uint8_t EXID_17_15:3; // EXID[17:15]
    uint8_t IDE:1;
    uint8_t RTR:1;
    uint16_t STDID:11;
} CAN_FR_S;

typedef union
{
    CAN_FR_S asStruct;
    uint16_t asUint16;
} CAN_FR_U;

/* PRIVATE DATA */
extern const CAN_config_S CAN_config;
static CAN_data_S CAN_data;

/* PRIVATE FUNCTIONS DECLARATION */
static void CAN_private_GPIOInit(void);
static void CAN_private_CANPeriphInit(void);
static void CAN_RXIRQ(CAN_TypeDef * CANx, const uint8_t FIFONumber);

/* PRIVATE FUNCTION DEFINITION */
static void CAN_private_GPIOInit(void)
{
	// Check that all configs are valid
	assert(IS_GPIO_PIN_SOURCE(CAN_config.HWConfig->rxPin));
	assert(IS_GPIO_PIN_SOURCE(CAN_config.HWConfig->txPin));
	assert(IS_GPIO_ALL_PERIPH(CAN_config.HWConfig->GPIOPort));

	// Clock should be enabled in the enable clock callback

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	// Configure GPIOs
	GPIO_InitStructure.GPIO_Pin = BITVALUE(CAN_config.HWConfig->rxPin) | BITVALUE(CAN_config.HWConfig->txPin);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // Input/Output controlled by peripheral
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CAN_config.HWConfig->GPIOPort, &GPIO_InitStructure);

	// Attach GPIO AF to UART
	GPIO_PinAFConfig(CAN_config.HWConfig->GPIOPort, CAN_config.HWConfig->rxPin, CAN_config.HWConfig->AFNumber);
	GPIO_PinAFConfig(CAN_config.HWConfig->GPIOPort, CAN_config.HWConfig->txPin, CAN_config.HWConfig->AFNumber);
}

static void CAN_private_CANPeriphInit(void)
{
    assert(IS_CAN_ALL_PERIPH(CAN_config.HWConfig->CANPeriph));

    CAN_InitTypeDef CANInitStruct;
    CAN_StructInit(&CANInitStruct);

    CAN_config.HWConfig->CANPeriph->MCR &= ~(0x10000U);

    // Sample point at 90%
    CANInitStruct.CAN_Prescaler = PCLK / (BAUD_RATE * NUMBER_OF_TIME_QUANTA);
    CANInitStruct.CAN_SJW = CAN_SJW_1tq;
    CANInitStruct.CAN_BS1 = CAN_BS1_8tq;
    CANInitStruct.CAN_BS2 = CAN_BS2_1tq;

    // CANInitStruct.CAN_Mode = CAN_Mode_Normal;
    CANInitStruct.CAN_Mode = CAN_Mode_Silent_LoopBack;
    CANInitStruct.CAN_TTCM = DISABLE;
    CANInitStruct.CAN_ABOM = DISABLE;
    CANInitStruct.CAN_AWUM = DISABLE;
    CANInitStruct.CAN_NART = DISABLE;
    CANInitStruct.CAN_RFLM = DISABLE;
    CANInitStruct.CAN_TXFP = ENABLE;

    if(CAN_Init(CAN_config.HWConfig->CANPeriph, &CANInitStruct) != CAN_InitStatus_Success)
    {
        debug_writeStringBlocking("CAN init failed");
        CAN_data.initSuccessful = false;
    }

    // Enable FIFO 0 not empty interrupt
    CAN_ITConfig(CAN_config.HWConfig->CANPeriph, CAN_IT_FMP0, ENABLE);
	NVIC_SetPriority(CAN1_RX0_IRQn, 4);
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
}

/* PUBLIC FUNCTIONS */
void CAN_init(void)
{
    CAN_data.initSuccessful = true;

    assert(CAN_config.HWConfig != NULL);
    assert(CAN_config.HWConfig->enablePeripheralsClockCallback != NULL);
    assert(IS_CAN_ALL_PERIPH(CAN_config.HWConfig->CANPeriph));

    CAN_config.HWConfig->enablePeripheralsClockCallback();

    CAN_private_GPIOInit();
    CAN_private_CANPeriphInit();
}

bool CAN_sendMessage(const protocol_MID_E messageID, const protocol_allMessages_U * const message, const uint8_t dataLength)
{
    bool ret = false;

    if(CAN_data.initSuccessful)
    {
        CanTxMsg txMsg;

        txMsg.StdId = messageID;
        txMsg.IDE = CAN_Id_Standard;
        txMsg.RTR = CAN_RTR_Data;
        txMsg.DLC = dataLength;
        memcpy(txMsg.Data, message, MIN_OF(dataLength, sizeof(txMsg.Data)));

        if(CAN_Transmit(CAN_config.HWConfig->CANPeriph, &txMsg) == CAN_TxStatus_NoMailBox)
        {
            debug_writeString("CAN TX Failed");
        }
        else
        {
            ret = true;
        }
    }

    return ret;
}

void CAN_filterAdd(const protocol_MID_E messageID, const uint16_t filterNumber)
{
    CAN_FilterInitTypeDef CANFilterStruct;
    memset(&CANFilterStruct, 0U, sizeof(CANFilterStruct));

    const CAN_FR_U filterLayout =
    {
        .asStruct = {
            .EXID_17_15 = 0U,
            .IDE = 0U,
            .RTR = 0U,
            .STDID = messageID,
        }
    };
    CANFilterStruct.CAN_FilterIdLow = filterLayout.asUint16;
    CANFilterStruct.CAN_FilterIdHigh = 0x0;
    CANFilterStruct.CAN_FilterMaskIdLow = 0x0;
    CANFilterStruct.CAN_FilterMaskIdHigh = 0x0;
    CANFilterStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CANFilterStruct.CAN_FilterNumber = filterNumber;
    CANFilterStruct.CAN_FilterMode = CAN_FilterMode_IdList;
    CANFilterStruct.CAN_FilterScale = CAN_FilterScale_16bit;
    CANFilterStruct.CAN_FilterActivation = ENABLE;

    CAN_FilterInit(CAN_config.HWConfig->CANPeriph, &CANFilterStruct);
}

void CAN1_RX0_IRQHandler(void)
{
    CAN_RXIRQ(CAN1, CAN_FIFO0);
}

static void CAN_RXIRQ(CAN_TypeDef * CANx, const uint8_t FIFONumber)
{
    CanRxMsg rxMsg;
    CAN_Receive(CANx, FIFONumber, &rxMsg);
    if(CAN_config.messageReceivedCallback != NULL)
    {
        debug_writeString("CAN RX TESTER");
        CAN_config.messageReceivedCallback(rxMsg.StdId, (protocol_allMessages_U *)rxMsg.Data);
    }
}