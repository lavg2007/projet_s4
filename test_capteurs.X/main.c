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
       DelayAprox10Us(100);
       LCD_WriteStringAtPos(string, line, j);
       DelayAprox10Us(100);
        
    }
    

}

void main() {
      AIC_Init();
      LED_Init();
//      ADC_Init();
    LCD_Init();
    PMODS_InitPin(0,8,1,0,0);
    PMODS_InitPin(0,9,1,0,0);
    int cap1,cap2;
    int past_cap1,past_cap2 =0;
    int count =0;
//    char btn = '0';
    char cap1str[40];
    while(1)
    {   BTN_Init();
//        LCD_DisplayClear();
        DelayAprox10Us(100);
//        if (ADC_AnalogRead(17)<150) cap1=0;
//        else if (ADC_AnalogRead(17)> 950) cap1 =950;
//        else cap1 = ADC_AnalogRead(17);
        cap1 = AIC_Val_016(17);
         lcdBlocks(0, cap1);
        
        
        
        DelayAprox10Us(100);
//        sprintf( cap1str, "Capteur 1: %d", cap1 );
//        LCD_WriteStringAtPos(cap1str, 1, 0);
        cap2 = AIC_Val_016(18);
        lcdBlocks(1, cap2);
        
        DelayAprox10Us(5000);
        
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
//        btn ='1'; //BTN_GetValue('D');
        DelayAprox10Us(5000);
//        if (btn=='1')
//        {
//            LCD_DisplayClear();
//            DelayAprox10Us(200);
//            LCD_WriteStringAtPos("Sound Switch",0,0);
//            DelayAprox10Us(1000);
//            LCD_DisplayClear();
//        }
        if (count == 10)
        {
            LED_ToggleValue(0);
            count =0;
        }
        count++;
    }
}

