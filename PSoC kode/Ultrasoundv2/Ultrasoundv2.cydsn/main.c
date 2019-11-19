#include "edgeSensorApp.h"
#include "project.h"
#include "MotorApp.h"
#include "rpiCommunication.h"
#include "scale.h"
#include "waitingState.h"
#include "soundApp.h"

CY_ISR_PROTO(ball_int_handler);

int main(void)
{
    running_ = 0;
    rpiRunning = 1;
    CyGlobalIntEnable; //init global int
    ball_int_StartEx(ball_int_handler);
    //init drivers
    edgeSensorInit();
    rpiInit();
    soundInit();
    
    ADC_SAR_1_Start();  //initialisere ADC'en.
    
    ADC_SAR_1_StartConvert();   //Starter ADC konverteringen
    
    
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
    running_ = 0;
    
    motorStop();
    sendToSOMO(3);
    CyDelay(2000);
    
    waitingStateDrink();
    
    running_ = 1;
    
}