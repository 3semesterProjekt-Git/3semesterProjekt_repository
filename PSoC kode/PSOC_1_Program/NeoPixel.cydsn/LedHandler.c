#include "LedHandler.h"

void ledHandler(int state)
{
    switch (state)
    {
        case 0:
            StartSequence();
            break;
        case 1:
            StopSequence();
            break;
        case 2:
            CupHitSequence();
            break;
        case 3:
            WeightSequence();
            break;
        default:
            break;
        
    }
}
void TurnOff(int startPos, int amount)
{ 
    for(int i = startPos; i < startPos + amount; i++)
    {
        pPixelArray[GREEN + i * 3] = (unsigned int)0.0;
        pPixelArray[BLUE + i * 3] = (unsigned int)0.0;
        pPixelArray[RED + i * 3] = (unsigned int)0.0;
    }
}
void TurnOn(int startPos, int amount, int color, int intensity)
{
    if(startPos+amount > MAX)
    {
        amount = MAX - startPos;
    }
    for(int i = startPos; i < startPos + amount; i++)
    {
        pPixelArray[color + i * 3] = (unsigned int) intensity;
    }
}
void StopSequence()
{
    for(int i = 0; i < 10; i++)
    {
        TurnOff(0, MAX);
        NP_Update();
        CyDelay(500);
        TurnOn(0, MAX, RED, INTENSITY);
        NP_Update();
        CyDelay(500);
    }
}
void WeightSequence()
{
    TurnOff(0, MAX);
    TurnOn(0,MAX,RED,INTENSITY);
    TurnOn(0,MAX,GREEN,INTENSITY);
    NP_Update();
    TurnOff(0, MAX);
    TurnOn(0, MAX, GREEN, INTENSITY);
    NP_Update();
}
void CupHitSequence()
{
    int offset=MAX/2;
    for(int i = 0; i < MAX*10; i++)
    {   
        TurnOff(0, MAX);
        TurnOn(i%MAX,3,GREEN,INTENSITY);
        //if the second pixel flow is above max start from the first pos
        if(i%MAX+offset > MAX)
        {
            TurnOn(i%MAX-offset,3,GREEN,INTENSITY);
        }
        else
        {
            TurnOn(i%MAX + offset,3,GREEN,INTENSITY);
        }
        //overflow of 1 pixel
        if(i%MAX == MAX-2 || i%MAX+offset == MAX-2)
        {
            TurnOn(0,1,GREEN,INTENSITY);
        }
        //overflow of 2 pixels
        if(i%MAX == MAX-1 || i%MAX+offset == MAX-1)
        {
            TurnOn(0,2,GREEN,INTENSITY);
        }
        NP_Update();
        CyDelay(25);
    }
    TurnOn(0,MAX,GREEN,INTENSITY);
    NP_Update();
}
void StartSequence()
{
    TurnOff(0, MAX);
    //red
    TurnOn(0,MAX/4,RED, INTENSITY);
    NP_Update();
    CyDelay(1000);
    //yellow
    for(int i = 1; i <= 2; i++)
    {
        TurnOn(MAX/4*i,MAX/4,RED, INTENSITY);
        TurnOn(MAX/4*i,MAX/4,GREEN, INTENSITY);
        NP_Update();
        CyDelay(1000);
    }
    //green
    TurnOn(MAX/4*3,MAX/4,GREEN, INTENSITY);
    NP_Update();
    CyDelay(1000);
    TurnOff(0, MAX);
    for(int i = 0; i < MAX; i++)
    {
        TurnOn(i,1,GREEN, INTENSITY);
        NP_Update();
        CyDelay(25);
    }
    for(int i = 0; i < 4; i++)
    {
        TurnOff(0, MAX);
        NP_Update();
        CyDelay(50);
        TurnOn(0,MAX,GREEN, INTENSITY);
        NP_Update();
        CyDelay(50);
    }
}