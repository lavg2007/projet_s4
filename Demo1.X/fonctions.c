/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include <xc.h>
#include "fonctions.h"
#include "lcd.h"
#include "utils.h"

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
