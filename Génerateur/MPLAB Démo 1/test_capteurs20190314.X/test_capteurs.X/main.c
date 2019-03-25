/**
 * Author:    Pascal Gendron
 * Created:   2019-01-22
 * Universite de Sherbrooke
 **/

#include <stdio.h>
#include <stdint.h>
#include <xc.h>
#include "hardware.h"
#include "aic.h"
#include "timer.h"
#include "math.h"
#include "mcc.h"
#include "fftc.h"
#include "swDelay.h"

#define BAUD_RATE 9600
#define RECEIVE_BUFFER_LEN  1024
#define SIG_LEN 1024

int phase = 0;
int sine = 0;
const int freq = 1000;
const int fs = 16000;
int32_t* currentOutBuffer;
int32_t* currentInBuffer;
int32_t* pastOutBuffer;
int32_t* pastInBuffer;

int32_t inBuffer1[SIG_LEN];
int32_t inBuffer2[SIG_LEN];
int32_t outBuffer1[SIG_LEN];
int32_t outBuffer2[SIG_LEN];
int32_t sampleCount;
short swapBuffers;

#define _TMR2

// Board clock = 8 MHz
// Setting T1_INTR_RATE to 10000 means the timer will 
// fire every: 8Mhz/2*20/8/1/T1_INTR_RATE = 1ms
// See: Unit 2 - Elements of Real-time Systems / Unit 2 for more information
#define T2_INTR_RATE 10000

void initialize_timer_interrupt() { 
    // Refer to : https://reference.digilentinc.com/_media/learn/courses/unit-2/unit_2.pdf 
    // for more information
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    LCD_Init();
    currentInBuffer = inBuffer1;
    sampleCount = 0;
    OpenTimer2( (T2_ON | T2_SOURCE_INT | T2_PS_1_1), (T2_INTR_RATE - 1) ); 
    mT2SetIntPriority(1);        
    mT2SetIntSubPriority(0);       
    mT2IntEnable(1);        
} 


void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler(void) {	
    phase = phase + freq/fs;
    sine = sin(2*M_PI*phase);
    currentInBuffer[sampleCount] = (sine)*(2^9);
    sampleCount += 1;
    
    if (sampleCount >= SIG_LEN) {
        LED_ToggleValue(3);
        sampleCount = 0;
        swapBuffers = 1;
        BIN2(1);
        usDelay(1);
        BIN2(0);
        
    }
    mT2ClearIntFlag();	// Macro function to clear the interrupt flag
}

//void lcdBlocks(char line, int num2)




void lcdBlocks(int num1, int num2)
{unsigned char string1[2] = {0xff,'\0'};
        int j;
//         for (j=0 ;j < num; j++)
//    {
////       DelayAprox10Us(100);
//       LCD_WriteStringAtPos(string1, line, j);
////       DelayAprox10Us(600);
//        
//    }
        for(j = 0; j < 16; j++){
            if(num1 > j){
                LCD_WriteStringAtPos(string1, 0, j);
            }
            if(num2 > j){
                LCD_WriteStringAtPos(string1, 1, j);
            }
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

 int val_016(int val)       //Fonction écrite par Phil
{
    int value;
    if (val<=150) value=0;
    else if (val> 924) value =16;
    else value = (val/50)-2;
    return value;
}

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


//    SSD_Init();
//    SSD_WriteDigits(16,6,1,1);
//         
//    DelayAprox10Us(10100);
//    SSD_Close();


void main() {
      LED_Init();
      RGBLED_Init();
      BTN_Init();
      initialize_timer_interrupt();
      ADC_Init();
      LCD_Init();
      I2C_Init(1600); //initialisation de la communication I2c a 1600hz
      UART_Init(9600);
      
    int cap1,cap2;
    int past_cap1 = 0,past_cap2 = 0;
    
    int count =0;
    char cap1str[40];
    char countstr[40];
    int k;
    int cstAvrg = 9;
    int valuescap1[cstAvrg + 1];
    int valuescap2[cstAvrg + 1];
    int moyennecap1 = 0;
    int moyennecap2 = 0;
    int stabValue1 = 0;
    int stabValue2 = 0;
    int stabValue1_16 = 0;
    int stabValue2_16 = 0;
    int i2cOut = 0;
    int address = 0;
    char buff[64];
    int toDisp1[64];
    int toDisp2[64];
    
//initialisation du tableau pour les valeurs du cap1
    for (k=0 ;k < cstAvrg; k++){
        valuescap1[k] = 0;
        valuescap2[k] = 0;
    }
    
    while(1)
    {   
        //AUDIO_Init('0',moyennecap1+1);

        
         if (BTN_GetValue('d')==1)
         {
            if (stabValue1 < 150 || stabValue1 > 950){RGBLED_SetValue(255,0,0);}
            else{RGBLED_SetValue(0,255,0);}        
         }
         else{RGBLED_SetValue(255,35,0);}

        if (swapBuffers == 1)
        {
            // Raise pin BIN1, oscilloscope timing measurements
            BIN1(1);
            LED_ToggleValue(2);
            if (currentInBuffer == inBuffer1) {
                currentInBuffer = inBuffer2;
                currentOutBuffer = outBuffer2;
                pastInBuffer = inBuffer1;
                pastOutBuffer = outBuffer1;
            } else {
                currentInBuffer = inBuffer1;
                currentOutBuffer = outBuffer1;
                pastInBuffer = inBuffer2;
                pastOutBuffer = outBuffer2;
            }
            
            //OC1_PWMPulseWidthSet((((currentOutBuffer[sampleCount] + 512) * PR2) >> 10));
            BIN1(0);
            swapBuffers = 0;
        }
        
        //test pour faire un integrateur fuyant au lieu de faire une moyenne
        //on trouve les nouvelles valeurs
        stabValue1 +=  ADC_AnalogRead(17); //valeur 0-1024
        stabValue1 *= 0.55; //facteur de fuite a valider 
        stabValue2 +=  ADC_AnalogRead(18);
        stabValue2 *= 0.6;
        stabValue1_16 = val_016(stabValue1);//valeur 0-16
        stabValue2_16 = val_016(stabValue2);
        //check si faut clear le display
        //Clear(stabValue1_16,stabValue2_16,past_cap1,past_cap2);
        
        //on save la derniere valeur pour faire des comparaisons plus tar
        past_cap1 = stabValue1_16;
        past_cap2 = stabValue2_16;
        
        
        //update du display
        //lcdBlocks(stabValue1_16,stabValue2_16);
        
        //Envoie de la valeur par UART pour la lire avec python
        //sprintf(buff, "%04d;%04d", stabValue1, stabValue2);
        //UART_PutString(buff);
        
        //DelayAprox10Us(1000);
        

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

