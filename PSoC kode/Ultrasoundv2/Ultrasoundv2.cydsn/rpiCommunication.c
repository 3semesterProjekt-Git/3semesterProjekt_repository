#include "rpiCommunication.h"
#include "soundApp.h"
#include "project.h"

CY_ISR_PROTO(isr_rpi_start)
{
   rpiStart();    
}
CY_ISR_PROTO(isr_rpi_stop)
{
    rpiStop();
    sendToSOMO(2);
}

void score()
{
    Pin_score_Write(0);
    CyDelay(2000);
    Pin_score_Write(1);
}

void rpiStart()
{
    rpiRunning = 1;
}

void rpiStop()
{
    rpiRunning = 0;
    
}

void rpiInit()
{
    Pin_score_Write(1);
}
