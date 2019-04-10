
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
#include "../Theremax.X/LUT_phase.h"
#include <stdio.h>
#include "oc1.h"
#include "pin_manager.h"
#include "led.h"
#include "UDP_app.h"

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
extern int countSound;
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
    countSound = 0;
    flagSound = true;
    instr = 0;
    flagCap = false;
    phase = 0;
    
    TMR5 = 0x0;
    // Period = 0.0000104167 s; Frequency = 192000000 Hz; PR5 2001; 
//    PR5 = 10000; //bon je suis pas fier, mais avec cette frequence la, tout roule
    // et les glichs sont presquent absent
    PR5 = 1000;
    // TCKPS 1:1; T32 16 Bit; TCS PBCLK; SIDL disabled; TGATE disabled; ON enabled; 
    T5CON = 0x8000;
    IFS0CLR= 1 << _IFS0_T5IF_POSITION;
    IEC0bits.T5IE = true;
//    IEC0bits.T5IE = false
    tmr5_obj.timerElapsed = false;
   
}


void __ISR(_TIMER_5_VECTOR, IPL1AUTO) _T5Interrupt (  )
{
    int wtf;
    BIN1(1);
    phase = phase + LUT_phase[stabFreq];
//    stabAmp = 1300;
    if(phase >= SINE_RANGE)
        phase -= SINE_RANGE;
    switch (instr)
    {
        case 0:
            currentInBuffer[bufferCount] = LUT_sin[phase] * stabAmp/1300 ;   
//            currentInBuffer[bufferCount] = 0;   
            break;
        case 1:
            currentInBuffer[bufferCount] = LUT_square[phase] * stabAmp/1300;
            
            break;
        case 2:
            currentInBuffer[bufferCount] = LUT_Tri[phase] * stabAmp/1300;
            break;
        case 3:
            currentInBuffer[bufferCount] = LUT_sin_guit2[phase] * stabAmp/1300;
            break;
        case 4:
            currentInBuffer[bufferCount] = LUT_sin_guit[phase]*0.6*stabAmp/1300;
            break;
        case 5 :   
            currentInBuffer[bufferCount] = LUT_sin_sax[phase] * stabAmp/1300;
            break;
    }
    
//    if(flagSound){
        OC1_PWMPulseWidthSet(((previousUDPBuffer[countSound]+SINE_RANGE)*PR3)>>SINE_LOG);
        countSound++;
//    }
    if(countSound >= SIG_LEN){
//        BIN1(1);
        if (currentUDPBuffer == UDP_Receive_Buffer1){
            previousUDPBuffer = UDP_Receive_Buffer1;
            currentUDPBuffer = UDP_Receive_Buffer2;
        }else {
            currentUDPBuffer = UDP_Receive_Buffer1;
            previousUDPBuffer = UDP_Receive_Buffer2;
//            SYS_CONSOLE_PRINT("%2\r\n");
        }        

        countSound = 0;
//        BIN1(0);
    }
    
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
    counter += 1;
    tmr5_obj.timerElapsed = true;
    IFS0CLR= 1 << _IFS0_T5IF_POSITION;
    BIN1(0);

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
