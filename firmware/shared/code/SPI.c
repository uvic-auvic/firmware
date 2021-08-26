/*
 * SPI.c
 *
 *  Created on: Aug 10, 2020
 *      Author: carte
 */

#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "RCCHelper.h"
#include "utils.h"
#include "SPI.h"

extern const SPI_config_S SPI_config;

void SPI_init()
{
	// Carter's code of initiating GPIOs
	/*RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

	GPIOE->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER4_1
										| GPIO_MODER_MODER5_1
										| GPIO_MODER_MODER6_1;

	GPIOE->AFR[0] |= ((uint32_t)0x05550500); //enable GPIO alternate function 5 for GPIO w/ appropriate pins*/

	// Check that all configs are valid
	assert(IS_GPIO_PIN_SOURCE(SPI_config.HWConfig->SCKPin));
	assert(IS_GPIO_ALL_PERIPH(SPI_config.HWConfig->SCKPort));
	assert(IS_GPIO_PIN_SOURCE(SPI_config.HWConfig->MISOPin));
	assert(IS_GPIO_ALL_PERIPH(SPI_config.HWConfig->MISOPort));
	assert(IS_GPIO_PIN_SOURCE(SPI_config.HWConfig->MOSIPin));
	assert(IS_GPIO_ALL_PERIPH(SPI_config.HWConfig->MOSIPort));
	assert(IS_GPIO_PIN_SOURCE(SPI_config.HWConfig->CSPin));
	assert(IS_GPIO_ALL_PERIPH(SPI_config.HWConfig->CSPort));
	assert(IS_SPI_ALL_PERIPH(SPI_config.HWConfig->SPIPeriph));

	// Enable clocks for GPIO ports and SPI peripheral
	RCCHelper_clockCmd(SPI_config.HWConfig->SCKPort, ENABLE);
	RCCHelper_clockCmd(SPI_config.HWConfig->MISOPort, ENABLE);
	RCCHelper_clockCmd(SPI_config.HWConfig->MOSIPort, ENABLE);
	RCCHelper_clockCmd(SPI_config.HWConfig->CSPort, ENABLE);
	RCCHelper_clockCmd(SPI_config.HWConfig->SPIPeriph, ENABLE);

	// GPIO init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin     = BITVALUE(SPI_config.HWConfig->SCKPin);
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_config.HWConfig->SCKPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin     = BITVALUE(SPI_config.HWConfig->MISOPin);
	GPIO_Init(SPI_config.HWConfig->MISOPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin     = BITVALUE(SPI_config.HWConfig->MOSIPin);
	GPIO_Init(SPI_config.HWConfig->MOSIPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin     = BITVALUE(SPI_config.HWConfig->CSPin);
	GPIO_Init(SPI_config.HWConfig->CSPort, &GPIO_InitStructure);

	GPIO_PinAFConfig(SPI_config.HWConfig->SCKPort, SPI_config.HWConfig->SCKPin, SPI_config.HWConfig->SPIAFNum);
	GPIO_PinAFConfig(SPI_config.HWConfig->MISOPort, SPI_config.HWConfig->MISOPin, SPI_config.HWConfig->SPIAFNum);
	GPIO_PinAFConfig(SPI_config.HWConfig->MOSIPort, SPI_config.HWConfig->MOSIPin, SPI_config.HWConfig->SPIAFNum);
	GPIO_PinAFConfig(SPI_config.HWConfig->CSPort, SPI_config.HWConfig->CSPin, SPI_config.HWConfig->SPIAFNum);

	// SPI peripheral init
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
	SPI_Init(SPI_config.HWConfig->SPIPeriph, &initSPI);

	// Enable SPI peripheral
	SPI_Cmd(SPI_config.HWConfig->SPIPeriph, ENABLE);
}

void SPI_run(void)
{
	uint16_t dataToSend;
	if(circBuffer2D_getSpaceAvailable(SPI_TX) <= 16U)
	{
		circBuffer2D_pop(SPI_TX, (uint8_t*)&dataToSend);
		while(!SPI_I2S_GetFlagStatus(SPI_config.HWConfig->SPIPeriph, SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI_config.HWConfig->SPIPeriph, (uint16_t)(dataToSend));
	}

	uint16_t dataReceived;
	if(circBuffer2D_getSpaceAvailable(SPI_RX))
	{
		while(!SPI_I2S_GetFlagStatus(SPI_config.HWConfig->SPIPeriph, SPI_I2S_FLAG_RXNE));
		dataReceived = SPI_I2S_ReceiveData(SPI_config.HWConfig->SPIPeriph);
		circBuffer2D_push(SPI_RX, (uint8_t*)&dataReceived, 2U);
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

bool SPI_sendAndReceive(circBuffer2D_channel_E channel,  uint8_t * const data, const uint8_t length)
{

	bool retSend = false;
	bool retReceive = false;

	if(SPI_send(channel, data, length))
	{
		retSend = true;
	}

	SPI_run();
	if (circBuffer2D_getSpaceAvailable(SPI_RX) < 16U)
	{
		retReceive = true;
	}
	return retSend && retReceive;
}
