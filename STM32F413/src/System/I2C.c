/*
 * I2C.c
 *
 *  Created on: March 4, 2023
 *      Author: Aman
 */

#include "System/I2C.h"

extern uint16_t switch_endiness_uint16(uint16_t input) {
	uint8_t temp = (input & 0xFF00) >> 8;
	input = (input & 0x00FF) << 8;
	input |= temp;
	return input;
}

extern uint32_t switch_endiness_uint32(uint32_t input, uint8_t numBytes) {

	uint32_t output = 0;

	for(uint8_t i = 0; i < numBytes; i++) {
		output = output << 8;
		output |= (input & 0xFF);
		input = input >> 8;
	}

	return output;
}

/** \brief  Initialize I2C Periperal

    The function initializes I2Cx peripheral [x = 1,2,3]

    \note Error checking is not implmented.

    \param [in]	Scl  Pin Number of Serial Clock.
    \param [in]	Sda  Pin Number of Serial Data.
    \param [in]	I2Cx  Address of I2C Peripheral. I2C1, I2C2, or I2C3
 */
extern I2C_Channel I2C_init(GPIO_Pin Scl, GPIO_Pin Sda, I2C_TypeDef * I2Cx) {
	I2C_Channel Channel;
	Channel.pinScl = Scl
	Channel.pinSda = Sda
	Channel.I2Cx = I2Cx;
	//Initialize I2C mutex
	Channel.mutex = xSemaphoreCreateMutex();

	// Configure the Peripheral
	RCC_EnableI2cClk(I2Cx);

	// Configure the Pins and activate the Interrupt Service Routine
	// TODO: Implement an Error Interrupt routine. Low Priority
	//I2C1 uses B Ports.
	if(I2Cx == I2C1){
		RCC_EnableGpioClk(GPIOB);
		GPIO_Init(Scl, GPIOB , MODER(Scl, GPIO_MODER_ANALOG), PUPDR(Scl, GPIO_PUDPR_NOPUPD),
			OSPEEDR(Scl, GPIO_OSPEEDR_LOW), AF(Scl, AF4), OTYPER(Scl, GPIO_OTYPER_PP));
		GPIO_Init(Sda, GPIOB, MODER(Sda, GPIO_MODER_ANALOG), PUPDR(Sda, GPIO_PUDPR_NOPUPD),
			OSPEEDR(Sda, GPIO_OSPEEDR_LOW), AF(Sda, AF4), OTYPER(Sda, GPIO_OTYPER_PP));
		NVIC_SetPriority(I2C1_EV_IRQn, 6); /* (4) */
		NVIC_EnableIRQ(I2C1_EV_IRQn); /* (5) */
		
		// NVIC_SetPriority(I2C1_ER_IRQn, 6); /* (4) */
		// NVIC_EnableIRQ(I2C1_ER_IRQn); /* (5) */
	// I2C2 uses B ports, but with different alternate functions
	} else if(I2Cx == I2C2){
		RCC_EnableGpioClk(GPIOB);
		GPIO_Init(Scl, GPIOB, MODER(Scl, GPIO_MODER_ANALOG), PUPDR(Scl, GPIO_PUDPR_NOPUPD),
			OSPEEDR(Scl, GPIO_OSPEEDR_LOW), AF(Scl, AF4), OTYPER(Scl, GPIO_OTYPER_PP));
		GPIO_Init(Sda, GPIOB, MODER(Sda, GPIO_MODER_ANALOG), PUPDR(Sda, GPIO_PUDPR_NOPUPD),
			OSPEEDR(Sda, GPIO_OSPEEDR_LOW), AF(Sda, (Sda == P11) ? AF4 : AF9), OTYPER(Sda, GPIO_OTYPER_PP));
		NVIC_SetPriority(I2C2_EV_IRQn, 6); /* (4) */
		NVIC_EnableIRQ(I2C2_EV_IRQn); /* (5) */
		// NVIC_SetPriority(I2C2_ER_IRQn, 6); /* (4) */
		// NVIC_EnableIRQ(I2C2_ER_IRQn); /* (5) */
	// I2C3 uses A, B, and C ports; but also two diferent alternate functions depending on the pair.
	} else if(I2Cx == I2C3){
		RCC_EnableGpioClk(GPIOA);
		RCC_EnableGpioClk((Sda == P9) ? GPIOC : GPIOB);
		GPIO_Init(Scl, GPIOA, MODER(Scl, GPIO_MODER_ANALOG), PUPDR(Scl, GPIO_PUDPR_NOPUPD),
			OSPEEDR(Scl, GPIO_OSPEEDR_LOW), AF(Scl, AF4), OTYPER(Scl, GPIO_OTYPER_PP));
		GPIO_Init(Sda, (Sda == P9) ? GPIOC : GPIOB, MODER(Sda, GPIO_MODER_ANALOG), PUPDR(Sda, GPIO_PUDPR_NOPUPD),
			OSPEEDR(Sda, GPIO_OSPEEDR_LOW), AF(Sda, (Sda == P9) ? AF4 : AF9), OTYPER(Sda, GPIO_OTYPER_PP));
		NVIC_SetPriority(I2C3_EV_IRQn, 6); /* (4) */
		NVIC_EnableIRQ(I2C3_EV_IRQn); /* (5) */
		// NVIC_SetPriority(I2C3_ER_IRQn, 6); /* (4) */
		// NVIC_EnableIRQ(I2C3_ER_IRQn); /* (5) */
	} else {
		// Throw some error.
	}

	// Setup the DMA 
	// Power on the peripheral
	I2Cx->CR1 |= I2C_CR1_PE;

	// Done
	return Channel;

	// I2C_InitTypeDef I2C_InitStructure;

	// /* I2C configuration */
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	// I2C1->CR1 = I2C_CR1_PE; //I don't think this is needed here

	// I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	// I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable; //
	// I2C_InitStructure.I2C_DigitalFilter = 0x00; //
	// I2C_InitStructure.I2C_OwnAddress1 = 0x00; //
	// I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; //
	// I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //
	// I2C_InitStructure.I2C_Timing = 0x00B01A4B; //0x00201D2B

}


extern void I2C_read(uint8_t address, uint8_t numBytes, uint8_t *message) {
	TaskToNotify = xTaskGetCurrentTaskHandle();

	I2C_inputBuffer = message;

	I2C1->CR2 = (numBytes << I2C_NBYTES_BIT) | (address << I2C_SADD_BIT) | I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
}

extern void I2C_write(uint8_t address, uint8_t numBytes, uint8_t message[]) {
	TaskToNotify = xTaskGetCurrentTaskHandle();

	memcpy(I2C_OutputBuffer, message, numBytes);

	I2C1->CR2 = (numBytes << I2C_NBYTES_BIT) | (address << I2C_SADD_BIT);
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;

}

void I2C1_EV_IRQnHandler(void) {
	//TODO: Implement This if needed.
}

void I2C2_EV_IRQnHandler(void) {
	//TODO: Implement This if needed.
}

void I2C3_EV_IRQnHandler(void) {

	if((I2C3->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE) {
		*I2C_inputBuffer = I2C3->RXDR;
		I2C_inputBuffer++;

	} else if ((I2C3->ISR & I2C_ISR_TXIS) == I2C_ISR_TXIS) {
		I2C3->TXDR = I2C_OutputBuffer[I2C_outputBufferIndex_Head];
		I2C_outputBufferIndex_Head = (I2C_outputBufferIndex_Head + 1) % I2C_OUTPUT_BUFFER_SIZE;

	} else if((I2C3->ISR & I2C_ISR_TC) == I2C_ISR_TC) { //If all bytes have been transmitted
		I2C3->CR2 |=  I2C_CR2_STOP;
		I2C_outputBufferIndex_Head = 0;
		I2C_inputBuffer = 0;

		vTaskNotifyGiveFromISR(TaskToNotify, pdFALSE);

	} else if ((I2C3->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF) { //If a NACK is returned

		vTaskNotifyGiveFromISR(TaskToNotify, pdFALSE);
	}
}
