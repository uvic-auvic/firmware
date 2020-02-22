/*
 * motorRPMFeedback.c
 *
 *  Created on: Feb 21, 2020
 *      Author: Poorna Chander Kalidas
 */

#include "motorRPMFeedback.h"

#include "stm32f4xx.h"
#include "RTOS.h"
#include <stdlib.h>

#define TIM5_PRESCALER		(0)
#define PWM_IN_TIMER_FREQ	((SystemCoreClock) /(TIM5_PRESCALER + 1))
#define PWM_IN_ARRAY_LENGTH (10)
#define PULSE_PER_ROTATION	(7)
#define FREQ_TO_RPM_CONV	((float)60 / (PULSE_PER_ROTATION))
#define INTERNAL_OSC_CALIB	(1) // In case we decide to adjust for the manufacturing error in the internal clock
#define MIN_RPM_PERIOD_MS	(85U) /* Equivalent to 100 RPM */

typedef struct
{
    uint32_t PWMInTimestamps[PWM_IN_ARRAY_LENGTH];
	uint32_t oldestTimestampDuringLastRun;
	uint32_t lastTimestampUpdate;
	int16_t RPM;
} motorRPMFeedback_motorData_S;

typedef struct
{
    motorRPMFeedback_motorData_S motorData[MOTOR_DRIVER_CHANNEL_COUNT];
} motorRPMFeedback_data_S;

static motorRPMFeedback_data_S motorRPMFeedback_data;

static void motorRPMFeedback_private_initGPIO(void);
static void motorRPMFeedback_private_initTimer(void);
static void motorRPMFeedback_private_initDMA(void);

static void motorRPMFeedback_private_initGPIO(void)
{
	// GPIOA clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Common Configuration for all PWM IN pins
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// GPIO Configuration: Motor 1, PA1, TIM5_CH2, DMA1 Stream4 Channel6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); // Connect TIM5 pins to AF

	// GPIO Configuration: Motor 2, PA2, TIM5_CH3, DMA1 Stream0 Channel6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); // Connect TIM5 pins to AF

	// GPIO Configuration: Motor 3, PA11, TIM1_CH4, DMA2 Stream4 Channel6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1); // Connect TIM1 pins to AF

	// GPIO Configuration: Motor 4, PA9, TIM1_CH2, DMA2 Stream2 Channel6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1); // Connect TIM1 pins to AF

	// GPIO Configuration: Motor 5, PD14, TIM4_CH3, DMA1 Stream7 Channel2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4); // Connect TIM4 pins to AF

	// GPIO Configuration: Motor 6, PD13, TIM4_CH2, DMA1 Stream3 Channel2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); // Connect TIM4 pins to AF

	// GPIO Configuration: Motor 7, PA7, TIM3_CH2, DMA1 Stream5 Channel5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); // Connect TIM3 pins to AF

	// GPIO Configuration: Motor 8, PB1, TIM3_CH4, DMA1 Stream2 Channel5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); // Connect TIM3 pins to AF
}

static void motorRPMFeedback_private_initTimer(void)
{
	// Enable peripheral clock for TIM1, TIM3, TIM4, TIM5
	RCC->APB1ENR |= RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5;
	RCC->APB2ENR |= RCC_APB2Periph_TIM1;

	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFFU;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM5_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	// Timer Configuration: Motor 1, PA1, TIM5_CH2, DMA1 Stream4 Channel6
	TIM5->DIER |= TIM_DIER_CC2DE; //Capture/Compare 1 DMA request enable
	TIM5->CCMR1 |= TIM_CCMR1_CC2S_0; // Configure channel 1 as input
	TIM5->CCER |= TIM_CCER_CC2E; // Enable capture

	// Timer Configuration: Motor 2, PA2, TIM5_CH3, DMA1 Stream0 Channel6
	TIM5->DIER |= TIM_DIER_CC3DE;
	TIM5->CCMR2 |= TIM_CCMR2_CC3S_0;
	TIM5->CCER |= TIM_CCER_CC3E;

	// Timer Configuration: Motor 3, PA11, TIM1_CH4, DMA2 Stream4 Channel6
	TIM1->DIER |= TIM_DIER_CC4DE;
	TIM1->CCMR2 |= TIM_CCMR2_CC4S_0;
	TIM1->CCER |= TIM_CCER_CC4E;

	// Timer Configuration: Motor 4, PA9, TIM1_CH2, DMA2 Stream2 Channel6
	TIM1->DIER |= TIM_DIER_CC2DE;
	TIM1->CCMR1 |= TIM_CCMR1_CC2S_0;
	TIM1->CCER |= TIM_CCER_CC2E;

	// Timer Configuration: Motor 5, PD14, TIM4_CH3, DMA1 Stream7 Channel2
	TIM4->DIER |= TIM_DIER_CC3DE;
	TIM4->CCMR2 |= TIM_CCMR2_CC3S_0;
	TIM4->CCER |= TIM_CCER_CC3E;

	// Timer Configuration: Motor 6, PD13, TIM4_CH2, DMA1 Stream3 Channel2
	TIM4->DIER |= TIM_DIER_CC2DE;
	TIM4->CCMR1 |= TIM_CCMR1_CC2S_0;
	TIM4->CCER |= TIM_CCER_CC2E;

	// Timer Configuration: Motor 7, PA7, TIM3_CH2, DMA1 Stream5 Channel5
	TIM3->DIER |= TIM_DIER_CC2DE;
	TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;
	TIM3->CCER |= TIM_CCER_CC2E;

	// Timer Configuration: Motor 8, PB1, TIM3_CH4, DMA1 Stream2 Channel5
	TIM3->DIER |= TIM_DIER_CC4DE;
	TIM3->CCMR2 |= TIM_CCMR2_CC4S_0;
	TIM3->CCER |= TIM_CCER_CC4E;

	TIM1->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
	TIM5->CR1 |= TIM_CR1_CEN;
}

static void motorRPMFeedback_private_initDMA(void)
{
	//enable DMA1 and DMA2 Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2, ENABLE);
	//create DMA structure
	DMA_InitTypeDef DMA_InitStructure;

	// Common configuration for all DMA streams
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	//setting circular mode
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//medium priority
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//source and destination data size word=32bit
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	//automatic memory destination increment enable.
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//source address increment disable
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//Location assigned to peripheral register will be source
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//chunk of data to be transfered
	DMA_InitStructure.DMA_BufferSize = PWM_IN_ARRAY_LENGTH;
	//source and destination start addresses
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &TIM5->CNT;

	// DMA Configuration: Motor 1, PA1, TIM5_CH2, DMA1 Stream4 Channel6
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_0].PWMInTimestamps;
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream4, ENABLE); //Enable the DMA1 - Stream 4

	// DMA Configuration: Motor 2, PA2, TIM5_CH3, DMA1 Stream0 Channel6
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_1].PWMInTimestamps;
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream0, ENABLE); //Enable the DMA1 - Stream 0

	// DMA Configuration: Motor 3, PA11, TIM1_CH4, DMA2 Stream4 Channel6
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_2].PWMInTimestamps;
	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream4, ENABLE); //Enable the DMA2 - Stream 4

	// DMA Configuration: Motor 4, PA9, TIM1_CH2, DMA2 Stream2 Channel6
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_3].PWMInTimestamps;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream2, ENABLE); //Enable the DMA2 - Stream 2

	// DMA Configuration: Motor 5, PD14, TIM4_CH3, DMA1 Stream7 Channel2
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_4].PWMInTimestamps;
	DMA_Init(DMA1_Stream7, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream7, ENABLE); //Enable the DMA1 - Stream 7

	// DMA Configuration: Motor 6, PD13, TIM4_CH2, DMA1 Stream3 Channel2
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_5].PWMInTimestamps;
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream3, ENABLE); //Enable the DMA1 - Stream 3

	// DMA Configuration: Motor 7, PA7, TIM3_CH2, DMA1 Stream5 Channel5
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_6].PWMInTimestamps;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream5, ENABLE); //Enable the DMA1 - Stream 5

	// DMA Configuration: Motor 8, PB1, TIM3_CH4, DMA1 Stream2 Channel5
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t) motorRPMFeedback_data.motorData[MOTOR_DRIVER_CHANNEL_7].PWMInTimestamps;
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream2, ENABLE); //Enable the DMA1 - Stream 2
}

void motorRPMFeedback_init(void)
{
    motorRPMFeedback_private_initGPIO();
    motorRPMFeedback_private_initTimer();
    motorRPMFeedback_private_initDMA();
}

// Returns the next index the DMA cotroller will write to in the PWM timestamps array
static uint16_t motorRPMFeedback_private_getCurrentDMAIndex(const motorDriver_channel_E channel)
{
	uint16_t ret = PWM_IN_ARRAY_LENGTH;

	if(channel < MOTOR_DRIVER_CHANNEL_COUNT)
	{
		/*
		* Below is the motor to DMA mapping for RPM
		* Motor 1 - DMA1 Stream4 CH6
		* Motor 2 - DMA1 Stream0 CH6
		* Motor 3 - DMA2 Stream4 CH6
		* Motor 4 - DMA2 Stream2 CH6
		* Motor 5 - DMA1 Stream7 CH2
		* Motor 6 - DMA1 Stream3 CH2
		* Motor 7 - DMA1 Stream5 CH5
		* Motor 8 - DMA1 Stream2 CH5
		*/
		volatile uint32_t * const motorToDMA_NTDR[MOTOR_DRIVER_CHANNEL_COUNT] = { &DMA1_Stream4->NDTR,
																					&DMA1_Stream0->NDTR,
																					&DMA2_Stream4->NDTR,
																					&DMA2_Stream2->NDTR,
																					&DMA1_Stream7->NDTR,
																					&DMA1_Stream3->NDTR,
																					&DMA1_Stream5->NDTR,
																					&DMA1_Stream2->NDTR };
	
		ret = PWM_IN_ARRAY_LENGTH - *motorToDMA_NTDR[channel];
	}

	return ret;
}

static int16_t motorRPMFeedback_private_getRPMFromBuffer(const motorDriver_channel_E channel)
{
	int16_t ret = 0;

	/*
	 * Below is the motor to Timer mapping for RPM
	 * Motor 1 - TIM5 CH2
	 * Motor 2 - TIM5 CH3
	 * Motor 3 - TIM1 CH4
	 * Motor 4 - TIM1 CH2
	 * Motor 5 - TIM4 CH3
	 * Motor 6 - TIM4 CH2
	 * Motor 7 - TIM3 CH2
	 * Motor 8 - TIM3 CH4
	 */
	static volatile uint16_t *motorToTIM_DIER[MOTOR_DRIVER_CHANNEL_COUNT] = { &TIM5->DIER,
																		      &TIM5->DIER,
																		      &TIM1->DIER,
																		      &TIM1->DIER,
																		      &TIM4->DIER,
																		      &TIM4->DIER,
																		      &TIM3->DIER,
																		      &TIM3->DIER };

	static uint16_t motorToTIM_CCxDE[MOTOR_DRIVER_CHANNEL_COUNT] = { TIM_DIER_CC2DE,
														   			 TIM_DIER_CC3DE,
														   			 TIM_DIER_CC4DE,
														   			 TIM_DIER_CC2DE,
														   			 TIM_DIER_CC3DE,
														   			 TIM_DIER_CC2DE,
														   			 TIM_DIER_CC2DE,
														   			 TIM_DIER_CC4DE };

	//need to stop raw data from updating
	*motorToTIM_DIER[channel] &= ~motorToTIM_CCxDE[channel];

	uint32_t avgTimeDiff = 0;

	for(uint8_t iteration = 0U; iteration < (PWM_IN_ARRAY_LENGTH / 2U); iteration++)
	{
		const uint8_t index1 = (motorRPMFeedback_private_getCurrentDMAIndex(channel) + (iteration * 2U)) % PWM_IN_ARRAY_LENGTH;
		const uint8_t index2 = (index1 + 1) % PWM_IN_ARRAY_LENGTH;

		if(motorRPMFeedback_data.motorData[channel].PWMInTimestamps[index1] >= motorRPMFeedback_data.motorData[channel].PWMInTimestamps[index2])
		{
			avgTimeDiff += motorRPMFeedback_data.motorData[channel].PWMInTimestamps[index1]	- motorRPMFeedback_data.motorData[channel].PWMInTimestamps[index2];
		}
		else //if not bigger, must be smaller
		{
			avgTimeDiff += (0xFFFFFFFFU - motorRPMFeedback_data.motorData[channel].PWMInTimestamps[index1]) + motorRPMFeedback_data.motorData[channel].PWMInTimestamps[index2];
		}
	}

	// make sure to turn the DMA back on
	*motorToTIM_DIER[channel] |= motorToTIM_CCxDE[channel];

	if(avgTimeDiff != 0U)
	{
		avgTimeDiff = avgTimeDiff / (PWM_IN_ARRAY_LENGTH / 2); // Average the timestamps
		ret = (PWM_IN_TIMER_FREQ / avgTimeDiff) * FREQ_TO_RPM_CONV * INTERNAL_OSC_CALIB; //Convert timestamps to rpm and apply correction for internal oscillator
		// Not checking for overflow. Motor RPM will likely never reach the 32767rpm.
	}

	return ret;
}

void motorRPMFeedback_run10ms(void)
{
	for(motorDriver_channel_E channel = (motorDriver_channel_E)0U; channel < MOTOR_DRIVER_CHANNEL_COUNT; channel++)
	{
		motorRPMFeedback_motorData_S * const motorData = &motorRPMFeedback_data.motorData[channel];

		motorData->RPM = motorRPMFeedback_private_getRPMFromBuffer(channel);

		const uint32_t newOldestTimeStamp = motorData->PWMInTimestamps[motorRPMFeedback_private_getCurrentDMAIndex(channel)]; // Get the oldest timestamp in the PWM_IN_TIMESTAMPS array
		if(newOldestTimeStamp == motorData->oldestTimestampDuringLastRun) // If the oldest timestamp hasn't changed since the last run
		{
			if(RTOS_getTimeElapsedMilliseconds(motorData->lastTimestampUpdate) > MIN_RPM_PERIOD_MS) // If the last update to the PWM_IN_TIMESTAMPS was MIN_RPM_PERIOD_MS ago, the motor is probably stopped
			{
				motorData->RPM = 0U;
			}
		}
		else
		{
			motorData->oldestTimestampDuringLastRun = newOldestTimeStamp; // Oldest timestamp has changed since last run, update it and the time this was calculated
			motorData->lastTimestampUpdate = RTOS_getTimeMilliseconds();
		}

		if((uint16_t)abs(motorData->RPM) < 110U)
		{
			motorData->RPM = 0U;
		}
	}
}

int16_t motorRPMFeedback_getMotorRPM(const motorDriver_channel_E channel)
{
	int16_t ret = 0U;

	if (channel < MOTOR_DRIVER_CHANNEL_COUNT)
	{
		ret = motorRPMFeedback_data.motorData[channel].RPM;
	}

	return ret;
}
