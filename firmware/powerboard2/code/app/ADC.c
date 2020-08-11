/*
 * ADC.c
 */

#include "ADC.h"

/* INCLUDES */
#include "stm32f4xx.h"
#include <stdbool.h>

/* DEFINES */

/* TYPEDEFS */
typedef struct
{
    uint16_t rawData[ADC_CHANNEL_COUNT];
} ADC_data_S;

/* PRIVATE DATA DECLARAION */
static ADC_data_S ADC_data;

static const uint32_t ADC_softwareChannelToADCChannelMapping[ADC_CHANNEL_COUNT] =
{
    [ADC_CHANNEL_ADC_BAT0_1]          = ADC_Channel_0,
    [ADC_CHANNEL_ADC_BAT0_2]          = ADC_Channel_1,
    [ADC_CHANNEL_ADC_BAT0_3]          = ADC_Channel_2,
    [ADC_CHANNEL_ADC_BAT0_4]          = ADC_Channel_3,
    [ADC_CHANNEL_ADC_BAT0_5]          = ADC_Channel_4,
    [ADC_CHANNEL_ADC_BAT0_6]          = ADC_Channel_5,
	[ADC_CHANNEL_ADC_BAT1_1]          = ADC_Channel_6,
	[ADC_CHANNEL_ADC_BAT1_2]          = ADC_Channel_7,
    [ADC_CHANNEL_ADC_BAT1_3]          = ADC_Channel_8,
    [ADC_CHANNEL_ADC_BAT1_4]          = ADC_Channel_9,
    [ADC_CHANNEL_ADC_BAT1_5]          = ADC_Channel_10,
	[ADC_CHANNEL_ADC_BAT1_6]          = ADC_Channel_11,
	[ADC_CHANNEL_ADC_EXT_PRESSURE]    = ADC_Channel_12,
};

/* PRIVATE FUNCTION DECLARATION */
static void ADC_private_initGPIOs(void);
static void ADC_private_initADC(void);
static void ADC_private_initDMA(void);

/* PRIVATE FUNCTION DEFINITION */
static void ADC_private_initGPIOs(void)
{
    //Enable peripheral clock for GPIOA, GPIOB, and GPIOC
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIOA_InitStructure;
    GPIO_StructInit(&GPIOA_InitStructure);

    GPIOA_InitStructure.GPIO_Pin     = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIOA_InitStructure.GPIO_Mode    = GPIO_Mode_AN;
    GPIOA_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIOA_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIOA_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOA, &GPIOA_InitStructure);

    GPIO_InitTypeDef GPIOB_InitStructure;

    GPIOB_InitStructure.GPIO_Pin     = GPIO_Pin_0 | GPIO_Pin_1;
    GPIOB_InitStructure.GPIO_Mode    = GPIO_Mode_AN;
    GPIOB_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIOB_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIOB_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOB, &GPIOB_InitStructure);

    GPIO_InitTypeDef GPIOC_InitStructure;

    GPIOC_InitStructure.GPIO_Pin     = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIOC_InitStructure.GPIO_Mode    = GPIO_Mode_AN;
    GPIOC_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIOC_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIOC_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOC, &GPIOC_InitStructure);
}

static void ADC_private_initADC(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode          = Single;
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion       = (uint8_t)ADC_CHANNEL_COUNT;

    // Writes the settings above into the the ADC config registers
    ADC_Init(ADC1, &ADC_InitStructure);

    // Do calibration. Must be done before enabling the ADC
    ADC_GetCalibrationFactor(ADC1);

    // Choose which HW ADC channels to sample and the sampling rate.
    for(ADC_channel_E channel = (ADC_channel_E)0U; channel < ADC_CHANNEL_COUNT; channel++)
    {
    	ADC_RegularChannelConfig(ADC1, ADC_softwareChannelToADCChannelMapping[channel], ADC_SampleTime_28Cycles);
    }

    // Enable the ADC
    ADC_Cmd(ADC1, ENABLE);

    // DMA will not stop after first ADC conversion stops
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
}

static void ADC_private_initDMA(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_StructInit(&DMA_InitStructure);

    DMA_InitStructure.DMA_Channel               = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(ADC1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t)ADC_data.rawData;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize            = ADC_CHANNEL_COUNT;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    //DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;
    //DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    //DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;

    // Writes the settings above into the DMA config registers
    DMA_Init(DMA1_Stream0, &DMA_InitStructure);

    // Enable the DMA channel
    DMA_Cmd(DMA1_Stream0, ENABLE);
}

/* PUBLIC FUNCTIONS */
void ADC_init(void)
{
    ADC_private_initGPIOs();
    ADC_private_initDMA();
    ADC_private_initADC();

    // Kick off the first conversion
    ADC_SoftwareStartConv(ADC1);
}

uint16_t ADC_getChannelData(const ADC_channel_E channel)
{
    uint16_t ret = 0U;
    if(channel < ADC_CHANNEL_COUNT)
    {
        ret = ADC_data.rawData[channel];
    }

    return ret;
}
