#include "waitingState.h"
#include "rpiCommunication.h"
#include "scale.h"
#include "soundApp.h"
#include "LedHandler.h"

void waitingStateStart()
{
    if(readScaleStatus() != 0 )
    {
        ledHandler(WEIGHTON);
        
        if(rpiRunning == 1)
        {
            sendToSOMO(1);
            ledHandler(START);
            CyDelay(5000);
            running_ = 1;
        }
    }
    else
    {
        ledHandler(WEIGHTOFF);
    }
}

void waitingStateDrink()
{
    running_ = 0;
    
    int ScaleOff = 0;
    
    ledHandler(WEIGHTON);
    
    while(readScaleStatus() == 1)
    {
       //wait
    }
    
    ledHandler(WEIGHTOFF);
    
    while(readScaleStatus() != 1)
    {
        //wait
    }
    
    ledHandler(WEIGHTON);
    

    CyDelay(2000);
    
    running_ = 1;
}