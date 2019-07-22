/*
 * Flash_Interface.c
 *
 *  Created on: Aug 18, 2018
 *      Author: Poornachander
 */

#include "stm32f4xx.h"
#include "User_Defines.h"

uint8_t app_sector_write_protect = 0; //For future use. Ability to lock writing to app sector

extern void erase_app_sector() {
	// DO NOT erase sector_0 and sector_1. Bootloader resides in this area
#pragma message "TO DO:Need to check if FLASH_COMPLETE status is returned"
	FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3);

}

extern uint32_t read_word(uint32_t addr) {
	uint32_t *p = (uint32_t *)addr;
	return (*(p));
}

extern uint8_t read_byte(uint32_t addr) {
	uint8_t *p = (uint8_t *)addr;
	return (*(p));
}

extern int8_t write_word(uint32_t addr, uint32_t data) {

	if(addr >= APP_START_ADDRESS && addr < (APP_START_ADDRESS + APP_SECTION_SIZE - 4) && app_sector_write_protect == 0) {
		FLASH_ProgramWord(addr, data);
	} else {
		//Error: No permission to access memory space
		return -1;
	}
	return 1;
}

extern int8_t write_byte(uint32_t addr, uint8_t data) {

	if(addr >= APP_START_ADDRESS && addr < (APP_START_ADDRESS + APP_SECTION_SIZE) && app_sector_write_protect == 0) {
		FLASH_ProgramByte(addr, data);
	} else {
		//Error: No permission to access memory space
		return -1;
	}
	return 1;
}

extern void init_flash() {
	FLASH_Unlock();
}

extern void deinit_flash() {
	FLASH_Lock();
}
