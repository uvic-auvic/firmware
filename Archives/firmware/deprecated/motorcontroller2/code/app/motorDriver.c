/*
 * motorDriver.c
 *
 *  Created on: Feb 21, 2020
 *      Author: Poorna Chander Kalidas
 * 
 *  Low level driver for controlling the PWM to the motors
 */

#include "motorDriver.h"

#include "stm32f4xx.h"

//PWM Out Defines
#define TARGET_FREQUENCY	(60.0f)
#define PWM_OUT_PRESCALER	(42 - 1)
#define PWM_OUT_PERIOD		((uint32_t)((SystemCoreClock) / (PWM_OUT_PRESCALER + 1U) / TARGET_FREQUENCY))

#define NETURAL_DUTY_CYCLE				(9.0f)
#define NETURAL_PERIOD					((uint32_t)((PWM_OUT_PERIOD * NETURAL_DUTY_CYCLE) / 100U))
#define POWER_LIMIT						(0.55f) // Limit the max power to 55%. Blue Robotics thrusters max voltage is 12V, battery voltage is 22V
#define FULL_POWER_DUTY_CYCLE_OFFSET	(3.0f * POWER_LIMIT)  // Full power is +/- 3% from NEUTRAL_DUTY_CYCLE
#define FULL_POWER_PERIOD_OFFSET		((uint32_t)((PWM_OUT_PERIOD * FULL_POWER_DUTY_CYCLE_OFFSET) / 100U))


static void motorDriver_private_initGPIO(void);
static void motorDriver_private_initTimer(void);

static void motorDriver_private_initGPIO(void)
{
	//Enable clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//GPIOA Configuration for motors 1-4 and 7
	GPIO_InitTypeDef GPIOA_InitStruct;
	GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_10 | GPIO_Pin_8 | GPIO_Pin_6;
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIOA_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOA_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIOA_InitStruct);

	//GPIOD configuration for motors 5-6
	GPIO_InitTypeDef GPIOD_InitStruct;
	GPIOD_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_12;
	GPIOD_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIOD_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOD_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOD_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIOD_InitStruct);

	//GPIOB configuration for motor 8
	GPIO_InitTypeDef GPIOB_InitStruct;
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIOB_InitStruct);

	//pin alternate function timer configuration
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
}

static void motorDriver_private_initTimer(void)
{
    // Enable clock for timers
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //timer set up
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = PWM_OUT_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_OUT_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//pwm init
	TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    //pwm config for TIM2 channel 1 PA0 motor 1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    //pwm config for TIM2 channel 4 PA3 motor 2
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    //pwm config for TIM4 channel 4 PD15 motor 5
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    //pwm config for TIM4 channel 1 PD12 motor 6
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    //pwm config for TIM3 channel 1 PA6 motor 7
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //pwm config for TIM3 channel 3 PB0 motor 8
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = NETURAL_PERIOD;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	//pwm config for TIM1 channel 3 PA10 motor 3
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	//pwm config for TIM1 channel 1 PA8 motor 4
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void motorDriver_init(void)
{
    motorDriver_private_initGPIO();
    motorDriver_private_initTimer();
    motorDriver_stopAllMotors();
}

void motorDriver_stopAllMotors(void)
{
    motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_0, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PA1
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_1, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PA3
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_2, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PA10
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_3, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PA8
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_4, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PD14
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_5, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PD12
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_6, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PA6
	motorDriver_setPowerLevelPercent(MOTOR_DRIVER_CHANNEL_7, 0, MOTOR_DRIVER_DIRECTION_FORWARD); //PB0
}

// Percent is in 10^-1 percent
void motorDriver_setPowerLevelPercent(const motorDriver_channel_E motor_x, uint16_t percent, const motorDriver_direction_E dir)
{
	percent = percent > MOTOR_DRIVER_PERCENT_BASE ? MOTOR_DRIVER_PERCENT_BASE : percent;
	uint32_t cc_value = (percent * FULL_POWER_PERIOD_OFFSET) / 1000U;

	if(dir == MOTOR_DRIVER_DIRECTION_FORWARD)
	{
		cc_value = NETURAL_PERIOD + cc_value;
	}
	else
	{
		cc_value = NETURAL_PERIOD - cc_value;
	}

	switch(motor_x){
		case MOTOR_DRIVER_CHANNEL_0:
			TIM_SetCompare1(TIM2, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_1:
			TIM_SetCompare4(TIM2, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_2:
			TIM_SetCompare3(TIM1, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_3:
			TIM_SetCompare1(TIM1, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_4:
			TIM_SetCompare4(TIM4, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_5:
			TIM_SetCompare1(TIM4, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_6:
			TIM_SetCompare1(TIM3, cc_value);
			break;
		case MOTOR_DRIVER_CHANNEL_7:
			TIM_SetCompare3(TIM3, cc_value);
			break;
        case MOTOR_DRIVER_CHANNEL_COUNT:
        default:
            break;
	}
}
