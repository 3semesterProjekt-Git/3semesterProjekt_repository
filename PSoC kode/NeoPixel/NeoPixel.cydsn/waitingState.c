#include "waitingState.h"
#include "rpiCommunication.h"
#include "scale.h"
#include "soundApp.h"

void waitingStateStart()
{
    if(rpiRunning == 1)
    {
        
        if(readScaleStatus() != 0)
        {
            sendToSOMO(1);
            CyDelay(5000);
            running_ = 1;
        }
    }
}

void waitingStateDrink()
{
    running_ = 0;
    while(readScaleStatus() == 1)
    {
        //wait
    }
    while(readScaleStatus()!= 1)
    {
        //wait
    }

    CyDelay(3000);
    
    running_ = 1;
}