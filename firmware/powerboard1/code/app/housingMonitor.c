/*
 * housingMonitor.c
 *
 *  Created on: Feb 14, 2020
 *      Author: me
*/

#include "housingMonitor.h"

enum housingMonitor_housingStatus_E
{
	No_leak = 0,
	Leak = 1
};

enum housingMonitor_housing_E
{
	Main = 0
};

uint16_t threshold = 2048;

void housingMonitor_initGPIO(void);
void housingMonitor_initDMA(void);
void housingMonitor_initADC(void);


void housingMonitor_initGPIO(void)
{
	//Enable peripheral clock for GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef _OUT;
    GPIO_StructInit(&GPIO_InitStructure_ADC);

	GPIO_InitStructure_ADC.GPIO_Pin     = GPIO_Pin_1;
	GPIO_InitStructure_ADC.GPIO_Mode    = GPIO_Mode_AN;
	GPIO_InitStructure_ADC.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure_ADC.GPIO_PuPd    = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &GPIO_InitStructure_ADC);

//Using Pin PA2 for signaling leaking
/*	GPIO_InitTypeDef GPIO_InitStructure_OUT;
	    GPIO_StructInit(&GPIO_InitStructure_OUT);

		GPIO_InitStructure_OUT.GPIO_Pin     = GPIO_Pin_2;
		GPIO_InitStructure_ADC.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure_OUT.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure_OUT.GPIO_PuPd    = GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOA, &GPIO_InitStructure_OUT);*/
}

void housingMonitor_initDMA(void)
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

housingMonitor_initADC();
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection         = ADC_ScanDirection_Upward;

    // Writes the settings above into the the ADC config registers
    ADC_Init(ADC1, &ADC_InitStructure);

    // Do calibration. Must be done before enabling the ADC
    ADC_GetCalibrationFactor(ADC1);

    // Enable the ADC
    ADC_Cmd(ADC1, ENABLE);

    // DMA will not stop after first ADC conversion stops
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);
}

void housingMonitor_init(void)
{
	housingMonitor_initGPIO();
	housingMonitor_initDMA();
	housingMonitor_initADC();

	ADC_StartOfConversion(ADC1);
}

housingMonitor_housingStatus_E housingMonitor_getHousingStatus(const housingMonitor_housing_E housing)
{
	uint16_t SensorValue = 0U;
	housingMonitor_housingStatus_E HStatus = No_leak;
	SensorValue = ADC_GetConversionValue(ADC1);
	if (SensorValue >= threshold)
	{
		HStatus = Leak;
	}
	return HStatus;
}

void housingMonitor_run(void)
{
	housingMonitor_housing_E housing = Main;
	if (housingMonitor_getHousingStatus(housing) == Leak)
	{
		//Do something here.
		//If leaking happened Pin PA2 output high, else is low
		//GPIOA->ODR |= GPIO_PIN_2;
	}
	//else GPIOA->ODR &= ~GPIO_PIN_2;
}
