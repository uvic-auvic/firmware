/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32fx.h"

#include "bootloader.h"
#include "time.h"
#include "flashInterface.h"

int main(void)
{
    time_init();
    flashInterface_init();
    bootloader_init();

    while(1)
    {
        bootloader_run();
    }
}
