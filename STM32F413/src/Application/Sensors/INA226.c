/*
 * INA226_Current_Sensor.c
 *
 *  Created on: May 29, 2018
 *      Author: Poornachander
 */

#include <Application/Sensors/INA226.h>

extern void init_INA226_Current_Sensor() {

	uint8_t valueToWriteOnStartup[3] = {CALIB_REG_ADDRESS, CALIB_REG_VALUE_HIGH_BYTE, CALIB_REG_VALUE_LOW_BYTE};

	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {
		I2C_write(SYSTEM_SENSOR_I2C_ADDRESS, 3, valueToWriteOnStartup);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		I2C_write(MOTOR_SENSOR_I2C_ADDRESS, 3, valueToWriteOnStartup);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		xSemaphoreGive(I2C_mutex);
	}

}

extern uint32_t update_system_current() {

	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {

		uint8_t currentReg = CURRENT_REG_ADDRESS;

		I2C_write(SYSTEM_SENSOR_I2C_ADDRESS, 1, &currentReg);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFFFFFF;
		}

		uint32_t current = 0;

		I2C_read(SYSTEM_SENSOR_I2C_ADDRESS, 2, (uint8_t *)&current);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFFFFFF;
		}

		xSemaphoreGive(I2C_mutex);

		current = switch_endiness_uint16(current);
		current = TO_CURRENT(current) * (SYSTEM_CURRENT_CALC_CORRECTION);

		return current;
	} else {
		return 0xFFFFFFFF;
	}

}

extern uint32_t update_motor_current() {
	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {
		uint8_t currentReg = CURRENT_REG_ADDRESS;

		I2C_write(MOTOR_SENSOR_I2C_ADDRESS, 1, &currentReg);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFFFFFF;
		}

		uint32_t current = 0;

		I2C_read(MOTOR_SENSOR_I2C_ADDRESS, 2, (uint8_t *)&current);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFFFFFF;
		}

		xSemaphoreGive(I2C_mutex);

		current = switch_endiness_uint16(current);
		current = TO_CURRENT(current) * (MOTOR_CURRENT_CALC_CORRECTION);

		return current;
	} else {
		return 0xFFFFFFFF;
	}

}

extern uint16_t get_system_bus_voltage() {
	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {
		uint8_t voltageReg = BUS_VOLTAGE_REG_ADDRESS;

		I2C_write(SYSTEM_SENSOR_I2C_ADDRESS, 1, &voltageReg);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		uint16_t voltage = 0;

		I2C_read(SYSTEM_SENSOR_I2C_ADDRESS, 2, (uint8_t *)&voltage);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		xSemaphoreGive(I2C_mutex);

		voltage = switch_endiness_uint16(voltage);
		voltage = TO_VOLTAGE(voltage);

		return voltage;
	} else {
		return 0xFFFF;
	}

}

extern uint16_t get_motor_bus_voltage() {
	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {
		uint8_t voltageReg = BUS_VOLTAGE_REG_ADDRESS;

		I2C_write(MOTOR_SENSOR_I2C_ADDRESS, 1, &voltageReg);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		uint16_t voltage = 0;

		I2C_read(MOTOR_SENSOR_I2C_ADDRESS, 2, (uint8_t *)&voltage);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		xSemaphoreGive(I2C_mutex);

		voltage = switch_endiness_uint16(voltage);
		voltage = TO_VOLTAGE(voltage);

		return voltage;
	} else {
		return 0xFFFF;
	}

}

extern uint16_t get_system_shunt_voltage() {

	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {
		uint8_t shuntVoltageReg = SHUNT_VOLTAGE_REG_ADDRESS;

		I2C_write(SYSTEM_SENSOR_I2C_ADDRESS, 1, &shuntVoltageReg);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		uint16_t shuntVoltage = 0;

		I2C_read(SYSTEM_SENSOR_I2C_ADDRESS, 2, (uint8_t *)&shuntVoltage);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		xSemaphoreGive(I2C_mutex);

		shuntVoltage = switch_endiness_uint16(shuntVoltage);
		shuntVoltage = TO_SHUNT_VOLTAGE(shuntVoltage) * (SYSTEM_CURRENT_CALC_CORRECTION);

		return shuntVoltage;
	} else {
		return 0xFFFF;
	}

}

extern uint16_t get_motor_shunt_voltage() {
	if(xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)) {
		uint8_t shuntVoltageReg = SHUNT_VOLTAGE_REG_ADDRESS;

		I2C_write(MOTOR_SENSOR_I2C_ADDRESS, 1, &shuntVoltageReg);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		uint16_t shuntVoltage = 0;

		I2C_read(MOTOR_SENSOR_I2C_ADDRESS, 2, (uint8_t *)&shuntVoltage);
		ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT);

		xSemaphoreGive(I2C_mutex);

		shuntVoltage = switch_endiness_uint16(shuntVoltage);
		shuntVoltage = TO_SHUNT_VOLTAGE(shuntVoltage) * (MOTOR_CURRENT_CALC_CORRECTION);

		return shuntVoltage;
	} else {
		return 0xFFFF;
	}

}