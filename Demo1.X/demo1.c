/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC32MX MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC32MX MCUs - pic32mx : v1.35
        Device            :  PIC32MX370F512L
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB             :  MPLAB X 3.55
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

#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "demo1.h"
#include "hardware.h"
#include "swDelay.h"
#include "led.h"

/*
                         Main application
 */
const unsigned int freqSampling = 20000;
const unsigned int freqTest = 1000;
int32_t *currentInBuffer, *currentOutBuffer, bufferCount;
bool swapBuffers;
float phase, sinOut;
int main(void)
{
    int32_t *previousInBuffer, *previousOutBuffer;
    static int32_t inBuffer1[SIG_LEN], inBuffer2[SIG_LEN], outBuffer1[SIG_LEN], outBuffer2[SIG_LEN];
    // initialize the device
    phase = 0;
    sinOut = 0;
    bufferCount = 0;
    swapBuffers = false;
    currentInBuffer = inBuffer1;
    currentOutBuffer = outBuffer1;
    SYSTEM_Initialize();
    
    LED_SetValue(6,1);
    
    
    
    while (1)
    {
        if(swapBuffers)
        {
            BIN1(1);
            LED_ToggleValue(4);
            IEC0bits.T2IE = false;
            if (currentInBuffer == inBuffer1) {
                currentInBuffer = inBuffer2;
                currentOutBuffer = outBuffer2;
                previousInBuffer = inBuffer1;
                previousOutBuffer = outBuffer1;
            } else {
                currentInBuffer = inBuffer1;
                currentOutBuffer = outBuffer1;
                previousInBuffer = inBuffer2;
                previousOutBuffer = outBuffer2;
            }
            usDelay(5);
            IEC0bits.T2IE = true;
            swapBuffers = false;
            BIN1(0);
        }
    }

    return -1;
}
/**
 End of File
*/