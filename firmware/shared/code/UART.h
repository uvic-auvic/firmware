/*
 * UART.h
 */

#ifndef UART_H_
#define UART_H_

#if DEVICE_FAMILY_STM32F4 == 1
#include "UARTF4.h"
#elif DEVICE_FAMILY_STM32F0 == 1
#include "UARTF0.h"
#endif

#endif /* UART_H_ */
