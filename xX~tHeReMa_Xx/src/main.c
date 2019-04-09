/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "main.h"
#include "global_event.h"
#include "bsp.h"
#include "system_config.h"
#include "system/common/sys_module.h"   // SYS function prototypes
#include "driver/spi/src/dynamic/drv_spi_internal.h"
#include "../Theremax.X/swt.h"
#include "UDP_app.h"
#include "led.h"
#include "lcd.h"
#include "app_commands.h"
#include "tmr5.h"
#include "tmr3.h"
#include "oc1.h"
#include "i2c.h"
#include "pin_manager.h"
#include "../Theremax.X/btn.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************


int32_t *currentInBuffer, bufferCount;
int16_t buffer69; //16 bits
bool swapBuffers, flagCap;
static bool btnDb;
int32_t stabFreq; // valeur de sortie du capteur de frequence
int32_t stabAmp; // valeur de sortie du capteur d'amplitude


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the TCPIP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

MAIN_DATA mainData;
//static enum 
//{
//    USART_RW_INIT,
//    USART_RW_WORKING,
//    USART_RW_DONE,
//} usartRWState;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


        static unsigned long int counter=0;


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MAIN_Initialize ( void )

  Remarks:
    See prototype in main.h.
 */


void OSCILLATOR_Initialize(void)
{
    SYSTEM_RegUnlock(); 
    // CF no clock failure; COSC FRCPLL; PLLODIV DIV_1; PBDIVRDY disabled; SLOCK out of lock; FRCDIV FRC/1; SLPEN Idle on WAIT instruction; NOSC FRCPLL; PLLMULT MUL_24; SOSCEN disabled; PBDIV DIV_1; CLKLOCK unlocked; OSWEN Switch is Complete; SOSCRDY disabled; 
    OSCCON = 0x71100;
    SYSTEM_RegLock();
    // TUN Center Frequency; 
    OSCTUN = 0x0;
    // DIVSWEN disabled; RSLP disabled; ACTIVE Active; ROSEL SYSCLK; OE Not Driven out on REFCLKO pin; SIDL disabled; RODIV 0; ON disabled; 
    REFOCON = 0x100;
    // ROTRIM 0; 
    REFOTRIM = 0x0;
}

void INTERRUPT_Initialize (void)
{    
    //    TI: Timer 2
    //    Priority: 1
    //    SubPriority: 0
        IPC5bits.T5IP = 1;
        IPC5bits.T5IS = 0;

    //  Enable the multi vector
    INTCONbits.MVEC = 1;
    //  Enable Global Interrupts 
    __builtin_mtc0(12,0,(__builtin_mfc0(12,0) | 0x0001));

}
void MAIN_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    mainData.state = MAIN_STATE_INIT;

    mainData.handleUSART0 = DRV_HANDLE_INVALID;
    
    
    
    PIN_MANAGER_Initialize();
    //OSCILLATOR_Initialize();
    SWT_Init();
    BTN_Init();
    TMR5_Initialize();
    TMR3_Initialize();
    OC1_Initialize();
    UDP_Initialize();
    LED_Init();
    LCD_Init();
    INTERRUPT_Initialize();    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    LED_SetValue(0,1);
}

int val_016(int32_t val)       //Fonction écrite par Phil
{
    int value;
    if (val<=150) value =0x0;
	else if (val>151 && val<=190) value=0x1;
	else if (val>190 && val<=230) value=0x2;
	else if (val>230 && val<=270) value=0x3;
	else if (val>270 && val<=310) value=0x4;
	else if (val>310 && val<=350) value=0x5;
	else if (val>350 && val<=390) value=0x6;
	else if (val>390 && val<=430) value=0x7;
//	else if (val>430 && val<=470) value=0x8;
//	else if (val>470 && val<=510) value=0x9;
//	else if (val>510 && val<=550) value=0xA;
//	else if (val>550 && val<=590) value=0xB;
//	else if (val>590 && val<=630) value=0xC;
//	else if (val>630 && val<=670) value=0xD;
//	else if (val>670 && val<=710) value=0xE;
//	else if (val>710 && val<=750) value=0xF;
	else value =0x10;
    return value;
}
void Clear(int cap1,int cap2, int past_cap1,int past_cap2)
{       
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
//affichage de l'instrument choisi


static void LedTask(void) {
    if (global_event_triggered(&global_events.tic_toc)) {
        global_events.tic_toc = false;
        counter++;
        if(counter == 1000){
            LED_ToggleValue(1);
            counter = 0;
        }
    }
}
/******************************************************************************
  Function:
    void MAIN_Tasks ( void )

  Remarks:
    See prototype in main.h.
 */

void MAIN_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( mainData.state )
    {
            /* Application's initial state. */
        case MAIN_STATE_INIT:
        {
            bool appInitialized = true;
            SYS_CONSOLE_MESSAGE("Init\r\n");

            if (mainData.handleUSART0 == DRV_HANDLE_INVALID)
            {
                mainData.handleUSART0 = DRV_USART_Open(MAIN_DRV_USART, DRV_IO_INTENT_READWRITE|DRV_IO_INTENT_NONBLOCKING);
                appInitialized &= (DRV_HANDLE_INVALID != mainData.handleUSART0);
            }


            if (appInitialized)
            {

                mainData.state = MAIN_STATE_SERVICE_TASKS;
            }
            break;
        }

        case MAIN_STATE_SERVICE_TASKS:
        {
            UDP_Tasks();
            //LedTask();
            //LED0Toggle();
            JB1Toggle();        //LED heartbeat
            
            global_events.tic_toc = true;
            break;
        }

            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

int main(void) {
    int32_t *previousInBuffer;
    static int32_t inBuffer1[SIG_LEN], inBuffer2[SIG_LEN];
    int address;
    int16_t buffer; //16 bits?
    int stabValue2;
    int stabValue1;
    // initialize the device
    bufferCount = 0;
    swapBuffers = false;
    currentInBuffer = inBuffer1;
     I2C_Init(150000); //initiallisation de la communication I2c a 150 000hz
    char askContiADC0[3] = {0x01,0x44,0x93};
    char askContiADC1[3] = {0x01,0x54,0x93};
    char pointReg[2] = {0x90,0x00};
    bool capNum = false;
    int firfreq[16] ={0};
    int firamp[16] ={0};
    int va_016, vf_016, vpa_016,vpf_016;
    int NVBA=0;
    SYS_Initialize(NULL);
    
    while (1) {
        SYS_Tasks();
        MAIN_Tasks();
        if (!BTN_GetValue('d') && btnDb)
            btnDb = false;
        if (BTN_GetValue('d') && !btnDb)
            {
                LED_ToggleValue(instr);
                btnDb = true;
                ++instr;
                if (instr > 5)
                    instr = 0;
                LED_ToggleValue(instr);
            }
        // swap des buffer de réception
        
        // swap des buffers de génération
        if(swapBuffers)
        {
           
            
            //LED_ToggleValue(1);
////            IEC0bits.T2IE = false;
            if (currentInBuffer == inBuffer1) {
                previousInBuffer = inBuffer1;
                currentInBuffer = inBuffer2;
            } else {
                currentInBuffer = inBuffer1;
                previousInBuffer = inBuffer2;
            }
           
            //PACKETIZE
            int countECH,i;            
            for(countECH=0,i=0;countECH<SIG_LEN;countECH++,i+=4){
                UDP_Send_Buffer[i]= ((previousInBuffer[countECH] &  0x000000ff));
                UDP_Send_Buffer[i+1]=((previousInBuffer[countECH]>>8) & 0x000000ff);
                UDP_Send_Buffer[i+2]=((previousInBuffer[countECH]>>16) & 0x000000ff);
                UDP_Send_Buffer[i+3]=((previousInBuffer[countECH]>>24) & 0x000000ff);
            }
            UDP_bytes_to_send = (countECH)*4;
            countECH = 0;
            UDP_Send_Packet = true;          
            
            swapBuffers = false;
            //compteur pour affichage instrument
            
            //export previous buffer

        }
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
                stabFreq = (stabValue2 >> 4);
                
                
                
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
                stabAmp  = (stabValue1 >> 4);
                
                capNum = true;
                
            }
            
            flagCap = false;
            vf_016 = val_016(stabFreq);
            va_016 = val_016(stabAmp);
//            NVBA++;
        }
        
            
        if (NVBA  >= 25) {
          
            Clear(vf_016,va_016,vpf_016,vpa_016); 

            lcdBlocks(vf_016,va_016);


            vpf_016 = vf_016;
            vpa_016 = va_016;

            NVBA = 0;
        }

    };

    return 0;
}


/*******************************************************************************
 End of File
 */
