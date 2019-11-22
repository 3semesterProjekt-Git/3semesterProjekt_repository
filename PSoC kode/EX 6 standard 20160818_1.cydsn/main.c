/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "functions.h"


int main()
{
	Init();

	CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
 
    for(;;)
    {
	    Measure();
        CompareValues();
    
    
        // Følgende linjer er udskrift til UART, for finpudsning til sidst
        snprintf(value, sizeof(value), " V_Sample_avg_float = %f \r ", V_Sample_AVG_float);
        UART_1_PutString(value);
        
        snprintf(numberOfMeasurements, sizeof(numberOfMeasurements), " Antal maalinger = %d \r ", measurements);
        UART_1_PutString(numberOfMeasurements);
        // --------------------------------------------------------------
        
    }
}
/* [] END OF FILE */