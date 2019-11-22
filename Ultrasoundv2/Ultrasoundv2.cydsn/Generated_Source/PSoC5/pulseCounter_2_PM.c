/*******************************************************************************
* File Name: pulseCounter_2_PM.c  
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

#include "pulseCounter_2.h"

static pulseCounter_2_backupStruct pulseCounter_2_backup;


/*******************************************************************************
* Function Name: pulseCounter_2_SaveConfig
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
*  pulseCounter_2_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void pulseCounter_2_SaveConfig(void) 
{
    #if (!pulseCounter_2_UsingFixedFunction)

        pulseCounter_2_backup.CounterUdb = pulseCounter_2_ReadCounter();

        #if(!pulseCounter_2_ControlRegRemoved)
            pulseCounter_2_backup.CounterControlRegister = pulseCounter_2_ReadControlRegister();
        #endif /* (!pulseCounter_2_ControlRegRemoved) */

    #endif /* (!pulseCounter_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_2_RestoreConfig
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
*  pulseCounter_2_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_2_RestoreConfig(void) 
{      
    #if (!pulseCounter_2_UsingFixedFunction)

       pulseCounter_2_WriteCounter(pulseCounter_2_backup.CounterUdb);

        #if(!pulseCounter_2_ControlRegRemoved)
            pulseCounter_2_WriteControlRegister(pulseCounter_2_backup.CounterControlRegister);
        #endif /* (!pulseCounter_2_ControlRegRemoved) */

    #endif /* (!pulseCounter_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_2_Sleep
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
*  pulseCounter_2_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void pulseCounter_2_Sleep(void) 
{
    #if(!pulseCounter_2_ControlRegRemoved)
        /* Save Counter's enable state */
        if(pulseCounter_2_CTRL_ENABLE == (pulseCounter_2_CONTROL & pulseCounter_2_CTRL_ENABLE))
        {
            /* Counter is enabled */
            pulseCounter_2_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            pulseCounter_2_backup.CounterEnableState = 0u;
        }
    #else
        pulseCounter_2_backup.CounterEnableState = 1u;
        if(pulseCounter_2_backup.CounterEnableState != 0u)
        {
            pulseCounter_2_backup.CounterEnableState = 0u;
        }
    #endif /* (!pulseCounter_2_ControlRegRemoved) */
    
    pulseCounter_2_Stop();
    pulseCounter_2_SaveConfig();
}


/*******************************************************************************
* Function Name: pulseCounter_2_Wakeup
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
*  pulseCounter_2_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void pulseCounter_2_Wakeup(void) 
{
    pulseCounter_2_RestoreConfig();
    #if(!pulseCounter_2_ControlRegRemoved)
        if(pulseCounter_2_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            pulseCounter_2_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!pulseCounter_2_ControlRegRemoved) */
    
}


/* [] END OF FILE */
