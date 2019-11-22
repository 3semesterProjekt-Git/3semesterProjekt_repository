/*******************************************************************************
* File Name: triggerPWM.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "triggerPWM.h"

/* Error message for removed <resource> through optimization */
#ifdef triggerPWM_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* triggerPWM_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 triggerPWM_initVar = 0u;


/*******************************************************************************
* Function Name: triggerPWM_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  triggerPWM_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void triggerPWM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(triggerPWM_initVar == 0u)
    {
        triggerPWM_Init();
        triggerPWM_initVar = 1u;
    }
    triggerPWM_Enable();

}


/*******************************************************************************
* Function Name: triggerPWM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  triggerPWM_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void triggerPWM_Init(void) 
{
    #if (triggerPWM_UsingFixedFunction || triggerPWM_UseControl)
        uint8 ctrl;
    #endif /* (triggerPWM_UsingFixedFunction || triggerPWM_UseControl) */

    #if(!triggerPWM_UsingFixedFunction)
        #if(triggerPWM_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 triggerPWM_interruptState;
        #endif /* (triggerPWM_UseStatus) */
    #endif /* (!triggerPWM_UsingFixedFunction) */

    #if (triggerPWM_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        triggerPWM_CONTROL |= triggerPWM_CFG0_MODE;
        #if (triggerPWM_DeadBand2_4)
            triggerPWM_CONTROL |= triggerPWM_CFG0_DB;
        #endif /* (triggerPWM_DeadBand2_4) */

        ctrl = triggerPWM_CONTROL3 & ((uint8 )(~triggerPWM_CTRL_CMPMODE1_MASK));
        triggerPWM_CONTROL3 = ctrl | triggerPWM_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        triggerPWM_RT1 &= ((uint8)(~triggerPWM_RT1_MASK));
        triggerPWM_RT1 |= triggerPWM_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        triggerPWM_RT1 &= ((uint8)(~triggerPWM_SYNCDSI_MASK));
        triggerPWM_RT1 |= triggerPWM_SYNCDSI_EN;

    #elif (triggerPWM_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = triggerPWM_CONTROL & ((uint8)(~triggerPWM_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~triggerPWM_CTRL_CMPMODE1_MASK));
        triggerPWM_CONTROL = ctrl | triggerPWM_DEFAULT_COMPARE2_MODE |
                                   triggerPWM_DEFAULT_COMPARE1_MODE;
    #endif /* (triggerPWM_UsingFixedFunction) */

    #if (!triggerPWM_UsingFixedFunction)
        #if (triggerPWM_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            triggerPWM_AUX_CONTROLDP0 |= (triggerPWM_AUX_CTRL_FIFO0_CLR);
        #else /* (triggerPWM_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            triggerPWM_AUX_CONTROLDP0 |= (triggerPWM_AUX_CTRL_FIFO0_CLR);
            triggerPWM_AUX_CONTROLDP1 |= (triggerPWM_AUX_CTRL_FIFO0_CLR);
        #endif /* (triggerPWM_Resolution == 8) */

        triggerPWM_WriteCounter(triggerPWM_INIT_PERIOD_VALUE);
    #endif /* (!triggerPWM_UsingFixedFunction) */

    triggerPWM_WritePeriod(triggerPWM_INIT_PERIOD_VALUE);

        #if (triggerPWM_UseOneCompareMode)
            triggerPWM_WriteCompare(triggerPWM_INIT_COMPARE_VALUE1);
        #else
            triggerPWM_WriteCompare1(triggerPWM_INIT_COMPARE_VALUE1);
            triggerPWM_WriteCompare2(triggerPWM_INIT_COMPARE_VALUE2);
        #endif /* (triggerPWM_UseOneCompareMode) */

        #if (triggerPWM_KillModeMinTime)
            triggerPWM_WriteKillTime(triggerPWM_MinimumKillTime);
        #endif /* (triggerPWM_KillModeMinTime) */

        #if (triggerPWM_DeadBandUsed)
            triggerPWM_WriteDeadTime(triggerPWM_INIT_DEAD_TIME);
        #endif /* (triggerPWM_DeadBandUsed) */

    #if (triggerPWM_UseStatus || triggerPWM_UsingFixedFunction)
        triggerPWM_SetInterruptMode(triggerPWM_INIT_INTERRUPTS_MODE);
    #endif /* (triggerPWM_UseStatus || triggerPWM_UsingFixedFunction) */

    #if (triggerPWM_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        triggerPWM_GLOBAL_ENABLE |= triggerPWM_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        triggerPWM_CONTROL2 |= triggerPWM_CTRL2_IRQ_SEL;
    #else
        #if(triggerPWM_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            triggerPWM_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            triggerPWM_STATUS_AUX_CTRL |= triggerPWM_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(triggerPWM_interruptState);

            /* Clear the FIFO to enable the triggerPWM_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            triggerPWM_ClearFIFO();
        #endif /* (triggerPWM_UseStatus) */
    #endif /* (triggerPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: triggerPWM_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void triggerPWM_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (triggerPWM_UsingFixedFunction)
        triggerPWM_GLOBAL_ENABLE |= triggerPWM_BLOCK_EN_MASK;
        triggerPWM_GLOBAL_STBY_ENABLE |= triggerPWM_BLOCK_STBY_EN_MASK;
    #endif /* (triggerPWM_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (triggerPWM_UseControl || triggerPWM_UsingFixedFunction)
        triggerPWM_CONTROL |= triggerPWM_CTRL_ENABLE;
    #endif /* (triggerPWM_UseControl || triggerPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: triggerPWM_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void triggerPWM_Stop(void) 
{
    #if (triggerPWM_UseControl || triggerPWM_UsingFixedFunction)
        triggerPWM_CONTROL &= ((uint8)(~triggerPWM_CTRL_ENABLE));
    #endif /* (triggerPWM_UseControl || triggerPWM_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (triggerPWM_UsingFixedFunction)
        triggerPWM_GLOBAL_ENABLE &= ((uint8)(~triggerPWM_BLOCK_EN_MASK));
        triggerPWM_GLOBAL_STBY_ENABLE &= ((uint8)(~triggerPWM_BLOCK_STBY_EN_MASK));
    #endif /* (triggerPWM_UsingFixedFunction) */
}

#if (triggerPWM_UseOneCompareMode)
    #if (triggerPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: triggerPWM_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void triggerPWM_SetCompareMode(uint8 comparemode) 
        {
            #if(triggerPWM_UsingFixedFunction)

                #if(0 != triggerPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << triggerPWM_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != triggerPWM_CTRL_CMPMODE1_SHIFT) */

                triggerPWM_CONTROL3 &= ((uint8)(~triggerPWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                triggerPWM_CONTROL3 |= comparemodemasked;

            #elif (triggerPWM_UseControl)

                #if(0 != triggerPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << triggerPWM_CTRL_CMPMODE1_SHIFT)) &
                                                triggerPWM_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & triggerPWM_CTRL_CMPMODE1_MASK;
                #endif /* (0 != triggerPWM_CTRL_CMPMODE1_SHIFT) */

                #if(0 != triggerPWM_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << triggerPWM_CTRL_CMPMODE2_SHIFT)) &
                                               triggerPWM_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & triggerPWM_CTRL_CMPMODE2_MASK;
                #endif /* (0 != triggerPWM_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                triggerPWM_CONTROL &= ((uint8)(~(triggerPWM_CTRL_CMPMODE1_MASK |
                                            triggerPWM_CTRL_CMPMODE2_MASK)));
                triggerPWM_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (triggerPWM_UsingFixedFunction) */
        }
    #endif /* triggerPWM_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (triggerPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: triggerPWM_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void triggerPWM_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != triggerPWM_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << triggerPWM_CTRL_CMPMODE1_SHIFT)) &
                                           triggerPWM_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & triggerPWM_CTRL_CMPMODE1_MASK;
            #endif /* (0 != triggerPWM_CTRL_CMPMODE1_SHIFT) */

            #if (triggerPWM_UseControl)
                triggerPWM_CONTROL &= ((uint8)(~triggerPWM_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                triggerPWM_CONTROL |= comparemodemasked;
            #endif /* (triggerPWM_UseControl) */
        }
    #endif /* triggerPWM_CompareMode1SW */

#if (triggerPWM_CompareMode2SW)


    /*******************************************************************************
    * Function Name: triggerPWM_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != triggerPWM_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << triggerPWM_CTRL_CMPMODE2_SHIFT)) &
                                                 triggerPWM_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & triggerPWM_CTRL_CMPMODE2_MASK;
        #endif /* (0 != triggerPWM_CTRL_CMPMODE2_SHIFT) */

        #if (triggerPWM_UseControl)
            triggerPWM_CONTROL &= ((uint8)(~triggerPWM_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            triggerPWM_CONTROL |= comparemodemasked;
        #endif /* (triggerPWM_UseControl) */
    }
    #endif /*triggerPWM_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!triggerPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: triggerPWM_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void triggerPWM_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(triggerPWM_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: triggerPWM_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 triggerPWM_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(triggerPWM_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(triggerPWM_CAPTURE_LSB_PTR));
    }

    #if (triggerPWM_UseStatus)


        /*******************************************************************************
        * Function Name: triggerPWM_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void triggerPWM_ClearFIFO(void) 
        {
            while(0u != (triggerPWM_ReadStatusRegister() & triggerPWM_STATUS_FIFONEMPTY))
            {
                (void)triggerPWM_ReadCapture();
            }
        }

    #endif /* triggerPWM_UseStatus */

#endif /* !triggerPWM_UsingFixedFunction */


/*******************************************************************************
* Function Name: triggerPWM_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void triggerPWM_WritePeriod(uint16 period) 
{
    #if(triggerPWM_UsingFixedFunction)
        CY_SET_REG16(triggerPWM_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(triggerPWM_PERIOD_LSB_PTR, period);
    #endif /* (triggerPWM_UsingFixedFunction) */
}

#if (triggerPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: triggerPWM_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void triggerPWM_WriteCompare(uint16 compare) \
                                       
    {
        #if(triggerPWM_UsingFixedFunction)
            CY_SET_REG16(triggerPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(triggerPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (triggerPWM_UsingFixedFunction) */

        #if (triggerPWM_PWMMode == triggerPWM__B_PWM__DITHER)
            #if(triggerPWM_UsingFixedFunction)
                CY_SET_REG16(triggerPWM_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(triggerPWM_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (triggerPWM_UsingFixedFunction) */
        #endif /* (triggerPWM_PWMMode == triggerPWM__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: triggerPWM_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_WriteCompare1(uint16 compare) \
                                        
    {
        #if(triggerPWM_UsingFixedFunction)
            CY_SET_REG16(triggerPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(triggerPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (triggerPWM_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: triggerPWM_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_WriteCompare2(uint16 compare) \
                                        
    {
        #if(triggerPWM_UsingFixedFunction)
            CY_SET_REG16(triggerPWM_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(triggerPWM_COMPARE2_LSB_PTR, compare);
        #endif /* (triggerPWM_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (triggerPWM_DeadBandUsed)


    /*******************************************************************************
    * Function Name: triggerPWM_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!triggerPWM_DeadBand2_4)
            CY_SET_REG8(triggerPWM_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            triggerPWM_DEADBAND_COUNT &= ((uint8)(~triggerPWM_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(triggerPWM_DEADBAND_COUNT_SHIFT)
                triggerPWM_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << triggerPWM_DEADBAND_COUNT_SHIFT)) &
                                                    triggerPWM_DEADBAND_COUNT_MASK;
            #else
                triggerPWM_DEADBAND_COUNT |= deadtime & triggerPWM_DEADBAND_COUNT_MASK;
            #endif /* (triggerPWM_DEADBAND_COUNT_SHIFT) */

        #endif /* (!triggerPWM_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: triggerPWM_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 triggerPWM_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!triggerPWM_DeadBand2_4)
            return (CY_GET_REG8(triggerPWM_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(triggerPWM_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(triggerPWM_DEADBAND_COUNT & triggerPWM_DEADBAND_COUNT_MASK)) >>
                                                                           triggerPWM_DEADBAND_COUNT_SHIFT));
            #else
                return (triggerPWM_DEADBAND_COUNT & triggerPWM_DEADBAND_COUNT_MASK);
            #endif /* (triggerPWM_DEADBAND_COUNT_SHIFT) */
        #endif /* (!triggerPWM_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (triggerPWM_UseStatus || triggerPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: triggerPWM_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(triggerPWM_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: triggerPWM_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 triggerPWM_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(triggerPWM_STATUS_PTR));
    }

#endif /* (triggerPWM_UseStatus || triggerPWM_UsingFixedFunction) */


#if (triggerPWM_UseControl)


    /*******************************************************************************
    * Function Name: triggerPWM_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 triggerPWM_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(triggerPWM_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: triggerPWM_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(triggerPWM_CONTROL_PTR, control);
    }

#endif /* (triggerPWM_UseControl) */


#if (!triggerPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: triggerPWM_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 triggerPWM_ReadCapture(void) 
    {
        return (CY_GET_REG16(triggerPWM_CAPTURE_LSB_PTR));
    }

#endif /* (!triggerPWM_UsingFixedFunction) */


#if (triggerPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: triggerPWM_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 triggerPWM_ReadCompare(void) 
    {
        #if(triggerPWM_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(triggerPWM_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(triggerPWM_COMPARE1_LSB_PTR));
        #endif /* (triggerPWM_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: triggerPWM_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 triggerPWM_ReadCompare1(void) 
    {
        return (CY_GET_REG16(triggerPWM_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: triggerPWM_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 triggerPWM_ReadCompare2(void) 
    {
        return (CY_GET_REG16(triggerPWM_COMPARE2_LSB_PTR));
    }

#endif /* (triggerPWM_UseOneCompareMode) */


/*******************************************************************************
* Function Name: triggerPWM_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 triggerPWM_ReadPeriod(void) 
{
    #if(triggerPWM_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(triggerPWM_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(triggerPWM_PERIOD_LSB_PTR));
    #endif /* (triggerPWM_UsingFixedFunction) */
}

#if ( triggerPWM_KillModeMinTime)


    /*******************************************************************************
    * Function Name: triggerPWM_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void triggerPWM_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(triggerPWM_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: triggerPWM_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 triggerPWM_ReadKillTime(void) 
    {
        return (CY_GET_REG8(triggerPWM_KILLMODEMINTIME_PTR));
    }

#endif /* ( triggerPWM_KillModeMinTime) */

/* [] END OF FILE */
