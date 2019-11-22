#include <project.h>
//defines
#define MAX 20 //20 is the strip length
#define RED 1
#define BLUE 2
#define GREEN 0
#define INTENSITY 25 //max 255, 0 is off
//Allocate an array for MAX NeoPixels
unsigned char pPixelArray[MAX*3]; //amount of pixels * times RGB
//states
enum {START,STOP,CUPHIT, WEIGHT}; //enums for function call
//forward declaration
void ledHandler(int);
void WeightSequence();
void StartSequence();
void CupHitSequence();
void StopSequence();
void TurnOff(int, int);
void TurnOn(int,int,int,int);