#ifndef __LED_H_
#define __LED_H_

#include "System/GPIO.h"

typedef enum {
	LED_RESET = 0,
	LED_SET = !LED_RESET,
	LED_STATE_COUNT,
} LED_State;

typedef enum {
    LED_RED,
    LED_GREEN,
    LED_BLUE,
	LED_ORANGE,
	LED_YELLOW,
	LED_YELLOW_GREEN,
	LED_NAN,
    LED_COLOR_COUNT,
} LED_Color;

typedef enum {
	BLINK_OFF = 0,
	BLINK_SLOW,
	BLINK_NORMAL,
	BLINK_FAST,
	BLINK_COUNT,
} LED_Blink;

typedef struct __attribute__((__packed__)){
	LED_Color LED;
	LED_State STATE;
	LED_Blink BLINK;
	GPIO_Channel CHANNEL;
} LED_Channel;

LED_Channel LED_Init(GPIO_Pin PIN, GPIO_TypeDef* GPIOx, LED_Color LED, LED_Blink BLINK);

extern void LED_On(LED_Channel *LEDx);
extern void LED_Off(LED_Channel *LEDx);
extern void LED_Toggle(LED_Channel *LEDx);

void set_Blink_Off(LED_Channel *LEDx);
void set_Blink_Slow(LED_Channel *LEDx);
void set_Blink_Normal(LED_Channel *LEDx);
void set_Blink_Fast(LED_Channel *LEDx);

// Generic RTOS Task (Node) that contains the LED


#endif /*__LED_H_*/
