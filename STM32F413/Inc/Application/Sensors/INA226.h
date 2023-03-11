/*
 * INA226_Current_Sensor.h
 *
 *  Created on: May 29, 2018
 *      Author: Poornachander
 */

#ifndef INA226_CURRENT_SENSOR_H_
#define INA226_CURRENT_SENSOR_H_

#include <stm32f413xx.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "System/I2C.h"


#define I2C_TIMEOUT		2000

//INA226 Current and Power Sensor Register address
#define CONFIG_REG_ADDRESS	0x00
#define CALIB_REG_ADDRESS	0x05
#define CURRENT_REG_ADDRESS	0x04
#define POWER_REG_ADDRESS	0x03
#define BUS_VOLTAGE_REG_ADDRESS	0x02
#define SHUNT_VOLTAGE_REG_ADDRESS	0x01

//INA226 Slave address
#define MOTOR_SENSOR_I2C_ADDRESS	0b01001111 //0x4F
#define SYSTEM_SENSOR_I2C_ADDRESS	0b01001010 //0x4A

//Calibration register value
#define CALIB_REG_VALUE_HIGH_BYTE	0x10 //4mA per bit
#define CALIB_REG_VALUE_LOW_BYTE	0xAA //4mA per bit

//Correction to take into account trace resistance
#define SYSTEM_CURRENT_CALC_CORRECTION	((float)0.290) //Needs to be calibrated
#define MOTOR_CURRENT_CALC_CORRECTION	((float)0.714) //Needs to be calibrated

//Formulas to convert bits to actual values with units
#define TO_CURRENT(x) ((x)*(4))
#define TO_VOLTAGE(x) ((x)*1.25)
#define TO_SHUNT_VOLTAGE(x) ((x)*2.5)
#define TO_POWER(x) ((x)*25)



extern void init_INA226_Current_Sensor();
extern uint32_t update_system_current();
extern uint32_t update_motor_current();
extern uint16_t get_system_bus_voltage();
extern uint16_t get_motor_bus_voltage();
extern uint16_t get_system_shunt_voltage();
extern uint16_t get_motor_shunt_voltage();

#endif /* INA226_CURRENT_SENSOR_H_ */