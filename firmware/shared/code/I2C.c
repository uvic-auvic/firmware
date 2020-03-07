/*
 *	I2CF4.c
 */

#include "I2C.h"

#include <stddef.h>

/* DEFINES */

/* TYPEDEFS */


/* PRIVATE DATA */

extern I2C_config_S I2C_config; 

/* PRIVATE FUNCTION DECLARATIONS */

/* PRIVATE FUNCTION DEFINITIONS */

/* PUBLIC FUNCTIONS */

/**
 * 
 * mode: normal use case is I2C_STANDARD
 */
void I2C_init()
{
	if(I2C_config.HWConfig == NULL)
		configASSERT(0U);

}
