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
      
      ADC_Init();
      LCD_Init();
      I2C_Init(1600); //initiallisation de la communication I2c a 1600hz
      
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
    int i2cOut = 0;
    int address = 0;
    int buffer[16];
    int toDisp1[64];
    int toDisp2[64];
    
//initialisation du tableau pour les valeurs du cap1
    for (k=0 ;k < cstAvrg; k++){
        valuescap1[k] = 0;
        valuescap2[k] = 0;
    }
    
    while(1)
    {   
        AUDIO_Init('0',moyennecap1+1);

        
         if (BTN_GetValue('d')==1)
         {
            if (moyennecap1 < 100 || moyennecap1 > 950){RGBLED_SetValue(255,0,0);}
            else{RGBLED_SetValue(0,255,0);}        
         }
         else{RGBLED_SetValue(255,35,0);}

        
        //test pour faire un integrateur fuillant au lieu de faire une moyenne
        //on trouve les nouvelles valeurs
        stabValue1 +=  ADC_Val_016(17);
        stabValue1 *= 0.6;
        stabValue2 +=  ADC_Val_016(18);
        stabValue2 *= 0.6;
        
        //check si faut clear le display
        Clear(stabValue1,stabValue2,past_cap1,past_cap2);
        
        //on save la derniere valeur pour faire des comparaisons plus tar
        past_cap1 = stabValue1;
        past_cap2 = stabValue2;
        
        //update du display
        lcdBlocks(stabValue1,stabValue2);
//        address = 0x4a;
//        i2cOut = I2C_Read(address,buffer,16);
//        snprintf(toDisp1,10,"%d",i2cOut);  //0 si ca marche
//        snprintf(toDisp2,18,"%d",buffer);  //devrait etre les valeurs lu
//        LCD_DisplayClear();
//        DelayAprox10Us(1000);
//        LCD_WriteStringAtPos(toDisp1, 0);
//        LCD_WriteStringAtPos(toDisp2, 1);
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

