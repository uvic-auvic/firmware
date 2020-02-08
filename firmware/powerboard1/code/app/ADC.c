/*
 * ADC.c
 */

#include "ADC.h"

/* INCLUDES */
#include "stm32f0xx.h"
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
    [ADC_CHANNEL_WATER_SENSOR]          = ADC_Channel_0,
    [ADC_CHANNEL_EXTERNAL_PRESSURE]     = ADC_Channel_1,
    [ADC_CHANNEL_LEFT_BATT_VOLTAGE]     = ADC_Channel_2,
    [ADC_CHANNEL_RIGHT_BATT_VOLTAGE]    = ADC_Channel_3,
    [ADC_CHANNEL_RIGHT_BATT_CURRENT]    = ADC_Channel_4,
    [ADC_CHANNEL_LEFT_BATT_CURRENT]     = ADC_Channel_5,
};

/* PRIVATE FUNCTION DECLARATION */
static void ADC_private_initGPIOs(void);
static void ADC_private_initADC(void);
static void ADC_private_initDMA(void);

/* PRIVATE FUNCTION DEFINITION */
static void ADC_private_initGPIOs(void)
{
    //Enable peripheral clock for GPIOA
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void ADC_private_initADC(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_ScanDirection         = ADC_ScanDirection_Upward;

    // Writes the settings above into the the ADC config registers
    ADC_Init(ADC1, &ADC_InitStructure);

    // Do calibration. Must be done before enabling the ADC
    ADC_GetCalibrationFactor(ADC1);

    // Choose which HW ADC channels to sample and the sampling rate.
    for(ADC_channel_E channel = (ADC_channel_E)0U; channel < ADC_CHANNEL_COUNT; channel++)
    {
        ADC_ChannelConfig(ADC1, ADC_softwareChannelToADCChannelMapping[channel], ADC_SampleTime_28_5Cycles);
    }

    // Enable the ADC
    ADC_Cmd(ADC1, ENABLE);

    // DMA will not stop after first ADC conversion stops
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);
}

static void ADC_private_initDMA(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_StructInit(&DMA_InitStructure);

    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(ADC1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)ADC_data.rawData;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = ADC_CHANNEL_COUNT;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;

    // Writes the settings above into the DMA config registers
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // Enable the DMA channel
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/* PUBLIC FUNCTIONS */
void ADC_init(void)
{
    ADC_private_initGPIOs();
    ADC_private_initDMA();
    ADC_private_initADC();

    // Kick off the first conversion
    ADC_StartOfConversion(ADC1);
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
