/*
 * LED.c
 *
 *  Created on: Jan. 28, 2023
 *      Author: amann
 */

#include "Application/LED.h"


LED_Channel LED_Init(GPIO_Pin PIN, GPIO_TypeDef* GPIOx, LED_Color LED, LED_Blink BLINK){
	LED_Channel LEDx;
	LEDx.LED = LED;
	LEDx.BLINK = BLINK;
	LEDx.CHANNEL.PIN = PIN;
	LEDx.CHANNEL.GPIOx = GPIOx;
	GPIO_Init(PIN, GPIOx, MODER(PIN, GPIO_MODER_OUTPUT), PUPDR(PIN, GPIO_PUDPR_NOPUPD),
			OSPEEDR(PIN, GPIO_OSPEEDR_HIGH), AF(PIN, AF0), OTYPER(PIN, GPIO_OTYPER_PP));
	// Turn on LED by default
	LED_On(&LEDx);
	return LEDx;
}

extern void set_Blink_Off(LED_Channel *LEDx){
	LEDx->BLINK = BLINK_OFF;
}
extern void set_Blink_Slow(LED_Channel *LEDx){
	LEDx->BLINK = BLINK_SLOW;
}
extern void set_Blink_Normal(LED_Channel *LEDx){
	LEDx->BLINK = BLINK_NORMAL;
}
extern void set_Blink_Fast(LED_Channel *LEDx){
	LEDx->BLINK = BLINK_FAST;
}
extern void LED_On(LED_Channel *LEDx){
	LEDx->STATE = LED_SET;
	GPIO_WritePin(&(LEDx->CHANNEL), GPIO_SET);
}
extern void LED_Off(LED_Channel *LEDx){
	LEDx->STATE = LED_RESET;
	GPIO_WritePin(&(LEDx->CHANNEL), GPIO_RESET);
}
extern void LED_Toggle(LED_Channel *LEDx){
	LEDx->STATE ^= LEDx->STATE;
	GPIO_TogglePin(&(LEDx->CHANNEL));
}

void vBlinkyLedTask( void *pvParameters ) {
	LED_Channel *LED = (LED_Channel *)pvParameters;

	for( ;; ) {
		LED_Toggle(LED);
		vTaskDelay(pdMS_TO_TICKS(LED->BLINK));
	}
}

