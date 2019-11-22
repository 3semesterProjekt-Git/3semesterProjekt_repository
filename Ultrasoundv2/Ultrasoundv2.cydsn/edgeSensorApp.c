#include "edgeSensorApp.h"

volatile uint32_t pulseWidth;
volatile int time_us;
volatile int e1 = 0, e2 = 0, e3 = 0, e4 = 0;
volatile int status = 0;

CY_ISR(pulseInt_handler)
{
    triggerPWM_Stop();
    //Reads number of counts in pulse
    pulseWidth = pulseCounter_ReadCapture();
    //Converts data to an integer of us
    time_us = (uint32_t)pulseWidth * 2;
    //About 30 cm    
    if(time_us > 1800)
    {
        status = status + 1;
    }
    
    switch (status)
    {
        case 11:    //LED_Write(1);     //Forreste venstre sensor
                    DelayBased180Turn();
                break;
        case 21:    //LED_Write(1);     //Venstre sensor
                    DelayBased90Turn(1);
                break;
        case 31:    //LED_Write(1);     //Højre sensor
                    DelayBased90Turn(2);
                break;
        case 41:    //LED_Write(1);     //Forreste højre sensor
                    DelayBased180Turn();
                break;
        default:    //LED_Write(0);
                    driveForwards();
                break;
    }
    
    //Resetting variables
    getStatus();
    //Clear capture flag
    pulseCounter_ReadStatusRegister();
    triggerPWM_Start();
}

CY_ISR(echoInt1_handler)
{
    e1 = 1;
    if((e1+e2+e3+e4) == 4 )
    {
        status = 10;
    }
}

CY_ISR(echoInt2_handler)
{
    e2 = 1;
    if((e1+e2+e3+e4) == 4 )
    {
        status = 20;
    }
}

CY_ISR(echoInt3_handler)
{   
    e3 = 1;
    if((e1+e2+e3+e4) == 4 )
    {
        status = 30;
    }
}

CY_ISR(echoInt4_handler)
{
    e4 = 1;
    if((e1+e2+e3+e4) == 4 )
    {
        status = 40;
    }
}

void edgeSensorInit()
{
    //Start 500kHz clock
    CLK_500kHz_Start();
    //Start 24MHz clock
    CLK_24MHz_Start();
    //Start trigger
    triggerPWM_Start();
    //Start counters
    pulseCounter_Start();
    //Start interrupts
    pulseInt_StartEx(pulseInt_handler);
    echoInt_1_StartEx(echoInt1_handler);
    echoInt_2_StartEx(echoInt2_handler);
    echoInt_3_StartEx(echoInt3_handler);
    echoInt_4_StartEx(echoInt4_handler);
}

int getStatus()
{
    int tempStatus = status;
    
    //Resetting variables
    status = 0;
    e1 = 0;
    e2 = 0;
    e3 = 0;
    e4 = 0;
    
    return tempStatus;
}