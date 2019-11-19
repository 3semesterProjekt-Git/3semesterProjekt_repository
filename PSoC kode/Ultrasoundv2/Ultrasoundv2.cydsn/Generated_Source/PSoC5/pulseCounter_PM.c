/*******************************************************************************
* File Name: pulseCounter_PM.c  
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

#include "pulseCounter.h"

static pulseCounter_backupStruct pulseCounter_backup;


/*******************************************************************************
* Function Name: pulseCounter_SaveConfig
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
*  pulseCounter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void pulseCounter_SaveConfig(void) 
{
    #if (!pulseCounter_UsingFixedFunction)

        pulseCounter_backup.CounterUdb = pulseCounter_ReadCounter();

        #if(!pulseCounter_ControlRegRemoved)
            pulseCounter_backup.CounterControlRegister = pulseCounter_ReadControlRegister();
        #endif /* (!pulseCounter_ControlRegRemoved) */

    #endif /* (!pulseCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_RestoreConfig
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
*  pulseCounter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_RestoreConfig(void) 
{      
    #if (!pulseCounter_UsingFixedFunction)

       pulseCounter_WriteCounter(pulseCounter_backup.CounterUdb);

        #if(!pulseCounter_ControlRegRemoved)
            pulseCounter_WriteControlRegister(pulseCounter_backup.CounterControlRegister);
        #endif /* (!pulseCounter_ControlRegRemoved) */

    #endif /* (!pulseCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_Sleep
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
*  pulseCounter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void pulseCounter_Sleep(void) 
{
    #if(!pulseCounter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(pulseCounter_CTRL_ENABLE == (pulseCounter_CONTROL & pulseCounter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            pulseCounter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            pulseCounter_backup.CounterEnableState = 0u;
        }
    #else
        pulseCounter_backup.CounterEnableState = 1u;
        if(pulseCounter_backup.CounterEnableState != 0u)
        {
            pulseCounter_backup.CounterEnableState = 0u;
        }
    #endif /* (!pulseCounter_ControlRegRemoved) */
    
    pulseCounter_Stop();
    pulseCounter_SaveConfig();
}


/*******************************************************************************
* Function Name: pulseCounter_Wakeup
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
*  pulseCounter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_Wakeup(void) 
{
    pulseCounter_RestoreConfig();
    #if(!pulseCounter_ControlRegRemoved)
        if(pulseCounter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            pulseCounter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!pulseCounter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
