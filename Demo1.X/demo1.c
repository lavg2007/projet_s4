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
#include <stdio.h>
#include <stdint.h>
#include <xc.h>
#include "aic.h"
#include "adc.h"
#include "lcd.h"
#include "utils.h"
#include "rgbled.h"
#include "swt.h"

#define BAUD_RATE 9600
#define RECEIVE_BUFFER_LEN  1024

// MX3 DSP library include files
#include "fftc.h"

// FIR filter transfer functions, non-negative frequency components only, scaled by 2^HScaleLog2 for fixed-point encoding
//#include "filterFIRcoeffs.h"

/*
                         Main application
 */
const unsigned int freqSampling = 20000;
const unsigned int freqTest = 1000;
int32_t *currentInBuffer, *currentOutBuffer, bufferCount;
int16_t buffer69; //16 bits
bool swapBuffers, flagCap;
int32_t stabValue1;
char bufChar[10];
int HScaleLog2 = 8;


void Clear(int cap1,int cap2, int past_cap1,int past_cap2)
{
//        int past_cap1,past_cap2 =0;
        
        if (past_cap1 > cap1)
        {
            LCD_DisplayClear();
//            DelayAprox10Us(300);
            LED_ToggleValue(7);
            
        }
        
        if (past_cap2 > cap2)
        {
            LCD_DisplayClear();
//            DelayAprox10Us(300);
            LED_ToggleValue(6);
        }
        past_cap1 = cap1;
        past_cap2 = cap2;
//        DelayAprox10Us(300);
}

 int Val_016(int val)
{
    int value;
    if (val<=100) value=0;
    else if (val> 924) value =16;
    else value = (((val))/50)-2;
    return value;
}

void lcdBlocks(int num1, int num2)
{unsigned char string1[2] = {0xff,'\0'};int j;
        for(j = 0; j < 16; j++){
            if(num1 > j){
                LCD_WriteStringAtPos(string1, 0, j);
            }
            if(num2 > j){
                LCD_WriteStringAtPos(string1, 1, j);
            }
        }
}

int main(void)
{
    int32_t *previousInBuffer, *previousOutBuffer;
    static int32_t inBuffer1[SIG_LEN], inBuffer2[SIG_LEN], outBuffer1[SIG_LEN], outBuffer2[SIG_LEN];
    static int32c inFFT[FFT_LEN], outFFT[FFT_LEN], Scratch[FFT_LEN], H[FFT_LEN];
    int i2cOut = '6';
    int address = 0;
    int16_t buffer; //16 bits?
    char serialTest[12];
    int y,i = 0;
    int8_t buffer1[3] = {0,0,0};
    // initialize the device
    bufferCount = 0;
    swapBuffers = false;
    currentInBuffer = inBuffer1;
    currentOutBuffer = outBuffer1;
    I2C_Init(100000); //initiallisation de la communication I2c a 1600hz
    char askConti[3] = {0x01,0x04,0x83};
    char pointReg[2] = {0x90,0x00};

    char i2cin1 = '6';
    char i2cin2 = '6';
    //Write to Config register:
        i2cin1 = I2C_Write(0x4a,askConti,3,1);
    //Set reading mode as continuous
        i2cin1 = I2C_Write(0x4a,pointReg,2,1);
    
    
    
    SYSTEM_Initialize();
    LED_SetValue(6,1);
    SWT_Init();
    LCD_Init();
    
    
 
    int past_cap1 = 0,past_cap2 = 0;
    
    int count =0;
    int k;
    int cstAvrg = 9;
    int valuescap1[cstAvrg + 1];
    int valuescap2[cstAvrg + 1];
    int stabValue2 = 0;
    int stabValue1_16;
    char toDisp1[64];
    char toDisp2[6];
    int m, n;
    int valMax = 0;
    int indxMax = 0;
    

    //initialisation du tableau pour les valeurs du cap1
    for (k=0 ;k < cstAvrg; k++){
        valuescap1[k] = 0;
        valuescap2[k] = 0;
    }
    
    
    while (1)
    {
        //BIN1(0);
        //DEBUG FFT
            //ISR
            //Lecture des capteurs
    
        if(flagCap)
        {
            BIN1(1);
            flagCap = false;
            address = 0x4a;
            i2cOut = I2C_Read(address,buffer1,2);
            buffer = buffer1[0];
            buffer = buffer << 4 ;
            buffer69 = buffer + (buffer1[1] >>4);
            if (buffer69<100){
                buffer69 = 100;
            }
            else if (buffer69>1000){
                buffer69 = 1000;
            }         
            stabValue1 = buffer69;
            BIN1(0);
        }
            
//        if(swapBuffers)
//        {
//            //BIN1(1);
//            LED_ToggleValue(3);
//            IEC0bits.T2IE = false;
//            if (currentInBuffer == inBuffer1) {
//                currentInBuffer = inBuffer2;
//                currentOutBuffer = outBuffer2;
//                previousInBuffer = inBuffer1;
//                previousOutBuffer = outBuffer1;
//            } else {
//                currentInBuffer = inBuffer1;
//                currentOutBuffer = outBuffer1;
//                previousInBuffer = inBuffer2;
//                previousOutBuffer = outBuffer2;
//            }
//            for (n = 0; n < SIG_LEN; n++) {
//                previousOutBuffer[n] = currentInBuffer[n];
//            }
//        //FFT pour preparer a la multiplication
////            for (n = 0, m = SIG_LEN - H_LEN; n < H_LEN; n++, m++) {
////                inFFT[n].re = currentOutBuffer[m] * FFT_LEN;
////                inFFT[n].im = 0;
////            }
////            for (m = 0; m < SIG_LEN; m++, n++) {
////                inFFT[n].re = previousOutBuffer[m] * FFT_LEN;
////                inFFT[n].im = 0;
////            }
////            mips_fft32(outFFT, inFFT, (int32c *) fftc, Scratch, LOG2FFTLEN); //la sortie de la FFT mesure 1024 element
////
////            
////            //Trouver l'index de la valuer maximale
////            for(n = 0;n < (SIG_LEN - H_LEN);n++) {
////                if(inFFT[n].re > valMax){
////                    indxMax = n;
////                    valMax = inFFT[n].re;
////                }
////                        
////            }
//            
//            
//            //FFT inverse si on a besoin plus tard
//            /*
//            if (SW0()) {
//                // FIR Filtering with Y = HX, actually (HX)* in preparation for inverse FFT using forward FFT    operation
//                // *** VOTRE CODE ICI: BOUCLE FOR MANQUANTE... ***
//                
//                //passer dans un filtre d'ordre 2 H(m) c'est donc une simple multiplication dans le domaine imaginaire
//                //(a+bi)(c+di) 	= (ac - bd) + (ad + bc)i
//                for(n = 0; n < FFT_LEN; n++){
//                   inFFT[n].re = (outFFT[n].re * H[n].re - outFFT[n].im * H[n].im);
//                   inFFT[n].im = -(outFFT[n].re * H[n].im + outFFT[n].im * H[n].re); //  le moins (-) c'est pour le conjugue complexe
//                }
//            } else {
//                // Simply pass-through of X* in preparation for inverse FFT using forward FFT operation
//                for (n = 0; n < FFT_LEN; n++) { // Conjugaison complexe de la multiplication
//                    inFFT[n].re = outFFT[n].re;
//                    inFFT[n].im = -outFFT[n].im;
//                }
//            }
//            
//            mips_fft32(outFFT, inFFT, (int32c *) fftc, Scratch, LOG2FFTLEN);
//
//            // Extract real part of the inverse FFT, discard first 1/4 (H_LEN) samples as per the "Overlap-and-save" method, remove H scaling
//            if (SW0()) {
//                for (n = 0; n < SIG_LEN; n++)
//                    previousOutBuffer[n] = ((int) (outFFT[n + H_LEN].re)) >> HScaleLog2;
//            } else {
//                for (n = 0; n < SIG_LEN; n++)
//                    previousOutBuffer[n] = outFFT[n + H_LEN].re;
//            }
//             */
//            
//            IEC0bits.T2IE = true;
//            swapBuffers = false;
//            //BIN1(0);
//        }
        //STUFF D'AFFICHAGE ET LECTURE CAPTEUR      
        
        
       
        //test pour faire un integrateur fuillant au lieu de faire une moyenne
        //on trouve les nouvelles valeurs
//        stabValue1 *=0.4; // ajuster le coeff
        
        //equivalent de l'integrateur mais sur une val de 0 a 16
//        stabValue1_16 = Val_016(buffer69);
////        stabValue2 +=  Val_016(18);
////        stabValue2 *= 0.6;
//        //stabValue1_16 = 0;//Val_016(buffer69_2); --> adapter pour le deuxieme capteur
//        stabValue2 = 0;
//        
//        //check si faut clear le display
//        Clear(stabValue1_16,stabValue2,past_cap1,past_cap2);
//        
//        //on save la derniere valeur pour faire des comparaisons plus tar
//        past_cap1 = stabValue1_16;
//        past_cap2 = stabValue2;
//        
//        //update du display
//        lcdBlocks(stabValue1_16,stabValue2);
//        
//        
        
        
        //BIN1(1);
        
    if (count == cstAvrg)
        {
            LED_ToggleValue(2);
            count =0;
        }
        else{
        count++;
        }
        
    }


        
    
    
    //return -1;
}
/**
 End of File
*/