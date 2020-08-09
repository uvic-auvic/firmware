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

/* PRIVATE DATA */
typedef struct
{
    bool initSuccessful;
} CAN_data_S;

extern const CAN_config_S CAN_config;
static CAN_data_S CAN_data;

/* PRIVATE FUNCTIONS DECLARATION */
static void CAN_private_GPIOInit(void);
static void CAN_private_CANPeriphInit(void);
static void CAN_private_CANFilterInit(void);

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

    // Sample point at 90%
    CANInitStruct.CAN_Prescaler = (SystemCoreClock/2) / (BAUD_RATE * NUMBER_OF_TIME_QUANTA);
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
        debug_writeString("CAN init failed");
        CAN_data.initSuccessful = false;
    }
}

static void CAN_private_CANFilterInit(void)
{
    assert(IS_CAN_ALL_PERIPH(CAN_config.HWConfig->CANPeriph));

    CAN_FilterInitTypeDef CANFilterStruct;

    CANFilterStruct.CAN_FilterIdHigh = 0x011;
    CANFilterStruct.CAN_FilterIdLow = 0;
    CANFilterStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CANFilterStruct.CAN_FilterNumber = 0;
    CANFilterStruct.CAN_FilterMode = CAN_FilterMode_IdList;
    CANFilterStruct.CAN_FilterScale = CAN_FilterScale_16bit;
    CANFilterStruct.CAN_FilterActivation = ENABLE;

    CAN_FilterInit(CAN_config.HWConfig->CANPeriph, &CANFilterStruct);
    
}

/* PUBLIC FUNCTIONS */
void CAN_init(void)
{
    CAN_data.initSuccessful = true;

    assert(CAN_config.HWConfig != NULL);
    assert(CAN_config.HWConfig->enablePeripheralsClockCallback != NULL);

    CAN_config.HWConfig->enablePeripheralsClockCallback();

    CAN_private_GPIOInit();
    CAN_private_CANPeriphInit();
    CAN_private_CANFilterInit();
}

void CAN_SendMessage(const uint8_t * const data, const uint8_t dataLength)
{
    if(CAN_data.initSuccessful)
    {
        CanTxMsg txMsg;

        txMsg.StdId = 0x011;
        txMsg.IDE = CAN_Id_Standard;
        txMsg.RTR = CAN_RTR_Data;
        txMsg.DLC = dataLength;
        memcpy(txMsg.Data, data, MIN_OF(dataLength, sizeof(txMsg.Data)));

        CAN_Transmit(CAN_config.HWConfig->CANPeriph, &txMsg);
    }
}
