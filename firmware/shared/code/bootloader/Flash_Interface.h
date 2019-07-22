/*
 * Flash_Interface.h
 *
 *  Created on: Aug 18, 2018
 *      Author: Poornachander
 */

#ifndef FLASH_INTERFACE_H_
#define FLASH_INTERFACE_H_


extern void erase_app_sector();
extern uint32_t read_word(uint32_t addr);
extern uint8_t read_byte(uint32_t addr);
extern int8_t write_word(uint32_t addr, uint32_t data);
extern int8_t write_byte(uint32_t addr, uint8_t data);
extern void init_flash();
extern void deinit_flash();

#endif /* FLASH_INTERFACE_H_ */
