/* 
 * File:   main.c
 * Author: Cristian Fatu
 *
 * Created on April 13, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "adc.h"
#include "aic.h"
#include "timer.h"
#include "mot.h"
#include "lcd.h"

#define BAUD_RATE 9600
#define RECEIVE_BUFFER_LEN  1024

#define _TMR2

// Board clock = 8 MHz
// Setting T1_INTR_RATE to 10000 means the timer will 
// fire every: 8Mhz/2*20/8/1/T1_INTR_RATE = 1ms
// See: Unit 2 - Elements of Real-time Systems / Unit 2 for more information
#define T2_INTR_RATE 50    

float sin = 0;
float line_out = 0;
float phase = 1;
const int fs = 20000;
int moyennecap1 = 0;
int moyennecap2 = 0;
const int freq = 1000;

// Since the flag is changed within an interrupt, we neet the keyword volatile.
static volatile int Flag_timer = 0;
static volatile int Flag_btn = 0;
void initialize_timer_interrupt() { 
    // Refer to : https://reference.digilentinc.com/_media/learn/courses/unit-2/unit_2.pdf 
    // for more information
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    
    OpenTimer2( (T2_ON | T2_SOURCE_INT | T2_PS_1_1), (T2_INTR_RATE - 1) ); 
    mT2SetIntPriority(1);        
    mT2SetIntSubPriority(0);       
    mT2IntEnable(1);        
} 

void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler(void) {	
    phase = phase + freq/fs;
    output = sin(2*M_PI*phase);
    
    mT2ClearIntFlag();	// Macro function to clear the interrupt flag
}





void lcdBlocks(char line, int num)
{unsigned char string[2] = {0b11111111,'\0'};
        int j;
         for (j=0 ;j < num; j++)
    {
//       DelayAprox10Us(100);
       LCD_WriteStringAtPos(string, line, j);
//       DelayAprox10Us(100);
        
    }
    

}
int moyenne(int * values, int nbrVal){
    int somme = 0;
    int k;
        for (k=0 ;k<=nbrVal; k++)
        {
            somme += values[k];            
        }    
    return (somme / (nbrVal+1));
}

void Clear(int cap1,int cap2)
{
        int past_cap1,past_cap2 =0;
            if (past_cap1 > cap1)
        {
            LCD_DisplayClear();
        }
        
        if (past_cap2 > cap2)
        {
            LCD_DisplayClear();
        }
        past_cap1 = cap1;
        past_cap2 = cap2;
}


//    SSD_Init();
//    SSD_WriteDigits(16,6,1,1);
//         
//    DelayAprox10Us(10100);
//    SSD_Close();


void main() {
      LED_Init();
      RGBLED_Init();
      BTN_Init();
      
      ADC_Init();
      LCD_Init();
    int cap1,cap2;

    
    int count =0;
//    char btn = '0';
    char cap1str[40];
    char countstr[40];
    int k;
    int cstAvrg = 9;
    int valuescap1[cstAvrg + 1];
    int valuescap2[cstAvrg + 1];

//initialisation du tableau pour les valeurs du cap1
    for (k=0 ;k < cstAvrg; k++){
        valuescap1[k] = 0;
        valuescap2[k] = 0;
    }
    
    while(1)
    {   
//        AUDIO_Init('0',48000);
        LCD_DisplayClear();
        DelayAprox10Us(100);
        Clear(cap1,cap2);

        valuescap1[count]=ADC_AnalogRead(17);
        valuescap2[count]=ADC_AnalogRead(18);
        moyennecap1 = moyenne(valuescap1, cstAvrg);
        moyennecap2 = moyenne(valuescap2, cstAvrg);
        
        
        sprintf( cap1str, "moyenne : %d", moyennecap1 );
        LCD_WriteStringAtPos(cap1str, 1, 0);
        DelayAprox10Us(1000);
        if (count == cstAvrg)
        {
            LED_ToggleValue(0);
            count =0;
        }
        else{
        count++;
        }
    }
}

