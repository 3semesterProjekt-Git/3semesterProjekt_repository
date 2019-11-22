/*******************************************************************************
* File Name: echoInt_4.h
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
#if !defined(CY_ISR_echoInt_4_H)
#define CY_ISR_echoInt_4_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void echoInt_4_Start(void);
void echoInt_4_StartEx(cyisraddress address);
void echoInt_4_Stop(void);

CY_ISR_PROTO(echoInt_4_Interrupt);

void echoInt_4_SetVector(cyisraddress address);
cyisraddress echoInt_4_GetVector(void);

void echoInt_4_SetPriority(uint8 priority);
uint8 echoInt_4_GetPriority(void);

void echoInt_4_Enable(void);
uint8 echoInt_4_GetState(void);
void echoInt_4_Disable(void);

void echoInt_4_SetPending(void);
void echoInt_4_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the echoInt_4 ISR. */
#define echoInt_4_INTC_VECTOR            ((reg32 *) echoInt_4__INTC_VECT)

/* Address of the echoInt_4 ISR priority. */
#define echoInt_4_INTC_PRIOR             ((reg8 *) echoInt_4__INTC_PRIOR_REG)

/* Priority of the echoInt_4 interrupt. */
#define echoInt_4_INTC_PRIOR_NUMBER      echoInt_4__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable echoInt_4 interrupt. */
#define echoInt_4_INTC_SET_EN            ((reg32 *) echoInt_4__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the echoInt_4 interrupt. */
#define echoInt_4_INTC_CLR_EN            ((reg32 *) echoInt_4__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the echoInt_4 interrupt state to pending. */
#define echoInt_4_INTC_SET_PD            ((reg32 *) echoInt_4__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the echoInt_4 interrupt. */
#define echoInt_4_INTC_CLR_PD            ((reg32 *) echoInt_4__INTC_CLR_PD_REG)


#endif /* CY_ISR_echoInt_4_H */


/* [] END OF FILE */
