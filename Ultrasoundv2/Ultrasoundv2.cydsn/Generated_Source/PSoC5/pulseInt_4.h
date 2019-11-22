/*******************************************************************************
* File Name: pulseInt_4.h
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
#if !defined(CY_ISR_pulseInt_4_H)
#define CY_ISR_pulseInt_4_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void pulseInt_4_Start(void);
void pulseInt_4_StartEx(cyisraddress address);
void pulseInt_4_Stop(void);

CY_ISR_PROTO(pulseInt_4_Interrupt);

void pulseInt_4_SetVector(cyisraddress address);
cyisraddress pulseInt_4_GetVector(void);

void pulseInt_4_SetPriority(uint8 priority);
uint8 pulseInt_4_GetPriority(void);

void pulseInt_4_Enable(void);
uint8 pulseInt_4_GetState(void);
void pulseInt_4_Disable(void);

void pulseInt_4_SetPending(void);
void pulseInt_4_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the pulseInt_4 ISR. */
#define pulseInt_4_INTC_VECTOR            ((reg32 *) pulseInt_4__INTC_VECT)

/* Address of the pulseInt_4 ISR priority. */
#define pulseInt_4_INTC_PRIOR             ((reg8 *) pulseInt_4__INTC_PRIOR_REG)

/* Priority of the pulseInt_4 interrupt. */
#define pulseInt_4_INTC_PRIOR_NUMBER      pulseInt_4__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable pulseInt_4 interrupt. */
#define pulseInt_4_INTC_SET_EN            ((reg32 *) pulseInt_4__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the pulseInt_4 interrupt. */
#define pulseInt_4_INTC_CLR_EN            ((reg32 *) pulseInt_4__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the pulseInt_4 interrupt state to pending. */
#define pulseInt_4_INTC_SET_PD            ((reg32 *) pulseInt_4__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the pulseInt_4 interrupt. */
#define pulseInt_4_INTC_CLR_PD            ((reg32 *) pulseInt_4__INTC_CLR_PD_REG)


#endif /* CY_ISR_pulseInt_4_H */


/* [] END OF FILE */
