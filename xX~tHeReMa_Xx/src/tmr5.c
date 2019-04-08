
/**
  TMR5 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr5.c

  @Summary
    This is the generated source file for the TMR5 driver using PIC32MX MCUs

  @Description
    This source file provides APIs for driver for TMR5. 
    Generation Information : 
        Product Revision  :  PIC32MX MCUs - pic32mx : v1.35
        Device            :  PIC32MX370F512L
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB 	          :  MPLAB X 3.55
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdlib.h>
#include "tmr5.h"
#include <math.h>
#include "main.h"
#include "../test_IP_Harmony.X/LUT_phase.h"
#include <stdio.h>
#include "oc1.h"
#include "pin_manager.h"
#include "led.h"

/**
  Section: Data Type Definitions
*/
#define SINE_RANGE 2048
#define SINE_LOG 12

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/
static int counter;
int testBuffer[1024];
int phase;
bool flagCap;

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    bool                                                    timerElapsed;
    /*Software Counter value*/
    uint8_t                                                 count;

} TMR_OBJ;

static TMR_OBJ tmr5_obj;

/**
  Section: Driver Interface
*/


void TMR5_Initialize (void)
{
    // TMR5 0; 
    counter = 0;
    TMR5 = 0x0;
    // Period = 0.0000104167 s; Frequency = 192000000 Hz; PR5 2001; 
    PR5 = 0x3E8;
    // TCKPS 1:1; T32 16 Bit; TCS PBCLK; SIDL disabled; TGATE disabled; ON enabled; 
    T5CON = 0x8000;
    phase = 0;
    IFS0CLR= 1 << _IFS0_T5IF_POSITION;
    IEC0bits.T5IE = true;
//    IEC0bits.T5IE = false;
	flagCap = false;
    tmr5_obj.timerElapsed = false;
   
}


void __ISR(_TIMER_5_VECTOR, IPL1AUTO) _T5Interrupt (  )
{
    phase = phase + LUT_phase[stabFreq];
    if(phase >= SINE_RANGE)
        phase -= SINE_RANGE;
    if((prt_SWT_SWT5 == 1) && (prt_SWT_SWT6 == 1)&& (prt_SWT_SWT7 == 1)){
        currentInBuffer[bufferCount] = LUT_sin_guit[phase]*0.6*stabAmp/1024;
    }else if((prt_SWT_SWT5 == 0) && (prt_SWT_SWT6 == 1)&& (prt_SWT_SWT7 == 1)){
        currentInBuffer[bufferCount] = LUT_sin[phase] * stabAmp/1300;
    }else if((prt_SWT_SWT5 == 1) && (prt_SWT_SWT6 == 0)&& (prt_SWT_SWT7 == 1)){
        currentInBuffer[bufferCount] = LUT_Tri[phase] * stabAmp/1300;
    }else if((prt_SWT_SWT5 == 0) && (prt_SWT_SWT6 == 0)&& (prt_SWT_SWT7 == 1)){
        currentInBuffer[bufferCount] = LUT_sin_guit2[phase] * stabAmp/1300;
    }else if((prt_SWT_SWT5 == 1) && (prt_SWT_SWT6 == 1)&& (prt_SWT_SWT7 == 0)){
        currentInBuffer[bufferCount] = LUT_square[phase] * stabAmp/1300;
    }else if((prt_SWT_SWT5 == 0) && (prt_SWT_SWT6 == 1)&& (prt_SWT_SWT7 == 0)){
//        currentInBuffer[bufferCount] = LUT_sin_guit3[phase] * stabAmp/1300;
    }else{
        currentInBuffer[bufferCount] = LUT_sin_sax[phase] * stabAmp/1300;   
    }
    OC1_PWMPulseWidthSet(((currentInBuffer[bufferCount]+SINE_RANGE)*PR3)>>SINE_LOG);
    counter += 1;
    if(counter >= 200)
    {
        flagCap = true;
        counter = 0;
    }    

    if(bufferCount >= SIG_LEN)
    {    
        bufferCount = 0;
        swapBuffers = true;
    }   
    bufferCount += 1;

    tmr5_obj.timerElapsed = true;
    IFS0CLR= 1 << _IFS0_T5IF_POSITION;
}

void TMR5_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR5 = value;
    /* Reset the status information */
    tmr5_obj.timerElapsed = false;
}

uint16_t TMR5_Period16BitGet( void )
{
    return( PR5 );
}

void TMR5_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR5 = value;
    /* Reset the status information */
    tmr5_obj.timerElapsed = false;
}

uint16_t TMR5_Counter16BitGet( void )
{
    return( TMR5 );
}


void TMR5_Start( void )
{
    /* Reset the status information */
    tmr5_obj.timerElapsed = false;

    IFS0CLR= 1 << _IFS0_T5IF_POSITION;
    /*Enable the interrupt*/
    IEC0bits.T5IE = true;

    /* Start the Timer */
    T5CONbits.ON = 1;
}

void TMR5_Stop( void )
{
    /* Stop the Timer */
    T5CONbits.ON = false;

    /*Disable the interrupt*/
    IEC0bits.T5IE = false;
}

bool TMR5_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr5_obj.timerElapsed;

    if(status == true)
    {
        tmr5_obj.timerElapsed = false;
    }
    return status;
}

int TMR5_SoftwareCounterGet(void)
{
    return tmr5_obj.count;
}

void TMR5_SoftwareCounterClear(void)
{
    tmr5_obj.count = 0; 
}

/**
 End of File
*/
