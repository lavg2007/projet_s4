/* ************************************************************************** */
/** Descriptive File Name: swDelay.h - header file for software delay 
                           implemented by polling the core timer.

  @ Author
    Richard Wall
 
  @ Date
    Created:    May 28, 2016
    Verified:   May 19, 2017
  
  @Company
    Digilent Inc

  @File Name
    swDelay.h

  @Development Environment
    MPLAB X IDE x3.61 - http://www.microchip.com/mplab/mplab-x-ide 
    XC32 1.43 - http://www.microchip.com/mplab/compilers
    PLIB 3/7/20162 - http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library

  @Summary
    Millisecond, microsecond, 10 nanosecond delays using software polling of 
    the core timer

  @Description
    Provides interface to a millisecond and microsecond delay based on the 
    speed of the core timer.
 
  @Precondition
    "config_bits* must be included in the project that establishes the
    the value for CORE_MS_TICK_RATE based on the core timer frequency.

 **************************************************************************** */

#ifndef _SW_DELAY_H    // Guard against multiple inclusion 
#define _SW_DELAY_H

// Peripherals library include files (depricated!))
#define _SUPPRESS_PLIB_WARNING
#include "peripheral/timer.h"

// Clock defines
#define GetSystemClock()     (80000000UL)
#define GetCoreClock()       (GetSystemClock()/2)
#define CORE_MS_TICK_RATE	 (unsigned int) (GetCoreClock()/1000UL)

/*  Millisecond software blocking delay function */
void msDelay(unsigned int mS);

/*  Microsecond software blocking delay function */
void usDelay(unsigned int uS);

/*  10 Nanosecond software blocking delay function */
void nsDelay(unsigned int ns);

#endif /* _SW_DELAY_H */

/* *****************************************************************************
  End of swDelay.h
 */
