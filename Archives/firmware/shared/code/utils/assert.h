/*
 * assert.h
 *
 *  Created on: Feb 6, 2020
 *      Author: Poornachander
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef UNIT_TEST
#define assert(x)
#else
#include "FreeRTOS.h"
#include "task.h"
#define assert(x) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
#endif


#endif /* ASSERT_H_ */
