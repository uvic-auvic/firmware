/*
 * ISOTP_UART.h
 */

#ifndef ISOTP_UART_H_
#define ISOTP_UART_H_

#include <stdint.h>

void ISOTP_UART_init(void);
void ISOTP_UART_run1ms(void);
void ISOTP_UART_messageReceivedCallback(uint8_t * message, const uint8_t length);

#endif /* ISOTP_UART_H_ */
