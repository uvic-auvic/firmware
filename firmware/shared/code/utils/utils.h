/*
 * utils.h
 *
 *  Created on: Feb 6, 2020
 *      Author: Poornachander
 */

#ifndef UTILS_H_
#define UTILS_H_

#define BITVALUE(x) ( 1U << (x))

// TIME
#define MS_IN_SEC   (1000U)  // Milliseconds in one second
#define US_IN_SEC   (1000000U) // Microseconds in one second

#define UNUSED(x)   ((void)(x))

#define MIN_OF(x, y)      ( (x) < (y) ? x : y )
#define MAX_OF(x, y)      ( (x) > (y) ? x : y )  

#endif /* UTILS_H_ */
