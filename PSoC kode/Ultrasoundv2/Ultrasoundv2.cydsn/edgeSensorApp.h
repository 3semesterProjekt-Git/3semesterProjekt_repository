#include "project.h"
#include "MotorApp.h"

//Routine starts on echo falling edge
CY_ISR_PROTO(pulseInt_handler);
CY_ISR_PROTO(echoInt1_handler);
CY_ISR_PROTO(echoInt2_handler);
CY_ISR_PROTO(echoInt3_handler);
CY_ISR_PROTO(echoInt4_handler);

void edgeSensorInit();
int getStatus();