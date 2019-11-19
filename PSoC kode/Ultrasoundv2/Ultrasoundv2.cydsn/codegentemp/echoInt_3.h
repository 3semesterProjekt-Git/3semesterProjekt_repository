/*******************************************************************************
* File Name: echoInt_3.h
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
#if !defined(CY_ISR_echoInt_3_H)
#define CY_ISR_echoInt_3_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void echoInt_3_Start(void);
void echoInt_3_StartEx(cyisraddress address);
void echoInt_3_Stop(void);

CY_ISR_PROTO(echoInt_3_Interrupt);

void echoInt_3_SetVector(cyisraddress address);
cyisraddress echoInt_3_GetVector(void);

void echoInt_3_SetPriority(uint8 priority);
uint8 echoInt_3_GetPriority(void);

void echoInt_3_Enable(void);
uint8 echoInt_3_GetState(void);
void echoInt_3_Disable(void);

void echoInt_3_SetPending(void);
void echoInt_3_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the echoInt_3 ISR. */
#define echoInt_3_INTC_VECTOR            ((reg32 *) echoInt_3__INTC_VECT)

/* Address of the echoInt_3 ISR priority. */
#define echoInt_3_INTC_PRIOR             ((reg8 *) echoInt_3__INTC_PRIOR_REG)

/* Priority of the echoInt_3 interrupt. */
#define echoInt_3_INTC_PRIOR_NUMBER      echoInt_3__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable echoInt_3 interrupt. */
#define echoInt_3_INTC_SET_EN            ((reg32 *) echoInt_3__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the echoInt_3 interrupt. */
#define echoInt_3_INTC_CLR_EN            ((reg32 *) echoInt_3__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the echoInt_3 interrupt state to pending. */
#define echoInt_3_INTC_SET_PD            ((reg32 *) echoInt_3__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the echoInt_3 interrupt. */
#define echoInt_3_INTC_CLR_PD            ((reg32 *) echoInt_3__INTC_CLR_PD_REG)


#endif /* CY_ISR_echoInt_3_H */


/* [] END OF FILE */
