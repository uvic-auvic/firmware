/*
 * flashInterface.h
 *
 *  Created on: Aug 18, 2018
 *      Author: Poornachander
 */

#ifndef FLASHINTERFACE_H_
#define FLASHINTERFACE_H_

#include <stdint.h>

typedef enum
{
    FLASH_INTERFACE_RET_OK,
    FLASH_INTERFACE_RET_NO_PERMISSION,
} flashInterface_ret_E;

void flashInterface_init(void);
void flashInterface_eraseAppSectors();
uint32_t flashInterface_readWord(const uint32_t addr);
uint8_t flashInterface_readByte(const uint32_t addr);
flashInterface_ret_E flashInterface_writeWork(const uint32_t addr, const uint32_t data);
flashInterface_ret_E flashInterface_writeByte(const uint32_t addr, const uint8_t data);
void flashInterface_unlockFlash(void);
void flashInterface_lockFlash(void);

#endif /* FLASHINTERFACE_H_ */
