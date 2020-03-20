/*
 * flashInterface.c
 *
 *  Created on: Aug 18, 2018
 *      Author: Poornachander
 */

#include "flashInterface.h"
#include "stm32fx.h"
#include <stdbool.h>

typedef struct
{
	bool appSectorWriteProtect;
} flashInterface_data_S;

static flashInterface_data_S flashInterface_data;

/* PUBLIC FUNCTION DECLARATION */
void flashInterface_init(void)
{
	flashInterface_data.appSectorWriteProtect = false;
}

void flashInterface_unlockFlash(void)
{
	FLASH_Unlock();
}

void flashInterface_lockFlash(void)
{
	FLASH_Lock();
}

void flashInterface_eraseAppSectors(void)
{
	// DO NOT erase sector_0 and sector_1. Bootloader resides in this area
// #pragma message "TO DO:Need to check if FLASH_COMPLETE status is returned"
	FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3);

}

uint32_t flashInterface_readWord(const uint32_t addr)
{
	uint32_t *p = (uint32_t *)addr;
	return (*(p));
}

uint8_t flashInterface_readByte(const uint32_t addr)
{
	uint8_t *p = (uint8_t *)addr;
	return (*(p));
}

flashInterface_ret_E flashInterface_writeWork(const uint32_t addr, const uint32_t data)
{	
	flashInterface_ret_E ret = FLASH_INTERFACE_RET_NO_PERMISSION;
	if(addr >= APP_START_ADDRESS && addr < (APP_START_ADDRESS + APP_SECTION_SIZE - 4) && flashInterface_data.appSectorWriteProtect == false)
	{
		FLASH_ProgramWord(addr, data);
		ret = FLASH_INTERFACE_RET_OK;
	}

	return ret;
}

flashInterface_ret_E flashInterface_writeByte(const uint32_t addr, const uint8_t data)
{
	flashInterface_ret_E ret = FLASH_INTERFACE_RET_NO_PERMISSION;
	if(addr >= APP_START_ADDRESS && addr < (APP_START_ADDRESS + APP_SECTION_SIZE) && flashInterface_data.appSectorWriteProtect == false) {
		FLASH_ProgramByte(addr, data);
		ret = FLASH_INTERFACE_RET_OK;
	}

	return ret;
}
