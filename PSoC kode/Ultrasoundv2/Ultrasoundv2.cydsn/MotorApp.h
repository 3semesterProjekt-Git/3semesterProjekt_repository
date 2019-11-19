#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <project.h>
char buffer[250];
/*int speedDificulty[3] = {0, 20, 40};
int timeDificulty[3] = {0, 250, 1000};*/
int difficulty;

int state;          //angiver state i første diminsion af driveState arrayet
int randState;      //bruges til at angive en tilfældig værdi der får beerdroid til at køre tilfældigt
int randturn;       //bruges til at vælge drejeretning i turnRandom()

void speedSelect();
void changeState(int change);
void driveForwards();
void driveBackwards();
void motorStop();
void changeSpeed();
void DelayBased90Turn();
void DelayBased180Turn();
void setDificulty(int dif);