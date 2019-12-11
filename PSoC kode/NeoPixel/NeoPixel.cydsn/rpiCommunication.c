#include "rpiCommunication.h"
#include "soundApp.h"
#include "project.h"
#include "LedHandler.h"
#include "MotorApp.h"

CY_ISR_PROTO(isr_rpi_start)
{
   rpiStart();    
}
CY_ISR_PROTO(isr_rpi_stop)
{
    rpiStop();
    sendToSOMO(2);
    ledHandler(STOP);
}

void score()
{
    Pin_score_Write(0);
    CyDelay(3000);
    Pin_score_Write(1);
}

void rpiStart()
{
    rpiRunning = 1;
}

void rpiStop()
{
    if(rpiRunning == 1)
    {
        rpiRunning = 0;
        motorStop();
    }
}

void rpiInit()
{
    Pin_score_Write(1);
}
