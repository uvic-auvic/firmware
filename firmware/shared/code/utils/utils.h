/*
 * utils.h
 *
 *  Created on: Feb 6, 2020
 *      Author: Poornachander
 */

#ifndef UTILS_H_
#define UTILS_H_

// TIME
#define MS_IN_SEC   (1000U)  // Milliseconds in one second
#define US_IN_SEC   (1000000U) // Microseconds in one second

// Use to suppress errors when a parameter in a function is not being used. 
#define UNUSED(x)   ((void)(x))

// MIN MAX

// Returns the lesser of the two values, x and y
#define MIN_OF(x, y)      ( (x) < (y) ? x : y )

// Returns the greater of the two value, x and y
#define MAX_OF(x, y)      ( (x) > (y) ? x : y )

// BIT MANIPULATION

// Returns the a 1 in the bit position specificied by x
// Example: If x = 5, return value is 0x00000020 (0b00000000000000000000000000100000)
#define BITVALUE(x) ( 1U << (x))

// Returns the maximum value that can be held in x bits
// Example: If x = 8, return value is 0xFF (255)
#define MAXVALUE(x) (BITVALUE(x) - 1)

#endif /* UTILS_H_ */
