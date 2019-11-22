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
    rpiRunning = 0;
    CyGlobalIntEnable; //init global int
    ball_int_StartEx(ball_int_handler);
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
            Pin_running_Write(1);
            speedSelect();
            CyDelay(250);  // + timeDificulty[difficulty]
            if(rand() % 20 == 4)
            {
                score();
            }
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
        CyDelay(2000);
    
        //waitingStateDrink();
    
        running_ = 1;
    }
}