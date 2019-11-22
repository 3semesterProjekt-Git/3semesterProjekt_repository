/*******************************************************************************
* File Name: motorPWM_PM.c
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

#include "motorPWM.h"

static motorPWM_backupStruct motorPWM_backup;


/*******************************************************************************
* Function Name: motorPWM_SaveConfig
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
*  motorPWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motorPWM_SaveConfig(void) 
{

    #if(!motorPWM_UsingFixedFunction)
        #if(!motorPWM_PWMModeIsCenterAligned)
            motorPWM_backup.PWMPeriod = motorPWM_ReadPeriod();
        #endif /* (!motorPWM_PWMModeIsCenterAligned) */
        motorPWM_backup.PWMUdb = motorPWM_ReadCounter();
        #if (motorPWM_UseStatus)
            motorPWM_backup.InterruptMaskValue = motorPWM_STATUS_MASK;
        #endif /* (motorPWM_UseStatus) */

        #if(motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_256_CLOCKS || \
            motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_2_4_CLOCKS)
            motorPWM_backup.PWMdeadBandValue = motorPWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motorPWM_KillModeMinTime)
             motorPWM_backup.PWMKillCounterPeriod = motorPWM_ReadKillTime();
        #endif /* (motorPWM_KillModeMinTime) */

        #if(motorPWM_UseControl)
            motorPWM_backup.PWMControlRegister = motorPWM_ReadControlRegister();
        #endif /* (motorPWM_UseControl) */
    #endif  /* (!motorPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorPWM_RestoreConfig
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
*  motorPWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motorPWM_RestoreConfig(void) 
{
        #if(!motorPWM_UsingFixedFunction)
            #if(!motorPWM_PWMModeIsCenterAligned)
                motorPWM_WritePeriod(motorPWM_backup.PWMPeriod);
            #endif /* (!motorPWM_PWMModeIsCenterAligned) */

            motorPWM_WriteCounter(motorPWM_backup.PWMUdb);

            #if (motorPWM_UseStatus)
                motorPWM_STATUS_MASK = motorPWM_backup.InterruptMaskValue;
            #endif /* (motorPWM_UseStatus) */

            #if(motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_256_CLOCKS || \
                motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_2_4_CLOCKS)
                motorPWM_WriteDeadTime(motorPWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motorPWM_KillModeMinTime)
                motorPWM_WriteKillTime(motorPWM_backup.PWMKillCounterPeriod);
            #endif /* (motorPWM_KillModeMinTime) */

            #if(motorPWM_UseControl)
                motorPWM_WriteControlRegister(motorPWM_backup.PWMControlRegister);
            #endif /* (motorPWM_UseControl) */
        #endif  /* (!motorPWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motorPWM_Sleep
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
*  motorPWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motorPWM_Sleep(void) 
{
    #if(motorPWM_UseControl)
        if(motorPWM_CTRL_ENABLE == (motorPWM_CONTROL & motorPWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            motorPWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motorPWM_backup.PWMEnableState = 0u;
        }
    #endif /* (motorPWM_UseControl) */

    /* Stop component */
    motorPWM_Stop();

    /* Save registers configuration */
    motorPWM_SaveConfig();
}


/*******************************************************************************
* Function Name: motorPWM_Wakeup
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
*  motorPWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motorPWM_Wakeup(void) 
{
     /* Restore registers values */
    motorPWM_RestoreConfig();

    if(motorPWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motorPWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
