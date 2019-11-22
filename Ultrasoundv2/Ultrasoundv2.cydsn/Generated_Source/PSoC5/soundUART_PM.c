/*******************************************************************************
* File Name: soundUART_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "soundUART.h"


/***************************************
* Local data allocation
***************************************/

static soundUART_BACKUP_STRUCT  soundUART_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: soundUART_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the soundUART_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  soundUART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void soundUART_SaveConfig(void)
{
    #if(soundUART_CONTROL_REG_REMOVED == 0u)
        soundUART_backup.cr = soundUART_CONTROL_REG;
    #endif /* End soundUART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: soundUART_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  soundUART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling soundUART_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void soundUART_RestoreConfig(void)
{
    #if(soundUART_CONTROL_REG_REMOVED == 0u)
        soundUART_CONTROL_REG = soundUART_backup.cr;
    #endif /* End soundUART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: soundUART_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The soundUART_Sleep() API saves the current component state. Then it
*  calls the soundUART_Stop() function and calls 
*  soundUART_SaveConfig() to save the hardware configuration.
*  Call the soundUART_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  soundUART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void soundUART_Sleep(void)
{
    #if(soundUART_RX_ENABLED || soundUART_HD_ENABLED)
        if((soundUART_RXSTATUS_ACTL_REG  & soundUART_INT_ENABLE) != 0u)
        {
            soundUART_backup.enableState = 1u;
        }
        else
        {
            soundUART_backup.enableState = 0u;
        }
    #else
        if((soundUART_TXSTATUS_ACTL_REG  & soundUART_INT_ENABLE) !=0u)
        {
            soundUART_backup.enableState = 1u;
        }
        else
        {
            soundUART_backup.enableState = 0u;
        }
    #endif /* End soundUART_RX_ENABLED || soundUART_HD_ENABLED*/

    soundUART_Stop();
    soundUART_SaveConfig();
}


/*******************************************************************************
* Function Name: soundUART_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  soundUART_Sleep() was called. The soundUART_Wakeup() function
*  calls the soundUART_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  soundUART_Sleep() function was called, the soundUART_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  soundUART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void soundUART_Wakeup(void)
{
    soundUART_RestoreConfig();
    #if( (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) )
        soundUART_ClearRxBuffer();
    #endif /* End (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) */
    #if(soundUART_TX_ENABLED || soundUART_HD_ENABLED)
        soundUART_ClearTxBuffer();
    #endif /* End soundUART_TX_ENABLED || soundUART_HD_ENABLED */

    if(soundUART_backup.enableState != 0u)
    {
        soundUART_Enable();
    }
}


/* [] END OF FILE */
