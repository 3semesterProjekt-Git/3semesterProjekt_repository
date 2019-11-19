#include "project.h"

    uint16_t result;
    char uartBuffer[256];   //En buffer til håndteringen af data.
    uint16_t measurement, count_ok, count_not_low, count_not_high;

int readScaleStatus(void);
