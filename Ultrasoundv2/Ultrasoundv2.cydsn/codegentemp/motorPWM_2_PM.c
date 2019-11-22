/*******************************************************************************
* File Name: motorPWM_2_PM.c
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

#include "motorPWM_2.h"

static motorPWM_2_backupStruct motorPWM_2_backup;


/*******************************************************************************
* Function Name: motorPWM_2_SaveConfig
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
*  motorPWM_2_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motorPWM_2_SaveConfig(void) 
{

    #if(!motorPWM_2_UsingFixedFunction)
        #if(!motorPWM_2_PWMModeIsCenterAligned)
            motorPWM_2_backup.PWMPeriod = motorPWM_2_ReadPeriod();
        #endif /* (!motorPWM_2_PWMModeIsCenterAligned) */
        motorPWM_2_backup.PWMUdb = motorPWM_2_ReadCounter();
        #if (motorPWM_2_UseStatus)
            motorPWM_2_backup.InterruptMaskValue = motorPWM_2_STATUS_MASK;
        #endif /* (motorPWM_2_UseStatus) */

        #if(motorPWM_2_DeadBandMode == motorPWM_2__B_PWM__DBM_256_CLOCKS || \
            motorPWM_2_DeadBandMode == motorPWM_2__B_PWM__DBM_2_4_CLOCKS)
            motorPWM_2_backup.PWMdeadBandValue = motorPWM_2_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motorPWM_2_KillModeMinTime)
             motorPWM_2_backup.PWMKillCounterPeriod = motorPWM_2_ReadKillTime();
        #endif /* (motorPWM_2_KillModeMinTime) */

        #if(motorPWM_2_UseControl)
            motorPWM_2_backup.PWMControlRegister = motorPWM_2_ReadControlRegister();
        #endif /* (motorPWM_2_UseControl) */
    #endif  /* (!motorPWM_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorPWM_2_RestoreConfig
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
*  motorPWM_2_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motorPWM_2_RestoreConfig(void) 
{
        #if(!motorPWM_2_UsingFixedFunction)
            #if(!motorPWM_2_PWMModeIsCenterAligned)
                motorPWM_2_WritePeriod(motorPWM_2_backup.PWMPeriod);
            #endif /* (!motorPWM_2_PWMModeIsCenterAligned) */

            motorPWM_2_WriteCounter(motorPWM_2_backup.PWMUdb);

            #if (motorPWM_2_UseStatus)
                motorPWM_2_STATUS_MASK = motorPWM_2_backup.InterruptMaskValue;
            #endif /* (motorPWM_2_UseStatus) */

            #if(motorPWM_2_DeadBandMode == motorPWM_2__B_PWM__DBM_256_CLOCKS || \
                motorPWM_2_DeadBandMode == motorPWM_2__B_PWM__DBM_2_4_CLOCKS)
                motorPWM_2_WriteDeadTime(motorPWM_2_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motorPWM_2_KillModeMinTime)
                motorPWM_2_WriteKillTime(motorPWM_2_backup.PWMKillCounterPeriod);
            #endif /* (motorPWM_2_KillModeMinTime) */

            #if(motorPWM_2_UseControl)
                motorPWM_2_WriteControlRegister(motorPWM_2_backup.PWMControlRegister);
            #endif /* (motorPWM_2_UseControl) */
        #endif  /* (!motorPWM_2_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motorPWM_2_Sleep
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
*  motorPWM_2_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motorPWM_2_Sleep(void) 
{
    #if(motorPWM_2_UseControl)
        if(motorPWM_2_CTRL_ENABLE == (motorPWM_2_CONTROL & motorPWM_2_CTRL_ENABLE))
        {
            /*Component is enabled */
            motorPWM_2_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motorPWM_2_backup.PWMEnableState = 0u;
        }
    #endif /* (motorPWM_2_UseControl) */

    /* Stop component */
    motorPWM_2_Stop();

    /* Save registers configuration */
    motorPWM_2_SaveConfig();
}


/*******************************************************************************
* Function Name: motorPWM_2_Wakeup
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
*  motorPWM_2_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motorPWM_2_Wakeup(void) 
{
     /* Restore registers values */
    motorPWM_2_RestoreConfig();

    if(motorPWM_2_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motorPWM_2_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
