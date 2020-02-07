/*
 * circBuffer1D_componentSpecific.h
 *
 *  Created on: Jul 22, 2019
 *      Author: Poornachander
 */

#ifndef MOTORCONTROLLER2_CODE_APP_CIRCBUFFER1D_COMPONENTSPECIFIC_H_
#define MOTORCONTROLLER2_CODE_APP_CIRCBUFFER1D_COMPONENTSPECIFIC_H_

#define CIRCBUFFER1D_TOTAL_BUFFER_SIZE	(200U)

typedef enum
{
	CIRCBUFFER1D_CHANNEL_UART_TX,
	CIRCBUFFER1D_CHANNEL_UART_RX,

	CIRCBUFFER1D_CHANNEL_COUNT
} circBuffer1D_channel_E;

#endif /* MOTORCONTROLLER2_CODE_APP_CIRCBUFFER1D_COMPONENTSPECIFIC_H_ */
