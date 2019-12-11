#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

#include "waitingState.h"
#include "project.h"

int state;          //angiver state i første diminsion af driveState arrayet
int randState;      //bruges til at angive en tilfældig værdi der får beerdroid til at køre tilfældigt

void speedSelect();
void changeState(int change);
void driveForwards();
void driveBackwards();
void motorStop();
void changeSpeed();
void DelayBased90Turn();
void DelayBased180Turn();