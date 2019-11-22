/*******************************************************************************
* File Name: pulseCounter_1_PM.c  
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

#include "pulseCounter_1.h"

static pulseCounter_1_backupStruct pulseCounter_1_backup;


/*******************************************************************************
* Function Name: pulseCounter_1_SaveConfig
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
*  pulseCounter_1_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void pulseCounter_1_SaveConfig(void) 
{
    #if (!pulseCounter_1_UsingFixedFunction)

        pulseCounter_1_backup.CounterUdb = pulseCounter_1_ReadCounter();

        #if(!pulseCounter_1_ControlRegRemoved)
            pulseCounter_1_backup.CounterControlRegister = pulseCounter_1_ReadControlRegister();
        #endif /* (!pulseCounter_1_ControlRegRemoved) */

    #endif /* (!pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_RestoreConfig
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
*  pulseCounter_1_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_1_RestoreConfig(void) 
{      
    #if (!pulseCounter_1_UsingFixedFunction)

       pulseCounter_1_WriteCounter(pulseCounter_1_backup.CounterUdb);

        #if(!pulseCounter_1_ControlRegRemoved)
            pulseCounter_1_WriteControlRegister(pulseCounter_1_backup.CounterControlRegister);
        #endif /* (!pulseCounter_1_ControlRegRemoved) */

    #endif /* (!pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_Sleep
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
*  pulseCounter_1_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void pulseCounter_1_Sleep(void) 
{
    #if(!pulseCounter_1_ControlRegRemoved)
        /* Save Counter's enable state */
        if(pulseCounter_1_CTRL_ENABLE == (pulseCounter_1_CONTROL & pulseCounter_1_CTRL_ENABLE))
        {
            /* Counter is enabled */
            pulseCounter_1_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            pulseCounter_1_backup.CounterEnableState = 0u;
        }
    #else
        pulseCounter_1_backup.CounterEnableState = 1u;
        if(pulseCounter_1_backup.CounterEnableState != 0u)
        {
            pulseCounter_1_backup.CounterEnableState = 0u;
        }
    #endif /* (!pulseCounter_1_ControlRegRemoved) */
    
    pulseCounter_1_Stop();
    pulseCounter_1_SaveConfig();
}


/*******************************************************************************
* Function Name: pulseCounter_1_Wakeup
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
*  pulseCounter_1_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_1_Wakeup(void) 
{
    pulseCounter_1_RestoreConfig();
    #if(!pulseCounter_1_ControlRegRemoved)
        if(pulseCounter_1_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            pulseCounter_1_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!pulseCounter_1_ControlRegRemoved) */
    
}


/* [] END OF FILE */
