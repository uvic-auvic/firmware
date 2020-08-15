/*
 * -------------------------------------------
 *    CC3220 SDK - v0.10.00.00 
 * -------------------------------------------
 *
 *  Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 */
//*****************************************************************************
//
//  pin.c
//
//  Mapping of peripherals to pins.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup pin_api
//! @{
//
//*****************************************************************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ocp_shared.h"
#include "pin.h"

//*****************************************************************************
// Macros
//*****************************************************************************
#define PAD_MODE_MASK		0x0000000F
#define PAD_STRENGTH_MASK	0x000000E0
#define PAD_TYPE_MASK		0x00000310
#define PAD_CONFIG_BASE		((OCP_SHARED_BASE + \
                                  OCP_SHARED_O_GPIO_PAD_CONFIG_0))

//*****************************************************************************
// PIN to PAD matrix
//*****************************************************************************
static const unsigned long g_ulPinToPadMap[64] =
{
	10,11,12,13,14,15,16,17,255,255,18,
	19,20,21,22,23,24,40,28,29,25,255,
	255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,
	31,255,255,255,255,0,255,32,30,255,1,
	255,2,3,4,5,6,7,8,9
};


//*****************************************************************************
//
//! Configures pin mux for the specified pin.
//!
//! \param ulPin is a valid pin.
//! \param ulPinMode is one of the valid mode
//!
//! This function configures the pin mux that selects the peripheral function
//! associated with a particular SOC pin. Only one peripheral function at a
//! time can be associated with a pin, and each peripheral function should
//! only be associated with a single pin at a time.
//!
//! \return none
//
//*****************************************************************************
void PinModeSet(unsigned long ulPin,unsigned long ulPinMode)
{

  unsigned long ulPad;

  //
  // Get the corresponding Pad
  //
  ulPad = g_ulPinToPadMap[ulPin & 0x3F];

  //
  // Calculate the register address
  //
  ulPad = ((ulPad << 2) + PAD_CONFIG_BASE);

  //
  // Set the mode.
  //
  HWREG(ulPad) = (((HWREG(ulPad) & ~PAD_MODE_MASK) |  ulPinMode) & ~(3<<10));

}

//*****************************************************************************
//
//! Gets current pin mux configuration of specified pin.
//!
//! \param ulPin is a valid pin.
//!
//! This function get the current configuration of the pin mux.
//!
//! \return Returns current pin mode if \e ulPin is valid, 0xFF otherwise.
//
//*****************************************************************************
unsigned long PinModeGet(unsigned long ulPin)
{

  unsigned long ulPad;


  //
  // Get the corresponding Pad
  //
  ulPad = g_ulPinToPadMap[ulPin & 0x3F];


  //
  // Calculate the register address
  //
  ulPad = ((ulPad << 2) + PAD_CONFIG_BASE) ;

  //
  // return the mode.
  //
  return (HWREG(ulPad) & PAD_MODE_MASK);

}

//*****************************************************************************
//
//! Sets the direction of the specified pin(s).
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinIO is the pin direction and/or mode.
//!
//! This function configures the specified pin(s) as either input only or
//! output only or it configures the pin to be under hardware control.
//!
//! The parameter \e ulPinIO is an enumerated data type that can be one of
//! the following values:
//!
//! - \b PIN_DIR_MODE_IN
//! - \b PIN_DIR_MODE_OUT
//! - \b PIN_DIR_MODE_HW
//!
//! where \b PIN_DIR_MODE_IN specifies that the pin is programmed as a
//! input only, \b PIN_DIR_MODE_OUT specifies that the pin is
//! programmed output only, and \b PIN_DIR_MODE_HW specifies that the pin is
//! placed under hardware control.
//!
//!
//! \return None.
//
//*****************************************************************************
void PinDirModeSet(unsigned long ulPin, unsigned long ulPinIO)
{
  unsigned long ulPad;

  //
  // Get the corresponding Pad
  //
  ulPad = g_ulPinToPadMap[ulPin & 0x3F];

  //
  // Calculate the register address
  //
  ulPad = ((ulPad << 2) + PAD_CONFIG_BASE);

  //
  // Set the direction
  //
  HWREG(ulPad) = ((HWREG(ulPad) & ~0xC00) | ulPinIO);
}

//*****************************************************************************
//
//! Gets the direction of a pin.
//!
//! \param ulPin is one of the valid pin.
//!
//! This function gets the direction and control mode for a specified pin on
//! the selected GPIO port.  The pin can be configured as either an input only
//! or output only, or it can be under hardware control.  The type of control
//! and direction are returned as an enumerated data type.
//!
//! \return Returns one of the enumerated data types described for
//! GPIODirModeSet().
//
//*****************************************************************************
unsigned long PinDirModeGet(unsigned long ulPin)
{
  unsigned long ulPad;

  //
  // Get the corresponding Pad
  //
  ulPad = g_ulPinToPadMap[ulPin & 0x3F];

  //
  // Calculate the register address
  //
  ulPad = ((ulPad << 2) + PAD_CONFIG_BASE);

  //
  // Return the direction
  //
  return ((HWREG(ulPad) & 0xC00));
}

//*****************************************************************************
//
//! Gets Pin output drive strength and Type
//!
//! \param ulPin is one of the valid pin
//! \param pulPinStrength is pointer to storage for output drive strength
//! \param pulPinType is pinter to storage for pin type
//!
//! This function gets the pin type and output drive strength for the pin
//! specified by \e ulPin parameter. Parameters \e pulPinStrength and
//! \e pulPinType corresponds to the values used in PinConfigSet().
//!
//!
//! \return None.
//
//*****************************************************************************
void PinConfigGet(unsigned long ulPin,unsigned long  *pulPinStrength,
	       					unsigned long *pulPinType)
{

  unsigned long ulPad;


  //
  // Get the corresponding Pad
  //
  ulPad = g_ulPinToPadMap[ulPin & 0x3F];


  //
  // Calculate the register address
  //
  ulPad = ((ulPad << 2) + PAD_CONFIG_BASE);


  //
  // Get the type
  //
  *pulPinType = (HWREG(ulPad) & PAD_TYPE_MASK);

  //
  // Get the output drive strength
  //
  *pulPinStrength = (HWREG(ulPad) & PAD_STRENGTH_MASK);

}

//*****************************************************************************
//
//! Configure Pin output drive strength and Type
//!
//! \param ulPin is one of the valid pin
//! \param ulPinStrength is logical OR of valid output drive strengths.
//! \param ulPinType is one of the valid pin type.
//!
//!  This function sets the pin type and strength for the pin specified by
//! \e ulPin parameter.
//!
//! The parameter \e ulPinStrength should be one of the following
//! - \b PIN_STRENGTH_2MA
//! - \b PIN_STRENGTH_4MA
//! - \b PIN_STRENGTH_6MA
//!
//!
//! The parameter \e ulPinType should be one of the following
//! For standard type
//!
//! - \b PIN_TYPE_STD
//! - \b PIN_TYPE_STD_PU
//! - \b PIN_TYPE_STD_PD
//!
//! And for Open drain type
//!
//! - \b PIN_TYPE_OD
//! - \b PIN_TYPE_OD_PU
//! - \b PIN_TYPE_OD_PD
//!
//! \return None.
//
//*****************************************************************************
void PinConfigSet(unsigned long ulPin,unsigned long  ulPinStrength,
						unsigned long ulPinType)
{

  unsigned long ulPad;

  //
  // Get the corresponding Pad
  //
  ulPad = g_ulPinToPadMap[ulPin & 0x3F];

  //
  // Write the register
  //
  if(ulPinType == PIN_TYPE_ANALOG)
  {
    //
    // Isolate the input
    //
    HWREG(0x4402E144) |= ((0x80 << ulPad) & (0x1E << 8));

    //
    // Calculate the register address
    //
    ulPad = ((ulPad << 2) + PAD_CONFIG_BASE);

    //
    // Isolate the output
    //
    HWREG(ulPad) = 0xC00;

  }
  else
  {
    //
    // Enable the input
    //
    HWREG(0x4402E144) &= ~((0x80 << ulPad) & (0x1E << 8));

    //
    // Calculate the register address
    //
    ulPad = ((ulPad << 2) + PAD_CONFIG_BASE);

    //
    // Write the configuration
    //
    HWREG(ulPad) = ((HWREG(ulPad) & ~(PAD_STRENGTH_MASK | PAD_TYPE_MASK)) |
		  		(ulPinStrength | ulPinType ));
  }


}

//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by UART peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The UART pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin(s); other configurations may work as well depending upon the
//! board setup (for example, using the on-chip pull-ups).
//!
//!
//! \note This function cannot be used to turn any pin into a UART pin; it
//! only sets the pin mode and configures it for proper UART operation.
//!
//!
//! \return None.
//
//*****************************************************************************
void PinTypeUART(unsigned long ulPin,unsigned long ulPinMode)
{
    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin,ulPinMode);

    //
    // Set the pin for standard operation
    //
    PinConfigSet(ulPin,PIN_STRENGTH_2MA,PIN_TYPE_STD);
}


//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by I2C peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The I2C pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! the pin.
//!
//!
//! \note This function cannot be used to turn any pin into a I2C pin; it
//! only sets the pin mode and configures it for proper I2C operation.
//!
//!
//! \return None.
//
//*****************************************************************************
void PinTypeI2C(unsigned long ulPin,unsigned long ulPinMode)
{
    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin,ulPinMode);

    //
    // Set the pin for open-drain operation with a weak pull-up.
    //
    PinConfigSet(ulPin,PIN_STRENGTH_2MA,PIN_TYPE_OD_PU);
}


//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by SPI peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The SPI pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin.
//!
//!
//! \note This function cannot be used to turn any pin into a SPI pin; it
//! only sets the pin mode and configures it for proper SPI operation.
//!
//!
//! \return None.
//
//*****************************************************************************
void PinTypeSPI(unsigned long ulPin,unsigned long ulPinMode)
{

    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin,ulPinMode);

    //
    // Set the pin for standard operation
    //
    PinConfigSet(ulPin,PIN_STRENGTH_2MA|PIN_STRENGTH_4MA,PIN_TYPE_STD);

}


//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by I2S peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The I2S pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin.
//!
//!
//! \note This function cannot be used to turn any pin into a I2S pin; it
//! only sets the pin mode and configures it for proper I2S operation.
//!
//! \return None.
//
//*****************************************************************************
void PinTypeI2S(unsigned long ulPin,unsigned long ulPinMode)
{

    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin,ulPinMode);

    //
    // Set the pin for standard operation
    //
    PinConfigSet(ulPin,PIN_STRENGTH_2MA|PIN_STRENGTH_4MA,PIN_TYPE_STD);

}


//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by Timer peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The timer PWM pins must be properly configured for the Timer peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin; other configurations may work as well depending upon the
//! board setup (for example, using the on-chip pull-ups).
//!
//!
//! \note This function cannot be used to turn any pin into a timer PWM pin; it
//! only sets the pin mode and configures it for proper timer PWM operation.
//!
//! \return None.
//
//*****************************************************************************
void PinTypeTimer(unsigned long ulPin,unsigned long ulPinMode)
{

    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin,ulPinMode);

    //
    // Set the pin for standard operation
    //
    PinConfigSet(ulPin,PIN_STRENGTH_2MA|PIN_STRENGTH_4MA,PIN_TYPE_STD);
}


//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by Camera peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The Camera pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin.
//!
//!
//! \note This function cannot be used to turn any pin into a Camera pin; it
//! only sets the pin mode and configures it for proper Camera operation.
//!
//! \return None.
//
//*****************************************************************************
void PinTypeCamera(unsigned long ulPin,unsigned long ulPinMode)
{

    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin,ulPinMode);

    //
    // Set the pin for standard operation
    //
    PinConfigSet(ulPin,PIN_STRENGTH_2MA|PIN_STRENGTH_4MA,PIN_TYPE_STD);

}


//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by GPIO peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//! \param bOpenDrain is one to decide either OpenDrain or STD
//!
//! The GPIO pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin.
//!
//!
//! \return None.
//
//*****************************************************************************
void PinTypeGPIO(unsigned long ulPin,unsigned long ulPinMode,tBoolean bOpenDrain)
{

    //
    // Set the pin for standard push-pull operation.
    //
    if(bOpenDrain)
    {
            PinConfigSet(ulPin, PIN_STRENGTH_2MA, PIN_TYPE_OD);
    }
    else
    {
            PinConfigSet(ulPin, PIN_STRENGTH_2MA, PIN_TYPE_STD);
    }

    //
    // Set the pin to specified mode
    //
    PinModeSet(ulPin, ulPinMode);

}

//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by ADC
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The ADC pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin.
//!
//!
//! \note This function cannot be used to turn any pin into a ADC pin; it
//! only sets the pin mode and configures it for proper ADC operation.
//!
//! \return None.
//
//*****************************************************************************
void PinTypeADC(unsigned long ulPin,unsigned long ulPinMode)
{
  //
  // Configure the Pin
  //
  PinConfigSet(ulPin,PIN_STRENGTH_2MA,PIN_TYPE_ANALOG);
}

//*****************************************************************************
//
//! Sets the pin mode and configures the pin for use by SD Host peripheral
//!
//! \param ulPin is one of the valid pin.
//! \param ulPinMode is one of the valid pin mode.
//!
//! The MMC pins must be properly configured for the peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin.
//!
//!
//! \note This function cannot be used to turn any pin into a SD Host pin; it
//! only sets the pin mode and configures it for proper SD Host operation.
//!
//! \return None.
//
//*****************************************************************************
void PinTypeSDHost(unsigned long ulPin,unsigned long ulPinMode)
{
  //
  // Set pin mode
  //
  PinModeSet(ulPin,ulPinMode);

  //
  // Configure the Pin
  //
  PinConfigSet(ulPin,PIN_STRENGTH_2MA,PIN_TYPE_STD);

}


//*****************************************************************************
//
//! Sets the hysteresis for all the pins
//!
//! \param ulHysteresis is one of the valid predefined hysterisys values
//!
//! This function sets the hysteresis vlaue for all the pins. The parameter
//! \e ulHysteresis can be on one the following:
//! -\b PIN_HYSTERESIS_OFF - To turn Off hysteresis, default on POR
//! -\b PIN_HYSTERESIS_10  - To turn On hysteresis, 10%
//! -\b PIN_HYSTERESIS_20  - To turn On hysteresis, 20%
//! -\b PIN_HYSTERESIS_30  - To turn On hysteresis, 30%
//! -\b PIN_HYSTERESIS_40  - To turn On hysteresis, 40%
//!
//! \return None.
//
//*****************************************************************************
void PinHysteresisSet(unsigned long ulHysteresis)
{
  unsigned long ulRegValue;

  //
  // Read the current value
  //
  ulRegValue =  (HWREG( OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CMN_CONFIG )
                & ~(0x0000001C));

  //
  // Set the new Hysteresis
  //
  if( ulHysteresis != PIN_HYSTERESIS_OFF )
  {
      ulRegValue |= (ulHysteresis & 0x0000001C);
  }

  //
  // Write the new value
  //
  HWREG( OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CMN_CONFIG ) = ulRegValue;
}

//*****************************************************************************
//
//! Sets the level of the pin when locked
//!
//! \param ulPin is one of the valid pin.
//! \param ucLevel is the level the pin drives when locked
//!
//! This function sets the pin level when the pin is locked using
//! \sa PinLock() API.
//!
//! By default all pins are set to drive 0.
//!
//! \note Use case is to park the pins when entering LPDS
//!
//! \return None.
//
//*****************************************************************************
void PinLockLevelSet(unsigned long ulPin, unsigned char ucLevel)
{
  unsigned long ulPad;

  //
  // Supported only in ES2.00 and Later devices i.e. ROM Version 2.x.x or greater
  //
  if( (HWREG(0x00000400) & 0xFFFF) >= 2 )
  {
    //
    // Get the corresponding Pad
    //
    ulPad = g_ulPinToPadMap[ulPin & 0x3F];

    //
    // Get the required bit
    //
    ulPad = 1 << ulPad;

    if(ucLevel)
    {
      HWREG( OCP_SHARED_BASE + OCP_SHARED_O_SPARE_REG_6 ) |= ulPad;
    }
    else
    {
      HWREG( OCP_SHARED_BASE + OCP_SHARED_O_SPARE_REG_6 ) &= ~ulPad;
    }
  }
}

//*****************************************************************************
//
//! Locks all the pins to configured level(s).
//!
//! \param ulOutEnable the bit-packed representation of pins to be set as output
//!
//! This function locks all the pins to the pre-configure level. By default
//! the pins are set to drive 0. Default level can be changed using
//! \sa PinLockLevelSet() API.
//!
//! The \e ulOutEnable paramter is bit-packed representation of pins that
//! are required to be enabled as output. If a bit is set 1, the corresponding
//! pin (as shown below) are set  and locked as output.
//!
//!    |------|-----------------------------------------------|
//!    |  Bit |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
//!    |------|-----------------------------------------------|
//!    |  Pin |xx|xx|20|19|30|29|21|17|16|15|14|13|12|11|08|07|
//!    |------|-----------------------------------------------|
//!
//!    |------|-----------------------------------------------|
//!    |  Bit |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
//!    |------|-----------------------------------------------|
//!    |  Pin |06|05|04|03|02|01|64|63|62|61|60|59|58|57|55|50|
//!    |------|-----------------------------------------------|
//!
//!
//! \note Use case is to park the pins when entering LPDS
//!
//! \return None.
//
//*****************************************************************************
void PinLock(unsigned long ulOutEnable)
{
  //
  // Supported only in ES2.00 and Later devices i.e. ROM Version 2.x.x or greater
  //
  if( (HWREG(0x00000400) & 0xFFFF) >= 2 )
  {
    //
    // Enable/disable the pin(s) output
    //
    HWREG( OCP_SHARED_BASE + OCP_SHARED_O_SPARE_REG_7 ) = ~ulOutEnable;

    //
    // Lock the pins to selected levels
    //
    HWREG( OCP_SHARED_BASE + OCP_SHARED_O_SPARE_REG_5 ) |= (3 << 24);
  }
}

//*****************************************************************************
//
//! Unlocks all the pins.
//!
//! This function unlocks all the pins and can be used for peripheral function.
//!
//! By default all the pins are in unlocked state.
//!
//! \note Use case is to un-park the pins when exiting LPDS
//!
//! \return None.
//
//*****************************************************************************
void PinUnlock()
{
  //
  // Supported only in ES2.00 and Later devices i.e. ROM Version 2.x.x or greater
  //
  if( (HWREG(0x00000400) & 0xFFFF) >= 2 )
  {
    //
    // Unlock the pins
    //
    HWREG( OCP_SHARED_BASE + OCP_SHARED_O_SPARE_REG_5 ) &= ~(3 << 24);
  }
}

//*****************************************************************************
//
// Gets pad number from pin number
//
// \param ulPin is a valid pin number
//
// This function return the pad corresponding to the specified pin
//
// \return Pad number on success, 0xFF otherwise
//
//*****************************************************************************
unsigned long PinToPadGet(unsigned long ulPin)
{
	//
    // Return the corresponding Pad
    //
    return g_ulPinToPadMap[ulPin & 0x3F];
}


//*****************************************************************************
//
// Gets pin number from pad number
//
// \param ulPad is a valid pad number
//
// This function return the pin corresponding to the specified pad
//
// \return Pin number on success, 0xFF otherwise
//
//*****************************************************************************
unsigned long PinFromPadGet(unsigned long ulPad)
{
	unsigned long ulPin;
	
	//
	// search and return the pin number
	//
	for(ulPin=0; ulPin < sizeof(g_ulPinToPadMap)/4; ulPin++)
	{
		if(g_ulPinToPadMap[ulPin] == ulPad)
		{
			return ulPin;
		}
	}
	
	return 0xFF;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
