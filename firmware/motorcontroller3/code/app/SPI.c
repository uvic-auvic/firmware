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


void initSPI()
{

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

	GPIOE->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER4_1
										| GPIO_MODER_MODER5_1
										| GPIO_MODER_MODER6_1;

	GPIOE->AFR[0] |= ((uint32_t)0x05550500); //enable GPIO alternate function 5 for GPIO w/ appropriate pins


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
		SPI_Init(SPI4, &initSPI);

		SPI_Cmd(SPI4, ENABLE);

}

void SPI_run(void)
{
	uint8_t * dataToSend;
	if(circBuffer2D_getSpaceAvailable(SPI4_TX) <= 16U)
	{
		circBuffer2D_pop(SPI4_TX, dataToSend);
		SPI_I2S_SendData(SPI4, (uint16_t)&dataToSend);
	}

	uint16_t * dataReceived;
	if(circBuffer2D_getSpaceAvailable(SPI4_RX))
	{
		&dataReceived = SPI_I2S_ReceiveData(SPI4);
		circBuffer2D_push(SPI4_RX, dataReceived, 2U);
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

bool SPI_receive(circBuffer2D_channel_E channel, const uint8_t * data, const uint8_t length)
{

	bool ret = false;

	if((channel < CIRCBUFFER2D_CHANNEL_COUNT) && (data != NULL) && (length > 0))
	{
		ret = circBuffer2D_pop(channel, data);
	}

	return ret;
}

bool SPI_sendAndReceive(circBuffer2D_channel_E channel, const uint8_t * const data, const uint8_t length)
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

