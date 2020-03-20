/*
 * bootloader.h
 *
 *  Created on: Mar 17, 2020
 *      Author: Poorna Chander Kalidas
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

typedef struct
{
    void (* deinitPeripheralsCallback)(void);
} bootloader_config_S;

void bootloader_init(void);
void bootloader_run(void);

#endif /* BOOTLOADER_H_ */
