/*
 * UART_componentSpecific.h
 *
 *  Created on: Jul 23, 2019
 *      Author: Poornachander
 */

#ifndef MOTORCONTROLLER2_CODE_APP_UART_COMPONENTSPECIFIC_H_
#define MOTORCONTROLLER2_CODE_APP_UART_COMPONENTSPECIFIC_H_

#include "userDefines.h"
#include "stm32f4xx.h"

#if USE_PROTOBUFS
#include "POLARIS.pb.h"
#else
#include "protocol.h"
#endif

#define UART_TX_BUFFER_SIZE	(80U)

#define UART_RX_BUFFER_LENGTH (10U)
#define UART_RX_BUFFER_COUNT  (8U)

#if USE_PROTOBUFS
#define UART_TO_BOARD_MESSAGE_TYPE		POLARIS_allMessages_M
#define UART_TO_BOARD_MESSAGE_FIELDS	POLARIS_allMessages_M_fields
#else
#define UART_TO_BOARD_MESSAGE_TYPE		protocol_message_S
#endif

#endif /* MOTORCONTROLLER2_CODE_APP_UART_COMPONENTSPECIFIC_H_ */
