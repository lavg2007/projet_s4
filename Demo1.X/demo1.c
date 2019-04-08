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
//#include "app_commands.h"

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
bool swapBuffers, flagCap;
int32_t stabValue1;
char bufChar[10];
int HScaleLog2 = 8;
int32_t stabFreq; // valeur de sortie du capteur de frequence
int32_t stabAmp; // valeur de sortie du capteur d'amplitude


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

int val_016(int val)       //Fonction écrite par Phil
{
    int value;
    if (value<=150) value =0;
	else if (value>150 && value<=200) value=1;
	else if (value>200 && value<=250) value=2;
	else if (value>250 && value<=300) value=3;
	else if (value>300 && value<=350) value=4;
	else if (value>350 && value<=400) value=5;
	else if (value>150 && value<=450) value=6;
	else if (value>450 && value<=500) value=7;
	else if (value>150 && value<=550) value=8;
	else if (value>550 && value<=600) value=9;
	else if (value>150 && value<=650) value=10;
	else if (value>650 && value<=700) value=11;
	else if (value>150 && value<=750) value=12;
	else if (value>750 && value<=800) value=13;
	else if (value>150 && value<=850) value=14;
	else if (value>850 && value<=900) value=15;
	else value =16;
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
int32_t leaky(int32_t sum, int16_t val){
    //integrateur fuyant avec facteur d'oubli de 0.5
//   return((sum + val) >> 1 + (sum + val) >> 3);
    
        sum += val;
        sum *= 0.45;
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
    int8_t buffer2[3] = {0,0,0};
    // initialize the device
    bufferCount = 0;
    swapBuffers = false;
    currentInBuffer = inBuffer1;
    currentOutBuffer = outBuffer1;
    I2C_Init(150000); //initiallisation de la communication I2c a 1600hz
    char askContiADC0[3] = {0x01,0x44,0x93};
    char askContiADC1[3] = {0x01,0x54,0x93};
    char pointReg[2] = {0x90,0x00};

    char i2cin1 = '6';
    char i2cin2 = '6';
    
    
    
    
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
    int stabValue2;
    int stabValue1_16;
    char toDisp1[64];
    char toDisp2[6];
    int m, n;
    int valMax = 0;
    int indxMax = 0;
    int firfreq[32] ={0};
    int firamp[32] ={0};
    int stabAmp_16;
    int stabFreq_16;
    bool capNum = false;
    

    //initialisation du tableau pour les valeurs du cap1
    for (k=0 ;k < cstAvrg; k++){
        valuescap1[k] = 0;
        valuescap2[k] = 0;
    }
    
    
    
    
    while (1)
    {
        //Les deux capteurs ne peuvent pas etre lu dans mm cycle de lecture
        // comprends pas, vrm pas
        if(flagCap)
        {
            address = 0x4a;
            if(capNum == true){
                int8_t freqBuf[2];
                int16_t freq;
                
                //Write to Config register:
                I2C_Write(address,askContiADC0,3,1);
                //Set reading mode as continuous for ADC 0
                I2C_Write(address,pointReg,2,1);

                I2C_Read(address,freqBuf,2);
                buffer = freqBuf[0];
                buffer = buffer << 4;
                freq = buffer + (freqBuf[1] >>4);
//                
                
                firfreq[15] = firfreq[14]; //>>1;
                firfreq[14] = firfreq[13]; //>>1;
                firfreq[13] = firfreq[12]; //>>1;
                firfreq[12] = firfreq[11]; //>>1;
                firfreq[11] = firfreq[10]; //>>1;
                firfreq[10] = firfreq[9]; //>>1;
                firfreq[9] = firfreq[8]; //>>1;
                firfreq[8] = firfreq[7]; //>>1;
                firfreq[7] = firfreq[6]; //>>1;
                firfreq[6] = firfreq[5]; //>>1;
                firfreq[4] = firfreq[3]; //>>1;                
                firfreq[3] = firfreq[2]; //>>1;
                firfreq[2] = firfreq[1]; //>>1;
                firfreq[1] = firfreq[0];// >>1;
                
                int i,n;
                stabValue2 = 0;
                firfreq[0] = freq;
                
                for(n = 0;n < 16;n++){
                    
                    stabValue2 += (firfreq[n]); 
                }
                stabValue2 = stabValue2 >> 4;
                if (stabValue2<150){
                    stabFreq  = 150;//stabFreq = leaky(stabFreq,150);
                }else if(stabValue2>1000){
                   stabFreq  = 1000;// stabFreq = leaky(stabFreq,1000);
                }else{
                    stabFreq  = stabValue2;//stabFreq = leaky(stabFreq,freq);
                }  
                
                
                LED_ToggleValue(3);
                capNum = false;
            }else{
                int8_t ampBuf[2];
                int16_t amp;
                
                //Write to Config register:
                I2C_Write(address,askContiADC1,3,1);
                //Set reading mode as continuous for ADC 0
                I2C_Write(address,pointReg,2,1);


                I2C_Read(address,ampBuf,2);
                buffer = ampBuf[0];
                buffer = buffer << 4 ;
                amp = buffer + (ampBuf[1] >>4);
//                if (amp<100){
//                    amp = 100;// leaky(stabAmp,100);
//                }else if(amp>1000){
//                    amp = 1000;//leaky(stabAmp,1000);
//                }else{
//                    amp = amp;//leaky(stabAmp,amp);
//                }
                firamp[15] = firamp[14]; //>>1;
                firamp[14] = firamp[13]; //>>1;
                firamp[13] = firamp[12]; //>>1;
                firamp[12] = firamp[11]; //>>1;
                firamp[11] = firamp[10]; //>>1;
                firamp[10] = firamp[9]; //>>1;
                firamp[9] = firamp[8]; //>>1;
                firamp[8] = firamp[7]; //>>1;
                firamp[7] = firamp[6]; //>>1;
                firamp[6] = firamp[5]; //>>1;
                firamp[4] = firamp[3]; //>>1;                
                firamp[3] = firamp[2]; //>>1;
                firamp[2] = firamp[1]; //>>1;
                firamp[1] = firamp[0];// >>1;
                
                int i,n;
                stabValue1 = 0;
                firamp[0] = amp;
                
                for(n = 0;n < 16;n++){
                    
                    stabValue1 += (firamp[n]); 
                }
                stabAmp  = (stabValue1 >> 4) + (stabValue1 >>5);
                LED_ToggleValue(4);
                capNum = true;
                
            }
            
            flagCap = false;
        }
            
        if(swapBuffers)
        {
//            BIN1(1);
            BIN1(1);
            LED_ToggleValue(1);
////            IEC0bits.T2IE = false;
            if (currentInBuffer == inBuffer1) {
                previousInBuffer = inBuffer1;
                currentInBuffer = inBuffer2;
            } else {
                currentInBuffer = inBuffer1;
                previousInBuffer = inBuffer2;
            }
            //PACKETIZE
//            int countECH;
//            for(countECH=0,i=0;1<SIG_LEN;countECH++,i+=4){
//                UDP_Send_Buffer[i]= ((previousInBuffer[countECH] &  0x000000ff));
//                UDP_Send_Buffer[i+1]=((previousInBuffer[countECH] & 0x0000ff00)>>8);
//                UDP_Send_Buffer[i+2]=((previousInBuffer[countECH] & 0x00ff0000)>>16);
//                UDP_Send_Buffer[i+3]=((previousInBuffer[countECH] & 0xff000000)>>24);
//            }
            
            
            swapBuffers = false;
            //export previous buffer
            BIN1(0);
        }
//        //STUFF D'AFFICHAGE ET LECTURE CAPTEUR      
//        
//        
//       
//        //test pour faire un integrateur fuillant au lieu de faire une moyenne
//        //on trouve les nouvelles valeurs
//        stabFreq_16 = Val_016(stabFreq);
//        stabAmp_16 = Val_016(stabAmp);
//        
////        
////        //check si faut clear le display
//        Clear(stabValue1_16,stabValue2,past_cap1,past_cap2);
////        
////        //on save la derniere valeur pour faire des comparaisons plus tar
//        past_cap1 = stabFreq_16;
//        past_cap2 = stabAmp_16;
////        
////        //update du display
//        lcdBlocks(stabValue1_16,stabValue2);
////        
////        
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