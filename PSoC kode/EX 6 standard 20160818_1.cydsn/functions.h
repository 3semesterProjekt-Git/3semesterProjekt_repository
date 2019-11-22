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

/* [] END OF FILE */



#include <project.h>
#include <stdio.h>
#include <math.h>

#define n 1
#define N_Sample 10
#define ballEffect 0.07

int16 	V_Sample[n];
int32 	V_Sample_SUM_1;
float 	V_Sample_SUM_2;
int16 	V_Sample_MIN;
int16 	V_Sample_MAX;
int16 	V_Sample_AVG;
float 	V_Sample_VAR;
float 	V_Sample_AVG_float;
uint16 	i;
char value[255];   
char numberOfMeasurements[255];
int measurements;
float max_value;    

void DMA_1_Config();
void DMA_2_Config();
void Init();
void CompareValues();
float Measure();
CY_ISR(Sample_int);

