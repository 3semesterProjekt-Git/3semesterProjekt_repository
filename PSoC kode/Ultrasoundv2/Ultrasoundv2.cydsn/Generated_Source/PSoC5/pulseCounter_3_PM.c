/*******************************************************************************
* File Name: pulseCounter_3_PM.c  
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

#include "pulseCounter_3.h"

static pulseCounter_3_backupStruct pulseCounter_3_backup;


/*******************************************************************************
* Function Name: pulseCounter_3_SaveConfig
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
*  pulseCounter_3_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void pulseCounter_3_SaveConfig(void) 
{
    #if (!pulseCounter_3_UsingFixedFunction)

        pulseCounter_3_backup.CounterUdb = pulseCounter_3_ReadCounter();

        #if(!pulseCounter_3_ControlRegRemoved)
            pulseCounter_3_backup.CounterControlRegister = pulseCounter_3_ReadControlRegister();
        #endif /* (!pulseCounter_3_ControlRegRemoved) */

    #endif /* (!pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_RestoreConfig
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
*  pulseCounter_3_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_3_RestoreConfig(void) 
{      
    #if (!pulseCounter_3_UsingFixedFunction)

       pulseCounter_3_WriteCounter(pulseCounter_3_backup.CounterUdb);

        #if(!pulseCounter_3_ControlRegRemoved)
            pulseCounter_3_WriteControlRegister(pulseCounter_3_backup.CounterControlRegister);
        #endif /* (!pulseCounter_3_ControlRegRemoved) */

    #endif /* (!pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_Sleep
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
*  pulseCounter_3_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void pulseCounter_3_Sleep(void) 
{
    #if(!pulseCounter_3_ControlRegRemoved)
        /* Save Counter's enable state */
        if(pulseCounter_3_CTRL_ENABLE == (pulseCounter_3_CONTROL & pulseCounter_3_CTRL_ENABLE))
        {
            /* Counter is enabled */
            pulseCounter_3_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            pulseCounter_3_backup.CounterEnableState = 0u;
        }
    #else
        pulseCounter_3_backup.CounterEnableState = 1u;
        if(pulseCounter_3_backup.CounterEnableState != 0u)
        {
            pulseCounter_3_backup.CounterEnableState = 0u;
        }
    #endif /* (!pulseCounter_3_ControlRegRemoved) */
    
    pulseCounter_3_Stop();
    pulseCounter_3_SaveConfig();
}


/*******************************************************************************
* Function Name: pulseCounter_3_Wakeup
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
*  pulseCounter_3_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_3_Wakeup(void) 
{
    pulseCounter_3_RestoreConfig();
    #if(!pulseCounter_3_ControlRegRemoved)
        if(pulseCounter_3_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            pulseCounter_3_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!pulseCounter_3_ControlRegRemoved) */
    
}


/* [] END OF FILE */
