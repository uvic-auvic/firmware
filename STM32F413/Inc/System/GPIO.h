#ifndef SYSTEM_GPIO_H_
#define SYSTEM_GPIO_H_

#include <stm32f413xx.h>
#include <stdint.h>
#include <assert.h>

#include "System/RCC.h"

// TODO: Implement Pin Locking
// TODO: Use Variadic function to apply default arguments. 


#define MODER(Pin, Cfg) (uint32_t)(Cfg << (Pin * 2))
#define OTYPER(Pin, Cfg) (uint32_t)(Cfg << (Pin * 1))
#define PUPDR(Pin, Cfg) (uint32_t)(Cfg << (Pin * 2))
#define OSPEEDR(Pin, Cfg) (uint32_t)(Cfg << (Pin * 2))
#define AF(Pin, Cfg) (uint32_t)(Cfg << (Pin * 4))

#define BSRR_RESET_OFFSET (16U)

typedef enum {
	GPIO_RESET = 1,
	GPIO_SET = !GPIO_RESET,
	GPIO_STATE_COUNT,
} GPIO_State;

typedef enum {
	P0 = 0U,
	P1,
	P2,
	P3,
	P4,
	P5,
	P6,
	P7,
	P8,
	P9,
	P10,
	P11,
	P12,
	P13,
	P14,
	P15,
} GPIO_Pin;

typedef enum {
	AF0 = 0x0U,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15,
} GPIO_AF;

typedef enum {
	GPIO_MODER_INPUT = 0U,
	GPIO_MODER_OUTPUT,
	GPIO_MODER_AF,
	GPIO_MODER_ANALOG,
	GPIO_MODER_COUNT,
} GPIO_MODER;

typedef enum {
	GPIO_OTYPER_PP = 0U, // Push Pull
	GPIO_OTYPER_OD, // Open Drain
	GPIO_OTYPER_COUNT,
} GPIO_OTYPER;

typedef enum {
	GPIO_OSPEEDR_LOW = 0U,
	GPIO_OSPEEDR_MED,
	GPIO_OSPEEDR_FAST,
	GPIO_OSPEEDR_HIGH,
	GPIO_OSPEEDR_COUNT,
} GPIO_OSPEEDR;

typedef enum {
	GPIO_PUDPR_NOPUPD = 0U,
	GPIO_PUDPR_PU,
	GPIO_PUDPR_PD,
	GPIO_PUDPR_RESERVED,
	GPIO_PUDPR_COUNT,
} GPIO_PUPDR;

typedef struct __attribute__((__packed__)) {
	GPIO_Pin PIN;
	GPIO_TypeDef* GPIOx;
} GPIO_Channel;

/***************************************************************************//**
 * Initializes the GPIO Channel
 * @param Pin The Pin to be configured. See coressponding enum
 * @param GPIOx The address of Registers. See <stm32f413xx.h> for entries
 * @param MODER either 0, 1, 2, or 3
 * @param OTYPER either 1 or 2
 * @param PUPDR
 * @param OSPEEDR
 * @param AF
 ******************************************************************************/
GPIO_Channel GPIO_Init(GPIO_Pin Pin, GPIO_TypeDef* GPIOx, uint32_t MODER,
		uint32_t PUPDR, uint32_t OSPEEDR, uint32_t AF, uint16_t OTYPER);

GPIO_State GPIO_ReadPin(GPIO_Channel* Channel);
void GPIO_WritePin(GPIO_Channel* Channel, GPIO_State State);
void GPIO_TogglePin(GPIO_Channel* Channel);

#endif /* SYSTEM_GPIO_H_ */
