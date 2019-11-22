#include <project.h>
#include "LedHandler.h"
#include "edgeSensorApp.h"
#include "project.h"
#include "MotorApp.h"
#include "rpiCommunication.h"
#include "scale.h"
#include "waitingState.h"
#include "soundApp.h"

CY_ISR_PROTO(ball_int_handler);
CY_ISR_PROTO(isr_rpi_start);
CY_ISR_PROTO(isr_rpi_stop);

int main(void)
{
    NP_Start(MAX, pPixelArray, 12.8);
    //test prog
 
    //ledHandler(STOP);
    //CyDelay(2000);
    //ledHandler(WEIGHT);
    
    
    
    
    running_ = 0;
    //rpiRunning = 0;
    CyGlobalIntEnable; //init global int
    ball_int_StartEx(ball_int_handler);
    isr_rpi_start_StartEx(isr_rpi_start);
    isr_rpi_stop_StartEx(isr_rpi_stop);
    //init drivers
    edgeSensorInit();
    rpiInit();
    soundInit();
    
    ADC_SAR_1_Start();  //initialisere ADC'en
    
    state = 3;
    
    for(;;)
    {  
        while(running_ == 1)
        {
            speedSelect();
            CyDelay(250);  // + timeDificulty[difficulty]
        }
        while(running_ == 0)
        {
            waitingStateStart();
        }
    }
}

CY_ISR(ball_int_handler)
{
    if(running_ == 1)
    {
        running_ = 0;
    
        motorStop();
        sendToSOMO(3);
        ledHandler(CUPHIT);
        score();
        CyDelay(2000);
    
        waitingStateDrink();
    }
}