/*******************************************************************************
* File Name: pulseInt_2.h
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
#if !defined(CY_ISR_pulseInt_2_H)
#define CY_ISR_pulseInt_2_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void pulseInt_2_Start(void);
void pulseInt_2_StartEx(cyisraddress address);
void pulseInt_2_Stop(void);

CY_ISR_PROTO(pulseInt_2_Interrupt);

void pulseInt_2_SetVector(cyisraddress address);
cyisraddress pulseInt_2_GetVector(void);

void pulseInt_2_SetPriority(uint8 priority);
uint8 pulseInt_2_GetPriority(void);

void pulseInt_2_Enable(void);
uint8 pulseInt_2_GetState(void);
void pulseInt_2_Disable(void);

void pulseInt_2_SetPending(void);
void pulseInt_2_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the pulseInt_2 ISR. */
#define pulseInt_2_INTC_VECTOR            ((reg32 *) pulseInt_2__INTC_VECT)

/* Address of the pulseInt_2 ISR priority. */
#define pulseInt_2_INTC_PRIOR             ((reg8 *) pulseInt_2__INTC_PRIOR_REG)

/* Priority of the pulseInt_2 interrupt. */
#define pulseInt_2_INTC_PRIOR_NUMBER      pulseInt_2__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable pulseInt_2 interrupt. */
#define pulseInt_2_INTC_SET_EN            ((reg32 *) pulseInt_2__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the pulseInt_2 interrupt. */
#define pulseInt_2_INTC_CLR_EN            ((reg32 *) pulseInt_2__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the pulseInt_2 interrupt state to pending. */
#define pulseInt_2_INTC_SET_PD            ((reg32 *) pulseInt_2__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the pulseInt_2 interrupt. */
#define pulseInt_2_INTC_CLR_PD            ((reg32 *) pulseInt_2__INTC_CLR_PD_REG)


#endif /* CY_ISR_pulseInt_2_H */


/* [] END OF FILE */
