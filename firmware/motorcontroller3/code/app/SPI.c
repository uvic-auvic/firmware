/*
 * SPI.c
 *
 *  Created on: Aug 10, 2020
 *      Author: carte
 */

//PE2 SCK
//PE4 NSS
//PE5 MISO
//PE6 MOSI
//   SPI4

#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "utils.h"
#include "SPI.h"


void SPI_init()
{
	// Carter's code of initiating GPIOs
	/*RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

	GPIOE->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER4_1
										| GPIO_MODER_MODER5_1
										| GPIO_MODER_MODER6_1;

	GPIOE->AFR[0] |= ((uint32_t)0x05550500); //enable GPIO alternate function 5 for GPIO w/ appropriate pins*/

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//Init structures for GPIOA and GPIOC
	GPIO_InitTypeDef GPIOA_InitStructure, GPIOD_InitStructure;
	GPIO_StructInit(&GPIOA_InitStructure);
	GPIO_StructInit(&GPIOD_InitStructure);

	//
	GPIOA_InitStructure.GPIO_Pin     = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIOA_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
	GPIOA_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIOA_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIOA_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &GPIOA_InitStructure);

	GPIOD_InitStructure.GPIO_Pin     = GPIO_Pin_14;
	GPIOD_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
	GPIOD_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIOD_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIOD_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIOD_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_SPI1);

	SPI_InitTypeDef initSPI;

	initSPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	initSPI.SPI_DataSize = SPI_DataSize_8b;
	initSPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	initSPI.SPI_FirstBit = SPI_FirstBit_LSB;
	initSPI.SPI_Mode = SPI_Mode_Master;
	initSPI.SPI_CPHA = SPI_CPHA_1Edge;
	initSPI.SPI_CPOL = SPI_CPOL_Low;
	initSPI.SPI_CRCPolynomial = 7;
	initSPI.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &initSPI);

	SPI_Cmd(SPI1, ENABLE);

}

void SPI_run(void)
{
	uint16_t dataToSend;
	if(circBuffer2D_getSpaceAvailable(SPI4_TX) <= 16U)
	{
		circBuffer2D_pop(SPI4_TX, (uint8_t*)&dataToSend);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1, (uint16_t)(dataToSend));
	}

	uint16_t dataReceived;
	if(circBuffer2D_getSpaceAvailable(SPI4_RX))
	{
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
		dataReceived = SPI_I2S_ReceiveData(SPI1);
		circBuffer2D_push(SPI4_RX, (uint8_t*)&dataReceived, 2U);
	}
}


bool SPI_send(circBuffer2D_channel_E channel, const uint8_t * const data, const uint8_t length)
{

	bool ret = false;


	if((channel < CIRCBUFFER2D_CHANNEL_COUNT) && (data != NULL) && (length > 0) && circBuffer2D_getSpaceAvailable(channel))
	{
		ret = circBuffer2D_push(channel, data, length);
	}

	return ret;
}


// It seems that this function is not really needed
/*bool SPI_receive(circBuffer2D_channel_E channel,  uint8_t * const data, const uint8_t length)
{

	bool ret = false;

	if((channel < CIRCBUFFER2D_CHANNEL_COUNT) && (data != NULL) && (length > 0))
	{
		ret = circBuffer2D_pop(channel, data);
	}

	return ret;
}*/

bool SPI_sendAndReceive(circBuffer2D_channel_E channel,  uint8_t * const data, const uint8_t length)
{

	bool retSend = false;
	bool retReceive = false;

	if(SPI_send(channel, data, length))
	{
		retSend = true;
	}

	if(SPI_receive(channel, data, length))
	{
		retSend = true;
	}


	return retSend && retReceive;
}
