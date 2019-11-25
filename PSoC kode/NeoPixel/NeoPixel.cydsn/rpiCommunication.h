#pragma once
#include "project.h"

int rpiRunning;

CY_ISR_PROTO(isr_rpi_start);
CY_ISR_PROTO(isr_rpi_stop);

void score();
void rpiStart();
void handleStart();
void rpiStop();
void rpiInit();


