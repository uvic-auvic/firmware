/*
 * I2C.h
 *
 *  Created on: March 4, 2023
 *      Author: Aman
 */

#ifndef SYSTEM_I2C_H_
#define SYSTEM_I2C_H_

#include <stm32f413xx.h>
#include <core_cm4.h>

#include "GPIO.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define I2C_NBYTES_BIT	16U
#define I2C_SADD_BIT	1U

//I2C Output buffer
#define I2C_OUTPUT_BUFFER_SIZE	8U
uint8_t I2C_OutputBuffer[I2C_OUTPUT_BUFFER_SIZE];
uint8_t I2C_outputBufferIndex_Head = 0U;

//I2C Input Buffer
#define I2C_INPUT_BUFFER_SIZE	8U
uint8_t *I2C_inputBuffer;

//FreeRTOS current task handle
TaskHandle_t TaskToNotify = 0;


typedef struct __attribute__((__packed__)) {
    GPIO_Pin pinScl;
    GPIO_Pin pinSda;
    SemaphoreHandle_t mutex;
    I2C_TypeDef* I2Cx;
} I2C_Channel;

extern I2C_Channel I2C_init(GPIO_Pin Scl, GPIO_Pin Sda, I2C_TypeDef * I2Cx);

extern void I2C_read(uint8_t address, uint8_t numBytes, uint8_t *message);

extern void I2C_write(uint8_t address, uint8_t numBytes, uint8_t message[]);

#endif /* SYSTEM_I2C_H_ */
