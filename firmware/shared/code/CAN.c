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
#include "RCCHelper.h"

/* DEFINES */
#define BAUD_RATE   (500000U)   // 500kbps
#define NUMBER_OF_TIME_QUANTA   (10U)  // Number of clock cycles during 1bit time
#define PCLK    (SystemCoreClock/2)  // Frequency of the peripheral clock

/* TYPEDEFS */
typedef enum
{
    CAN_PERIPH_1,
    CAN_PERIPH_2,
    CAN_PERIPH_3,

    CAN_PERIPH_COUNT,
} CAN_periph_E;

typedef enum
{
    CAN_TX_MAILBOX_0 = 0,
    CAN_TX_MAILBOX_1 = 1,
    CAN_TX_MAILBOX_2 = 2,

    CAN_TX_MAILBOX_COUNT,
} CAN_TXMailbox_E;

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

static const IRQn_Type CAN_interruptNumberMaping[CAN_PERIPH_COUNT] =
{
    [CAN_PERIPH_1] = CAN1_RX0_IRQn,
    [CAN_PERIPH_2] = CAN2_RX0_IRQn,
    [CAN_PERIPH_3] = CAN3_RX0_IRQn,
};

/* PRIVATE FUNCTIONS DECLARATION */
static void CAN_private_GPIOInit(void);
static void CAN_private_CANPeriphInit(void);
static IRQn_Type CAN_private_getIRQNumber(const CAN_TypeDef * periph);
static inline void CAN_private_RXIRQ(CAN_TypeDef * CANx, const uint8_t FIFONumber);

/* PRIVATE FUNCTION DEFINITION */
static void CAN_private_GPIOInit(void)
{
	// Check that all configs are valid
	assert(IS_GPIO_PIN_SOURCE(CAN_config.HWConfig->rxPin));
	assert(IS_GPIO_PIN_SOURCE(CAN_config.HWConfig->txPin));
	assert(IS_GPIO_ALL_PERIPH(CAN_config.HWConfig->GPIOPort));

	// Enable GPIO port clock
    RCCHelper_clockCmd(CAN_config.HWConfig->GPIOPort, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	// Configure GPIOs
	GPIO_InitStructure.GPIO_Pin = BITVALUE(CAN_config.HWConfig->rxPin) | BITVALUE(CAN_config.HWConfig->txPin);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // Input/Output controlled by peripheral
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
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

    // Enable CAN periph clock
    RCCHelper_clockCmd(CAN_config.HWConfig->CANPeriph, ENABLE);

    CAN_InitTypeDef CANInitStruct;
    CAN_StructInit(&CANInitStruct);

    // Sample point at 90%
    CANInitStruct.CAN_Prescaler = PCLK / (BAUD_RATE * NUMBER_OF_TIME_QUANTA); // Specifies the length of a time quantum
    
    // The parameters below are described in the CAN standard
    CANInitStruct.CAN_SJW = CAN_SJW_1tq; // Specifies the maximum number of time quanta the CAN hardware is allowed to lengthen or shorten a bit to perform resynchronization
    CANInitStruct.CAN_BS1 = CAN_BS1_8tq; // Specifies the number of time quanta in Bit Segment 1
    CANInitStruct.CAN_BS2 = CAN_BS2_1tq; // Specifies the number of time quanta in Bit Segment 2

    // CANInitStruct.CAN_Mode = CAN_Mode_Normal;
    CANInitStruct.CAN_Mode = CAN_Mode_Silent_LoopBack; // Used for testing
    CANInitStruct.CAN_TTCM = DISABLE; // Time triggered communication mode
    CANInitStruct.CAN_ABOM = DISABLE; // Automatic bus-off management
    CANInitStruct.CAN_AWUM = DISABLE; // Automatic wake-up mode
    CANInitStruct.CAN_NART = DISABLE; // Non-automatic retransmission mode
    CANInitStruct.CAN_RFLM = DISABLE; // Receive FIFO Locked mode
    CANInitStruct.CAN_TXFP = ENABLE; // Transmit FIFO priority

    if(CAN_Init(CAN_config.HWConfig->CANPeriph, &CANInitStruct) != CAN_InitStatus_Success)
    {
        debug_writeStringBlocking("CAN init failed");
        CAN_data.initSuccessful = false;
    }

    // Hard coded to only use FIFO_0
    // Enable FIFO 0 not empty interrupt
    CAN_ITConfig(CAN_config.HWConfig->CANPeriph, CAN_IT_FMP0, ENABLE);
	NVIC_SetPriority(CAN_private_getIRQNumber(CAN_config.HWConfig->CANPeriph), 5);
	NVIC_EnableIRQ(CAN_private_getIRQNumber(CAN_config.HWConfig->CANPeriph));
}

static IRQn_Type CAN_private_getIRQNumber(const CAN_TypeDef * periph)
{
    IRQn_Type ret;

    switch((uint32_t)periph)
    {
        case (uint32_t)CAN1:
        {
            ret = CAN_interruptNumberMaping[CAN_PERIPH_1];
            break;
        }                
        case (uint32_t)CAN2:
        {
            ret = CAN_interruptNumberMaping[CAN_PERIPH_2];
            break;
        }            
        case (uint32_t)CAN3:
        {
            ret = CAN_interruptNumberMaping[CAN_PERIPH_3];
            break;
        }            
        default:
        {
            ret = 0;
            ret = ~ret; // Max value that IRQn_Type can hold;
            break;
        }
    }

    return ret;
}

static inline void CAN_private_RXIRQ(CAN_TypeDef * CANx, const uint8_t FIFONumber)
{
    if(CAN_config.messageReceivedCallback != NULL)
    {
        uint32_t message[2U];

        message[0] = CANx->sFIFOMailBox[FIFONumber].RDLR;
        message[1] = CANx->sFIFOMailBox[FIFONumber].RDHR;

        protocol_MID_E messageID =  (protocol_MID_E)(0x000007FF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 21));

        CAN_config.messageReceivedCallback(messageID, (protocol_allMessages_U *)message);
    }

    /* Release the FIFO */
    /* Release FIFO0 */
    if (FIFONumber == CAN_FIFO0)
    {
        CANx->RF0R |= CAN_RF0R_RFOM0;
    }
    /* Release FIFO1 */
    else /* FIFONumber == CAN_FIFO1 */
    {
        CANx->RF1R |= CAN_RF1R_RFOM1;
    }
}

/* PUBLIC FUNCTIONS */
void CAN_init(void)
{
    CAN_data.initSuccessful = true;

    assert(CAN_config.HWConfig != NULL);
    assert(IS_CAN_ALL_PERIPH(CAN_config.HWConfig->CANPeriph));

    CAN_private_GPIOInit();
    CAN_private_CANPeriphInit();
}

bool CAN_sendMessage(const protocol_MID_E messageID, const protocol_allMessages_U * const message, const uint8_t dataLength)
{
    bool ret = false;

    if(CAN_data.initSuccessful)
    {
        CAN_TypeDef * CAN = CAN_config.HWConfig->CANPeriph;
        CAN_TXMailbox_E TXMailbox;

        // Select one empty transmit mailbox
        if ((CAN->TSR & CAN_TSR_TME0) == CAN_TSR_TME0)
        {
            TXMailbox = CAN_TX_MAILBOX_0;
        }
        else if ((CAN->TSR & CAN_TSR_TME1) == CAN_TSR_TME1)
        {
            TXMailbox = CAN_TX_MAILBOX_1;
        }
        else if ((CAN->TSR & CAN_TSR_TME2) == CAN_TSR_TME2)
        {
            TXMailbox = CAN_TX_MAILBOX_2;
        }
        else
        {
            TXMailbox = CAN_TX_MAILBOX_COUNT;
        }

        if(TXMailbox < CAN_TX_MAILBOX_COUNT)
        {
            // Program the message ID, RTR=0, IDE=0, Standard ID
            CAN->sTxMailBox[TXMailbox].TIR = (messageID << 21) & CAN_TI0R_STID;

            // Program the data length (DLC) into the register
            CAN->sTxMailBox[TXMailbox].TDTR &= ~(CAN_TDT0R_DLC);
            CAN->sTxMailBox[TXMailbox].TDTR |= dataLength;

            // Fill in the data to transmit in the data registers
            CAN->sTxMailBox[TXMailbox].TDLR = ((const uint32_t * const)message)[0U];
            CAN->sTxMailBox[TXMailbox].TDHR = ((const uint32_t * const)message)[1U];

            // Request transmission
            CAN->sTxMailBox[TXMailbox].TIR |= CAN_TI0R_TXRQ;

            ret = true;
        }
    }

    if(ret == false)
    {
        debug_writeString("CAN TX Failed");
    }

    return ret;
}

void CAN_filterAdd(const protocol_MID_E messageID, const uint16_t filterNumber)
{
    // The CAN bus will have many messages, but only a few of those messages will need to be received by this node
    // Filters are used to filter out irrelevant messages at the hardware level
    // An interrupt will only be fired when a message required by the application is received.

    CAN_FilterInitTypeDef CANFilterStruct;
    memset(&CANFilterStruct, 0U, sizeof(CANFilterStruct));

    const CAN_FR_U filterLayout =
    {
        .asStruct =
        {
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
    CANFilterStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; // Hard coded to only use FIFO_0
    CANFilterStruct.CAN_FilterNumber = filterNumber;
    CANFilterStruct.CAN_FilterMode = CAN_FilterMode_IdList;
    CANFilterStruct.CAN_FilterScale = CAN_FilterScale_16bit;
    CANFilterStruct.CAN_FilterActivation = ENABLE;

    // This configuration will give us 14 filters.
    // FilterIDHigh, FilterMaskLow and High can be populated if more filters are required.

    CAN_FilterInit(CAN_config.HWConfig->CANPeriph, &CANFilterStruct);
}

void CAN1_RX0_IRQHandler(void)
{
    CAN_private_RXIRQ(CAN1, CAN_FIFO0);
}

void CAN2_RX0_IRQHandler(void)
{
    CAN_private_RXIRQ(CAN2, CAN_FIFO0);
}

void CAN3_RX0_IRQHandler(void)
{
    CAN_private_RXIRQ(CAN3, CAN_FIFO0);
}
