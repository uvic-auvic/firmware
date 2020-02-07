/*
 * LED.c
 *
 *  Created on: Feb  7, 2020
 *      Author: Poornachander
 */

#include "ADC.h"
/* INCLUDES */
#include "stm32f0xx.h"
#include "RTOS.h"
#include <stdbool.h>

/* DEFINES */

/* TYPEDEFS */
typedef struct
{
	uint16_t rawData[ADC_CHANNEL_COUNT];
} ADC_data_S;

/* PRIVATE DATA DECLARAION */
static ADC_data_S ADC_data;

/* PRIVATE FUNCTION DECLARATION */

/* PRIVATE FUNCTION DEFINITION */
// static void ADC_private_calibration (void)
// {
// 	ADC1->CR &= ~ADC_CR_ADEN;
// 	ADC1->CR |= ADC_CR_ADCAL;//WHY WONT YOU TURN TO 1
// 	while((ADC1->CR & ADC_CR_ADCAL) != 0)
// 	{
// 		//TIME-OUT MANAGEMENT
// 	}
// }

static void ADC_private_initGPIOs(void)
{
	//Enable peripheral clock for GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void ADC_private_initADC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);

	ADC_DMARequestModeConfig(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

	// ADC_private_calibration();
	// ADC1->CR |= ADC_CR_ADEN;
	// ADC1->CFGR1 |= ADC_CFGR1_CONT;
	// ADC1->CHSELR |= ADC_CHSELR_CHSEL7	| ADC_CHSELR_CHSEL6
	// 			 | ADC_CHSELR_CHSEL5 	| ADC_CHSELR_CHSEL4
	// 			 | ADC_CHSELR_CHSEL3 	| ADC_CHSELR_CHSEL2
	// 			 | ADC_CHSELR_CHSEL1 	| ADC_CHSELR_CHSEL0;
}

static void ADC_private_initDMA(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_StructInit(&DMA_InitStructure);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_data.rawData;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_COUNT;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);

	// RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	// ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
	// DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));		//Peripheral Address
	// DMA1_Channel1->CMAR = (uint32_t) ADC_data.rawData;		//Memory Address
	// DMA1_Channel1->CNDTR = ADC_CHANNEL_COUNT;					//Memory Size
	// DMA1_Channel1->CCR |= DMA_CCR_MINC		| DMA_CCR_MSIZE_0
	// 				   | DMA_CCR_PSIZE_0 	| DMA_CCR_CIRC;
	// DMA1_Channel1->CCR |= DMA_CCR_EN;
}

/* PUBLIC FUNCTIONS */
void ADC_init(void)
{
	ADC_private_initGPIOs();
	ADC_private_initDMA();
	ADC_private_initADC();
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