/*******************************************************************************
* File Name: pulseCounter_4_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "pulseCounter_4.h"

static pulseCounter_4_backupStruct pulseCounter_4_backup;


/*******************************************************************************
* Function Name: pulseCounter_4_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  pulseCounter_4_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void pulseCounter_4_SaveConfig(void) 
{
    #if (!pulseCounter_4_UsingFixedFunction)

        pulseCounter_4_backup.CounterUdb = pulseCounter_4_ReadCounter();

        #if(!pulseCounter_4_ControlRegRemoved)
            pulseCounter_4_backup.CounterControlRegister = pulseCounter_4_ReadControlRegister();
        #endif /* (!pulseCounter_4_ControlRegRemoved) */

    #endif /* (!pulseCounter_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_4_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  pulseCounter_4_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_4_RestoreConfig(void) 
{      
    #if (!pulseCounter_4_UsingFixedFunction)

       pulseCounter_4_WriteCounter(pulseCounter_4_backup.CounterUdb);

        #if(!pulseCounter_4_ControlRegRemoved)
            pulseCounter_4_WriteControlRegister(pulseCounter_4_backup.CounterControlRegister);
        #endif /* (!pulseCounter_4_ControlRegRemoved) */

    #endif /* (!pulseCounter_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_4_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  pulseCounter_4_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void pulseCounter_4_Sleep(void) 
{
    #if(!pulseCounter_4_ControlRegRemoved)
        /* Save Counter's enable state */
        if(pulseCounter_4_CTRL_ENABLE == (pulseCounter_4_CONTROL & pulseCounter_4_CTRL_ENABLE))
        {
            /* Counter is enabled */
            pulseCounter_4_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            pulseCounter_4_backup.CounterEnableState = 0u;
        }
    #else
        pulseCounter_4_backup.CounterEnableState = 1u;
        if(pulseCounter_4_backup.CounterEnableState != 0u)
        {
            pulseCounter_4_backup.CounterEnableState = 0u;
        }
    #endif /* (!pulseCounter_4_ControlRegRemoved) */
    
    pulseCounter_4_Stop();
    pulseCounter_4_SaveConfig();
}


/*******************************************************************************
* Function Name: pulseCounter_4_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  pulseCounter_4_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_4_Wakeup(void) 
{
    pulseCounter_4_RestoreConfig();
    #if(!pulseCounter_4_ControlRegRemoved)
        if(pulseCounter_4_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            pulseCounter_4_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!pulseCounter_4_ControlRegRemoved) */
    
}


/* [] END OF FILE */
