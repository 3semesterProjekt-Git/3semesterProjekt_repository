/*******************************************************************************
* File Name: pulseInt_3.h
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
#if !defined(CY_ISR_pulseInt_3_H)
#define CY_ISR_pulseInt_3_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void pulseInt_3_Start(void);
void pulseInt_3_StartEx(cyisraddress address);
void pulseInt_3_Stop(void);

CY_ISR_PROTO(pulseInt_3_Interrupt);

void pulseInt_3_SetVector(cyisraddress address);
cyisraddress pulseInt_3_GetVector(void);

void pulseInt_3_SetPriority(uint8 priority);
uint8 pulseInt_3_GetPriority(void);

void pulseInt_3_Enable(void);
uint8 pulseInt_3_GetState(void);
void pulseInt_3_Disable(void);

void pulseInt_3_SetPending(void);
void pulseInt_3_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the pulseInt_3 ISR. */
#define pulseInt_3_INTC_VECTOR            ((reg32 *) pulseInt_3__INTC_VECT)

/* Address of the pulseInt_3 ISR priority. */
#define pulseInt_3_INTC_PRIOR             ((reg8 *) pulseInt_3__INTC_PRIOR_REG)

/* Priority of the pulseInt_3 interrupt. */
#define pulseInt_3_INTC_PRIOR_NUMBER      pulseInt_3__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable pulseInt_3 interrupt. */
#define pulseInt_3_INTC_SET_EN            ((reg32 *) pulseInt_3__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the pulseInt_3 interrupt. */
#define pulseInt_3_INTC_CLR_EN            ((reg32 *) pulseInt_3__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the pulseInt_3 interrupt state to pending. */
#define pulseInt_3_INTC_SET_PD            ((reg32 *) pulseInt_3__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the pulseInt_3 interrupt. */
#define pulseInt_3_INTC_CLR_PD            ((reg32 *) pulseInt_3__INTC_CLR_PD_REG)


#endif /* CY_ISR_pulseInt_3_H */


/* [] END OF FILE */
