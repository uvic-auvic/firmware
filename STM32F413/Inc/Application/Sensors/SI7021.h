/*
 * SI7021.h
 *
 *  Created on: Oct 15, 2018
 *      Author: Poorna
 *      Modified: amann
 */

#ifndef APPLICATION_SENSORS_SI7021_H_
#define APPLICATION_SENSORS_SI7021_H_

#include <stm32f413xx.h>
#include "System/I2C.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//Time out
#define I2C_TIMEOUT                  2000

//FreeRTOS Sample Time
#define SAMPLE_TIME                  5000U

//SI7021 register commands
#define CMD_MEASURE_RH_HM            0xE5
#define CMD_MEASURE_RH_NM            0xF5
#define CMD_MEASURE_TEMP_HM          0xE3
#define CMD_MEASURE_TEMP_NH          0xF3
#define CMD_READ_TEMP_PREV_RH        0xE0
#define CMD_RESET                    0xFE


//Si7021 I2C slave address
#define Si_Address                   0b1000000

#define RELATIVE_HUMIDITY(RH_CODE)	(((125*RH_CODE)/65536)-6)
#define TEMPERATURE(TEMP_CODE) (((1757.2*TEMP_CODE)/65536)+2263)

volatile uint16_t SI7021_Temp;
volatile uint16_t SI7021_Humid;

extern uint16_t Update_Humidity();
extern uint16_t Update_Temperature();

uint16_t get_SI7021_Humidity();
uint16_t get_SI7021_Temperature();

void vI2cSensorSI7021Task( void *pvParameters );

#endif /* APPLICATION_SENSORS_SI7021_H_ */
