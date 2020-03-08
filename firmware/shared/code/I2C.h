/*
 *	I2CF4.h
 */

#ifndef I2C_H_
#define I2C_H_

#if (DEVICE_FAMILY_STM32F4)
#include "stm32f4xx.h"
#elif (DEVICE_FAMILY_STM32F0)
#include "stm32f0xx.h"
#endif

#include <stdint.h>

typedef struct {
	// GPIO
	uint8_t 	sclPin;
	uint8_t 	sdaPin;
	GPIO_TypeDef*	GPIOPort;
	
	// I2C
	I2C_TypeDef*	I2CPeriph;
	IRQn_Type	I2CInterruptNumber;
	uint32_t 	baudRate;

	// DMA
	DMA_TypeDef*	DMAController;
} I2C_HWConfig_S;

typedef enum {
	I2C_STANDARD = 0,
	I2C_EXT_ADDR // 10 bit device addressing 
} I2C_mode;

typedef struct {
	uint32_t	taskPriority;
	const I2C_HWConfig_S* const HWConfig;
} I2C_config_S;

typedef enum {
	I2C_IDLE = -1,
	I2C_WRITE = 0,
	I2C_READ = 1
} I2C_State;

#endif
