#include "project.h"

uint16_t result;
char uartBuffer[256];   //En buffer til håndteringen af data.
uint16_t measurement, count_ok, count_not_ok, status_scale;

int readScaleStatus(void);
void clearScaleVariables();
