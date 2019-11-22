#include "MotorApp.h"
#include "waitingState.h"

/*
Dette er et to dimensionelt array der indeholder de forskellige duty cycles der kan sendes til de to motore. De er opstillet parallelt,
og vil altid køre således. Ved at skifte rundt rundt i første dimension vil beerdroid dreje endten til højre eller venstre.
*/
int driveState[7][2] = {{10, 100},
                        {30, 100},
                        {50, 100},
                        {70, 100},
                        {70, 80},
                        {70, 60},
                        {70, 40}};


//denne funktion skifter tilfældigt rundt i driveState arrayet
void speedSelect()
{    
    if(randState == 0)                      //Når randstate er 0, laves en ny randstate værdi imellem -3 og 3
        randState = rand() % 7 - 3;
    
    if(randState < 0)                       //Hvis randstate er under 0 rykkes en gang ned i arrayet
    {
        changeState(-1);
        randState++;                        //randsdtate forøges så den til sidst rammer 0 og der findes en ny random værdi
    }
    else if(randState > 0)                  //Hvis randstate er over 0 rykkes en gang op i arrayet
    {
        changeState(1);
        randState--;                        //randsdtate trækkes 1 fra så den til sidst rammer 0 og der findes en ny random værdi
    }
}

void changeState(int change)                //Denne funktion sikre at state ikke forlader driveState arrayet
{
  
    
    if(change == 1 || change == -1)
    {
        if(state > 0 && change == -1)
        {
            state += change;
            changeSpeed();
        }
        else if(state < 6 && change == 1)
        {
            state += change;
            changeSpeed();
        } 
    }
}


void driveForwards()                        //Denne funktion sætter motoren til at køre fremad
{
    if(running_ == 1)
    {
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    Pin1_dir1_Write(0);
    Pin1_dir2_Write(1);
    Pin2_dir1_Write(0);
    Pin2_dir2_Write(1);
    
    
    motorPWM_1_WriteCompare(80); //Set speed to current drive state on PWM 1
    motorPWM_2_WriteCompare(80); //Set speed to current drive state on PWM 2
    
    motorPWM_1_Start();
    motorPWM_2_Start();
    }
}

void driveBackwards()                       //Denne funktion sætter motoren til at køre bagud
{
    if(running_ == 1)
    {
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    Pin1_dir1_Write(1);
    Pin1_dir2_Write(0);
    Pin2_dir1_Write(1);
    Pin2_dir2_Write(0);
    
    motorPWM_1_WriteCompare(80); //Set speed to current drive state on PWM 1
    motorPWM_2_WriteCompare(80); //Set speed to current drive state on PWM 2
    
    motorPWM_1_Start();
    motorPWM_2_Start();
    }
}

void motorStop()
{
    
    
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
}

void changeSpeed()                          //Denne funktion ændre placering i driveState, og hermed hvilken retning beerdroid køre
{
    //state = 3;

    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    if(driveState[state][0] > 9 && driveState[state][0] < 71)     //sikre at arrayet ikke forlades
    {
        motorPWM_1_WritePeriod(170);
        motorPWM_1_WriteCompare(driveState[state][0]); //Set speed to current drive state on PWM 1 - driveState[state][0]
    }
    if(driveState[state][1] > 39 && driveState[state][1] < 101)     //sikre at arrayet ikke forlades
    {
        motorPWM_2_WritePeriod(170);
        motorPWM_2_WriteCompare(driveState[state][1]); //Set speed to current drive state on PWM 2 - driveState[state][1]
    }
    
    motorPWM_1_Start();
    motorPWM_2_Start();
}

void DelayBased90Turn(int dir)
{
    if(running_ == 1)
    {
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    CyDelay(250);
    
    switch(dir)
    {
        case 1 :
            Pin1_dir1_Write(0);
            Pin1_dir2_Write(1);
            Pin2_dir1_Write(1);
            Pin2_dir2_Write(0);
            break;
        case 2 :
            Pin1_dir2_Write(0);
            Pin1_dir1_Write(1);
            Pin2_dir2_Write(1);
            Pin2_dir1_Write(0);
            break;
    }
    
    
    motorPWM_1_WriteCompare(100);
    motorPWM_2_WriteCompare(100);
    
    motorPWM_1_Start();
    motorPWM_2_Start();
    
    CyDelay(500);
    
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    CyDelay(250);
    
    driveForwards(); 
    }
}


void DelayBased180Turn()
{
    if(running_ == 1)
    {
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    CyDelay(250);
    
    driveBackwards();
    
    CyDelay(1000);
    
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    CyDelay(250);
    
    Pin1_dir1_Write(0);
    Pin1_dir2_Write(1);
    Pin2_dir1_Write(1);
    Pin2_dir2_Write(0);
    
    motorPWM_1_Start();
    motorPWM_2_Start();
    
    motorPWM_1_WriteCompare(100);
    motorPWM_2_WriteCompare(100);
    
    CyDelay(1000);
    
    motorPWM_1_Stop();
    motorPWM_2_Stop();
    
    CyDelay(250);
    
    driveForwards();
    }
}

/*void setDificulty(int dif)
{
    difficulty = dif;
}*/
