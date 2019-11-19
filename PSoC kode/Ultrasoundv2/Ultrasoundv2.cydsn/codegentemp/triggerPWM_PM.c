/*******************************************************************************
* File Name: triggerPWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "triggerPWM.h"

static triggerPWM_backupStruct triggerPWM_backup;


/*******************************************************************************
* Function Name: triggerPWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  triggerPWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void triggerPWM_SaveConfig(void) 
{

    #if(!triggerPWM_UsingFixedFunction)
        #if(!triggerPWM_PWMModeIsCenterAligned)
            triggerPWM_backup.PWMPeriod = triggerPWM_ReadPeriod();
        #endif /* (!triggerPWM_PWMModeIsCenterAligned) */
        triggerPWM_backup.PWMUdb = triggerPWM_ReadCounter();
        #if (triggerPWM_UseStatus)
            triggerPWM_backup.InterruptMaskValue = triggerPWM_STATUS_MASK;
        #endif /* (triggerPWM_UseStatus) */

        #if(triggerPWM_DeadBandMode == triggerPWM__B_PWM__DBM_256_CLOCKS || \
            triggerPWM_DeadBandMode == triggerPWM__B_PWM__DBM_2_4_CLOCKS)
            triggerPWM_backup.PWMdeadBandValue = triggerPWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(triggerPWM_KillModeMinTime)
             triggerPWM_backup.PWMKillCounterPeriod = triggerPWM_ReadKillTime();
        #endif /* (triggerPWM_KillModeMinTime) */

        #if(triggerPWM_UseControl)
            triggerPWM_backup.PWMControlRegister = triggerPWM_ReadControlRegister();
        #endif /* (triggerPWM_UseControl) */
    #endif  /* (!triggerPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: triggerPWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  triggerPWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void triggerPWM_RestoreConfig(void) 
{
        #if(!triggerPWM_UsingFixedFunction)
            #if(!triggerPWM_PWMModeIsCenterAligned)
                triggerPWM_WritePeriod(triggerPWM_backup.PWMPeriod);
            #endif /* (!triggerPWM_PWMModeIsCenterAligned) */

            triggerPWM_WriteCounter(triggerPWM_backup.PWMUdb);

            #if (triggerPWM_UseStatus)
                triggerPWM_STATUS_MASK = triggerPWM_backup.InterruptMaskValue;
            #endif /* (triggerPWM_UseStatus) */

            #if(triggerPWM_DeadBandMode == triggerPWM__B_PWM__DBM_256_CLOCKS || \
                triggerPWM_DeadBandMode == triggerPWM__B_PWM__DBM_2_4_CLOCKS)
                triggerPWM_WriteDeadTime(triggerPWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(triggerPWM_KillModeMinTime)
                triggerPWM_WriteKillTime(triggerPWM_backup.PWMKillCounterPeriod);
            #endif /* (triggerPWM_KillModeMinTime) */

            #if(triggerPWM_UseControl)
                triggerPWM_WriteControlRegister(triggerPWM_backup.PWMControlRegister);
            #endif /* (triggerPWM_UseControl) */
        #endif  /* (!triggerPWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: triggerPWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  triggerPWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void triggerPWM_Sleep(void) 
{
    #if(triggerPWM_UseControl)
        if(triggerPWM_CTRL_ENABLE == (triggerPWM_CONTROL & triggerPWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            triggerPWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            triggerPWM_backup.PWMEnableState = 0u;
        }
    #endif /* (triggerPWM_UseControl) */

    /* Stop component */
    triggerPWM_Stop();

    /* Save registers configuration */
    triggerPWM_SaveConfig();
}


/*******************************************************************************
* Function Name: triggerPWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  triggerPWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void triggerPWM_Wakeup(void) 
{
     /* Restore registers values */
    triggerPWM_RestoreConfig();

    if(triggerPWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        triggerPWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
