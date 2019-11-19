/*******************************************************************************
* File Name: pulseInt.h
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
#if !defined(CY_ISR_pulseInt_H)
#define CY_ISR_pulseInt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void pulseInt_Start(void);
void pulseInt_StartEx(cyisraddress address);
void pulseInt_Stop(void);

CY_ISR_PROTO(pulseInt_Interrupt);

void pulseInt_SetVector(cyisraddress address);
cyisraddress pulseInt_GetVector(void);

void pulseInt_SetPriority(uint8 priority);
uint8 pulseInt_GetPriority(void);

void pulseInt_Enable(void);
uint8 pulseInt_GetState(void);
void pulseInt_Disable(void);

void pulseInt_SetPending(void);
void pulseInt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the pulseInt ISR. */
#define pulseInt_INTC_VECTOR            ((reg32 *) pulseInt__INTC_VECT)

/* Address of the pulseInt ISR priority. */
#define pulseInt_INTC_PRIOR             ((reg8 *) pulseInt__INTC_PRIOR_REG)

/* Priority of the pulseInt interrupt. */
#define pulseInt_INTC_PRIOR_NUMBER      pulseInt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable pulseInt interrupt. */
#define pulseInt_INTC_SET_EN            ((reg32 *) pulseInt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the pulseInt interrupt. */
#define pulseInt_INTC_CLR_EN            ((reg32 *) pulseInt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the pulseInt interrupt state to pending. */
#define pulseInt_INTC_SET_PD            ((reg32 *) pulseInt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the pulseInt interrupt. */
#define pulseInt_INTC_CLR_PD            ((reg32 *) pulseInt__INTC_CLR_PD_REG)


#endif /* CY_ISR_pulseInt_H */


/* [] END OF FILE */
