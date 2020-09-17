/*
 * SPI.c
 *
 *  Created on: Aug 10, 2020
 *      Author: carte
 */

//PC9 - Yellow - CS
//PC10 - Orange - SCK
//PC11 - Brown - MISO
//PC12 - Red - MOSI

#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "utils.h"

void initSPI()
{

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	SPI_InitTypeDef initSPI;

	initSPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	initSPI.SPI_Mode = SPI_Mode_Master;

	UNUSED(initSPI);

}
