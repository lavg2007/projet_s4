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
{
    unsigned char string[2] = {0b11111111, '\0'};
    int j = 0;
    while(j <= num && j <= 16)
    {
        LCD_WriteStringAtPos(string, line, j);
        j += 1;
    }
}

void main() {
      AIC_Init();
      ADC_Init();
    LCD_Init();
    unsigned int potval;
    while(1)
    {
        LCD_DisplayClear();
        DelayAprox10Us(10);
        potval = AIC_Val()/64;
        lcdBlocks(0, potval);
        DelayAprox10Us(10000);
        
    }
}