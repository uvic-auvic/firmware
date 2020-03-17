/*
 * assert.h
 *
 *  Created on: Feb 6, 2020
 *      Author: Poornachander
 */

#ifndef ASSERT_H_
#define ASSERT_H_


#if USE_FREE_RTOS_ASSERT
#include "FreeRTOS.h"
#include "task.h"
#define assert(x) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
#else
#define assert(x) if( ( x ) == 0) { for( ;; ); }
#endif

#endif /* ASSERT_H_ */
