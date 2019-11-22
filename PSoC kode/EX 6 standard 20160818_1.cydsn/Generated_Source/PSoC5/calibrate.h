/*******************************************************************************
* File Name: calibrate.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_calibrate_H)
#define CY_ISR_calibrate_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void calibrate_Start(void);
void calibrate_StartEx(cyisraddress address);
void calibrate_Stop(void);

CY_ISR_PROTO(calibrate_Interrupt);

void calibrate_SetVector(cyisraddress address);
cyisraddress calibrate_GetVector(void);

void calibrate_SetPriority(uint8 priority);
uint8 calibrate_GetPriority(void);

void calibrate_Enable(void);
uint8 calibrate_GetState(void);
void calibrate_Disable(void);

void calibrate_SetPending(void);
void calibrate_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the calibrate ISR. */
#define calibrate_INTC_VECTOR            ((reg32 *) calibrate__INTC_VECT)

/* Address of the calibrate ISR priority. */
#define calibrate_INTC_PRIOR             ((reg8 *) calibrate__INTC_PRIOR_REG)

/* Priority of the calibrate interrupt. */
#define calibrate_INTC_PRIOR_NUMBER      calibrate__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable calibrate interrupt. */
#define calibrate_INTC_SET_EN            ((reg32 *) calibrate__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the calibrate interrupt. */
#define calibrate_INTC_CLR_EN            ((reg32 *) calibrate__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the calibrate interrupt state to pending. */
#define calibrate_INTC_SET_PD            ((reg32 *) calibrate__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the calibrate interrupt. */
#define calibrate_INTC_CLR_PD            ((reg32 *) calibrate__INTC_CLR_PD_REG)


#endif /* CY_ISR_calibrate_H */


/* [] END OF FILE */
