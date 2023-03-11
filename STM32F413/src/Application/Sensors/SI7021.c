/*
 * SI7021.c
 *
 *  Created on: Oct 15, 2018
 *      Author: Poorna
 *      Modified: amann
 */

#include "Application/Sensors/SI7021.h"

/***************************************************************************//**
 * Update_Humidity - Take the relative humidity and return it as a percentage
 ******************************************************************************/
extern uint16_t Update_Humidity() {
	if (xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)== pdTRUE) {

		uint8_t humidityAddress = CMD_MEASURE_RH_HM;

		I2C_write(Si_Address , 1, &humidityAddress);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFF;
		}

		uint16_t relativeHumidity = 0;

		I2C_read(Si_Address, 2, (uint8_t *)&relativeHumidity);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFF;
		}

		xSemaphoreGive(I2C_mutex);

		relativeHumidity = switch_endiness_uint16(relativeHumidity);

		return RELATIVE_HUMIDITY(relativeHumidity); //returns relative humidity %
	} else {
		return 0xFFFF;
	  }
}

/***************************************************************************//**
 * Update_Temperature - take the temperature in Kelvins and return it as 10*(actual temperature)
 ******************************************************************************/
extern uint16_t Update_Temperature() {

	if (xSemaphoreTake(I2C_mutex, I2C_TIMEOUT)== pdTRUE) {

		uint8_t tempAddress = CMD_MEASURE_TEMP_HM;

		I2C_write(Si_Address, 1, &tempAddress);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFF;
		}

		uint16_t temperature = 0;

		I2C_read(Si_Address, 2, (uint8_t *)&temperature);
		if(ulTaskNotifyTake(pdTRUE, I2C_TIMEOUT) == 0) {
			xSemaphoreGive(I2C_mutex);
			return 0xFFFF;
		}

		xSemaphoreGive(I2C_mutex);

		temperature = switch_endiness_uint16(temperature);

		return TEMPERATURE(temperature); //returns 10*temperature in Kelvins
	} else {
		return 0xFFFF;
	  }
}

uint16_t get_SI7021_Humidity(){
	return SI7021_Humid;
}
uint16_t get_SI7021_Temperature(){
	return SI7021_Temp;
}

//Get temperature and Humid and store it to a global variable to be get by one of the helper functions anytime.
//To make this periodic, use xTaskDelayUntil
// If this passes a certain threshold. Alert Us by Powering down peripherals and sending value to Jetson. This requires testing
void vI2cSensorSI7021Task( void *pvParameters ) {
	for( ;; ) {
		SI7021_Temp = Update_Temperature();
		SI7021_Humid = Update_Humidity();
		vTaskDelay(pdMS_TO_TICKS(SAMPLE_TIME));
	}
}