#include "waitingState.h"
#include "rpiCommunication.h"
#include "scale.h"
#include "soundApp.h"

void waitingStateStart()
{
    if(rpiRunning == 1)
    {
        running_ = 1;
        
        /*if(readScaleStatus() != 0)
        {
            sendToSOMO(1);
            CyDelay(3000);
            running_ = 1;
        }*/
    }
}

void waitingStateDrink()
{
    running_ = 0;
    
    while(readScaleStatus()!= 1)
    {
        
    }
    while(readScaleStatus()== 1)
    {
        
    }
    
    running_ = 1;
}