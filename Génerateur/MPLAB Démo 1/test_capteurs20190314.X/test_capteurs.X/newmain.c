/* 
 * File:   newmain.c
 * Author: Gabriel
 *
 * Created on 7 février 2019, 11:34
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


/*
 * 
 */
int main(int argc, char** argv) {

    LCD_Init();
    PMODS_InitPin(0,8,1,0,0);
    PMODS_InitPin(0,8,1,0,0);
    ADC_Init();
    
    unsigned int cap1;
    unsigned int cap2;
    char cap1str[40];
    char cap2str[40];
    while(1)
    {
        cap1 = ADC_AnalogRead(17);
        cap2 = ADC_AnalogRead(18);
        sprintf( cap1str, "Capteur 1: %d", cap1 );
        sprintf( cap2str, "Capteur 2: %d", cap2 );
        LCD_WriteStringAtPos(cap1str, 0, 0);
        LCD_WriteStringAtPos(cap2str, 1, 0);
        DelayAprox10Us(1000);
        LCD_DisplayClear();
        DelayAprox10Us(10);
    }
            
    return (EXIT_SUCCESS);
}

