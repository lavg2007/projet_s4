
/**
  TMR2 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr2.c

  @Summary
    This is the generated source file for the TMR2 driver using PIC32MX MCUs

  @Description
    This source file provides APIs for driver for TMR2. 
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
#include "tmr2.h"
#include "../hardware.h"
#include "../demo1.h"
#include <math.h>
#include "../LUT_phase.h"
#include <stdio.h>
#include "../filterIIRcoeffs.h"
#include "../config.h"
/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/
static bool binTest;
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

static TMR_OBJ tmr2_obj;

/**
  Section: Driver Interface
*/


void TMR2_Initialize (void)
{
    // TMR2 0; 
    counter = 0;
    TMR2 = 0x0;
    // Period = 0.00025 s; Frequency = 8000000 Hz; PR2 2000; 
    PR2 = 0x7D0;
    // TCKPS 1:1; T32 16 Bit; TCS PBCLK; SIDL disabled; TGATE disabled; ON enabled; 
    T2CON = 0x8000;
    phase = 0;
    IFS0CLR= 1 << _IFS0_T2IF_POSITION;
    IEC0bits.T2IE = true;
//    IEC0bits.T2IE = false;
	flagCap = false;
    tmr2_obj.timerElapsed = false;
   
}
extern int32_t stabFreq;
extern int32_t stabAmp;


void __ISR(_TIMER_2_VECTOR, IPL1AUTO) _T2Interrupt (  )
{
      BIN1(1);
    //***User Area Begin
//    phase = (phase + LUT_phase[500]) % 32768;
    
    
    phase = phase + LUT_phase[stabFreq];
    if(phase >= SINE_RANGE)
        phase -= SINE_RANGE;
    if((prt_SWT_SWT6 == 1) && (prt_SWT_SWT7 == 1)){
        currentInBuffer[bufferCount] = LUT_sin_guit[phase]*0.6*stabAmp/1024;
    }else if((prt_SWT_SWT6 == 1) && (prt_SWT_SWT7 == 0)){
        currentInBuffer[bufferCount] = LUT_sin[phase] * stabAmp/1300;
    }else if((prt_SWT_SWT6 == 0) && (prt_SWT_SWT7 == 1)){
        currentInBuffer[bufferCount] = LUT_Tri[phase] * stabAmp/1300;
    }else{
        currentInBuffer[bufferCount] = LUT_sin_guit2[phase] * stabAmp/1300;
    }
    OC1_PWMPulseWidthSet(((currentInBuffer[bufferCount]+SINE_RANGE)*PR3)>>SINE_LOG);
    counter += 1;
    if(counter >= 200)
    {
        flagCap = true;
        counter = 0;
    }    
//    BIN1(0); 
//  
    if(bufferCount >= SIG_LEN)
    {    
        bufferCount = 0;
        swapBuffers = true;
    }   
    bufferCount += 1;
    
//    sprintf(serialTest,"%d",bufferCount);
    
//      UART_PutString(serialTest);
    BIN1(0);
    //***User Area End

    tmr2_obj.count++;
    tmr2_obj.timerElapsed = true;
    IFS0CLR= 1 << _IFS0_T2IF_POSITION;
}

void TMR2_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR2 = value;
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;
}

uint16_t TMR2_Period16BitGet( void )
{
    return( PR2 );
}

void TMR2_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR2 = value;
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;
}

uint16_t TMR2_Counter16BitGet( void )
{
    return( TMR2 );
}


void TMR2_Start( void )
{
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;

    IFS0CLR= 1 << _IFS0_T2IF_POSITION;
    /*Enable the interrupt*/
    IEC0bits.T2IE = true;

    /* Start the Timer */
    T2CONbits.ON = 1;
}

void TMR2_Stop( void )
{
    /* Stop the Timer */
    T2CONbits.ON = false;

    /*Disable the interrupt*/
    IEC0bits.T2IE = false;
}

bool TMR2_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr2_obj.timerElapsed;

    if(status == true)
    {
        tmr2_obj.timerElapsed = false;
    }
    return status;
}

int TMR2_SoftwareCounterGet(void)
{
    return tmr2_obj.count;
}

void TMR2_SoftwareCounterClear(void)
{
    tmr2_obj.count = 0; 
}

/**
 End of File
*/
