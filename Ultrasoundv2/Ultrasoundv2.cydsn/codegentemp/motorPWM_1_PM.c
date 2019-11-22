/*******************************************************************************
* File Name: motorPWM_1_PM.c
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

#include "motorPWM_1.h"

static motorPWM_1_backupStruct motorPWM_1_backup;


/*******************************************************************************
* Function Name: motorPWM_1_SaveConfig
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
*  motorPWM_1_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motorPWM_1_SaveConfig(void) 
{

    #if(!motorPWM_1_UsingFixedFunction)
        #if(!motorPWM_1_PWMModeIsCenterAligned)
            motorPWM_1_backup.PWMPeriod = motorPWM_1_ReadPeriod();
        #endif /* (!motorPWM_1_PWMModeIsCenterAligned) */
        motorPWM_1_backup.PWMUdb = motorPWM_1_ReadCounter();
        #if (motorPWM_1_UseStatus)
            motorPWM_1_backup.InterruptMaskValue = motorPWM_1_STATUS_MASK;
        #endif /* (motorPWM_1_UseStatus) */

        #if(motorPWM_1_DeadBandMode == motorPWM_1__B_PWM__DBM_256_CLOCKS || \
            motorPWM_1_DeadBandMode == motorPWM_1__B_PWM__DBM_2_4_CLOCKS)
            motorPWM_1_backup.PWMdeadBandValue = motorPWM_1_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motorPWM_1_KillModeMinTime)
             motorPWM_1_backup.PWMKillCounterPeriod = motorPWM_1_ReadKillTime();
        #endif /* (motorPWM_1_KillModeMinTime) */

        #if(motorPWM_1_UseControl)
            motorPWM_1_backup.PWMControlRegister = motorPWM_1_ReadControlRegister();
        #endif /* (motorPWM_1_UseControl) */
    #endif  /* (!motorPWM_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorPWM_1_RestoreConfig
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
*  motorPWM_1_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motorPWM_1_RestoreConfig(void) 
{
        #if(!motorPWM_1_UsingFixedFunction)
            #if(!motorPWM_1_PWMModeIsCenterAligned)
                motorPWM_1_WritePeriod(motorPWM_1_backup.PWMPeriod);
            #endif /* (!motorPWM_1_PWMModeIsCenterAligned) */

            motorPWM_1_WriteCounter(motorPWM_1_backup.PWMUdb);

            #if (motorPWM_1_UseStatus)
                motorPWM_1_STATUS_MASK = motorPWM_1_backup.InterruptMaskValue;
            #endif /* (motorPWM_1_UseStatus) */

            #if(motorPWM_1_DeadBandMode == motorPWM_1__B_PWM__DBM_256_CLOCKS || \
                motorPWM_1_DeadBandMode == motorPWM_1__B_PWM__DBM_2_4_CLOCKS)
                motorPWM_1_WriteDeadTime(motorPWM_1_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motorPWM_1_KillModeMinTime)
                motorPWM_1_WriteKillTime(motorPWM_1_backup.PWMKillCounterPeriod);
            #endif /* (motorPWM_1_KillModeMinTime) */

            #if(motorPWM_1_UseControl)
                motorPWM_1_WriteControlRegister(motorPWM_1_backup.PWMControlRegister);
            #endif /* (motorPWM_1_UseControl) */
        #endif  /* (!motorPWM_1_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motorPWM_1_Sleep
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
*  motorPWM_1_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motorPWM_1_Sleep(void) 
{
    #if(motorPWM_1_UseControl)
        if(motorPWM_1_CTRL_ENABLE == (motorPWM_1_CONTROL & motorPWM_1_CTRL_ENABLE))
        {
            /*Component is enabled */
            motorPWM_1_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motorPWM_1_backup.PWMEnableState = 0u;
        }
    #endif /* (motorPWM_1_UseControl) */

    /* Stop component */
    motorPWM_1_Stop();

    /* Save registers configuration */
    motorPWM_1_SaveConfig();
}


/*******************************************************************************
* Function Name: motorPWM_1_Wakeup
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
*  motorPWM_1_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motorPWM_1_Wakeup(void) 
{
     /* Restore registers values */
    motorPWM_1_RestoreConfig();

    if(motorPWM_1_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motorPWM_1_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
