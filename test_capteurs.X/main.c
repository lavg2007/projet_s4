/**
 * Author:    Pascal Gendron
 * Created:   2019-01-22
 * Universite de Sherbrooke
 **/

#include <stdio.h>
#include <stdint.h>
#include <xc.h>
#include "aic.h"
#define BAUD_RATE 9600
#define RECEIVE_BUFFER_LEN  1024

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
    int moyennecap1 = 0;
    int moyennecap2 = 0;
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
//        if (ADC_AnalogRead(17)<150) cap1=0;
//        else if (ADC_AnalogRead(17)> 950) cap1 =950;
//        else cap1 = ADC_AnalogRead(17);
        cap1 = ADC_Val_016(17);
         lcdBlocks(0, cap1);
        
         if (BTN_GetValue('d')==1)
         {
            if (moyennecap1 < 100 || moyennecap1 > 950){RGBLED_SetValue(255,0,0);}
            else{RGBLED_SetValue(0,255,0);}        
         }
         else{RGBLED_SetValue(255,35,0);}
        DelayAprox10Us(100);

        cap2 = ADC_Val_016(18);
//        lcdBlocks(1, cap2);
//        
        
        
        
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

