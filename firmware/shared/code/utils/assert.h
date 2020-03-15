/*
 * assert.h
 *
 *  Created on: Feb 6, 2020
 *      Author: Poornachander
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#include "FreeRTOS.h"
#include "task.h"

#define assert(x) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	

#endif /* ASSERT_H_ */
