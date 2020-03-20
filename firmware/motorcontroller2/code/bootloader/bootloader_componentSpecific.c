/*
 * bootloader_componentSpecific.c
 *
 *  Created on: Mar 20, 2020
 *      Author: Poornachander
 */

#include "bootloader.h"

static void bootloader_componentSpecific_deinitPeripheralsCallback(void);

const bootloader_config_S bootloader_config =
{
	.deinitPeripheralsCallback = bootloader_componentSpecific_deinitPeripheralsCallback,
};

static void bootloader_componentSpecific_deinitPeripheralsCallback(void)
{
	
}