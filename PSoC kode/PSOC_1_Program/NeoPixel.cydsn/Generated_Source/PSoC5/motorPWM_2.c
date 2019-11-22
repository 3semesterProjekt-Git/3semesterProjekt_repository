/*******************************************************************************
* File Name: motorPWM_2.c
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

#include "motorPWM_2.h"

/* Error message for removed <resource> through optimization */
#ifdef motorPWM_2_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* motorPWM_2_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 motorPWM_2_initVar = 0u;


/*******************************************************************************
* Function Name: motorPWM_2_Start
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
*  motorPWM_2_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void motorPWM_2_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(motorPWM_2_initVar == 0u)
    {
        motorPWM_2_Init();
        motorPWM_2_initVar = 1u;
    }
    motorPWM_2_Enable();

}


/*******************************************************************************
* Function Name: motorPWM_2_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  motorPWM_2_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void motorPWM_2_Init(void) 
{
    #if (motorPWM_2_UsingFixedFunction || motorPWM_2_UseControl)
        uint8 ctrl;
    #endif /* (motorPWM_2_UsingFixedFunction || motorPWM_2_UseControl) */

    #if(!motorPWM_2_UsingFixedFunction)
        #if(motorPWM_2_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 motorPWM_2_interruptState;
        #endif /* (motorPWM_2_UseStatus) */
    #endif /* (!motorPWM_2_UsingFixedFunction) */

    #if (motorPWM_2_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        motorPWM_2_CONTROL |= motorPWM_2_CFG0_MODE;
        #if (motorPWM_2_DeadBand2_4)
            motorPWM_2_CONTROL |= motorPWM_2_CFG0_DB;
        #endif /* (motorPWM_2_DeadBand2_4) */

        ctrl = motorPWM_2_CONTROL3 & ((uint8 )(~motorPWM_2_CTRL_CMPMODE1_MASK));
        motorPWM_2_CONTROL3 = ctrl | motorPWM_2_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        motorPWM_2_RT1 &= ((uint8)(~motorPWM_2_RT1_MASK));
        motorPWM_2_RT1 |= motorPWM_2_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        motorPWM_2_RT1 &= ((uint8)(~motorPWM_2_SYNCDSI_MASK));
        motorPWM_2_RT1 |= motorPWM_2_SYNCDSI_EN;

    #elif (motorPWM_2_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = motorPWM_2_CONTROL & ((uint8)(~motorPWM_2_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~motorPWM_2_CTRL_CMPMODE1_MASK));
        motorPWM_2_CONTROL = ctrl | motorPWM_2_DEFAULT_COMPARE2_MODE |
                                   motorPWM_2_DEFAULT_COMPARE1_MODE;
    #endif /* (motorPWM_2_UsingFixedFunction) */

    #if (!motorPWM_2_UsingFixedFunction)
        #if (motorPWM_2_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            motorPWM_2_AUX_CONTROLDP0 |= (motorPWM_2_AUX_CTRL_FIFO0_CLR);
        #else /* (motorPWM_2_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            motorPWM_2_AUX_CONTROLDP0 |= (motorPWM_2_AUX_CTRL_FIFO0_CLR);
            motorPWM_2_AUX_CONTROLDP1 |= (motorPWM_2_AUX_CTRL_FIFO0_CLR);
        #endif /* (motorPWM_2_Resolution == 8) */

        motorPWM_2_WriteCounter(motorPWM_2_INIT_PERIOD_VALUE);
    #endif /* (!motorPWM_2_UsingFixedFunction) */

    motorPWM_2_WritePeriod(motorPWM_2_INIT_PERIOD_VALUE);

        #if (motorPWM_2_UseOneCompareMode)
            motorPWM_2_WriteCompare(motorPWM_2_INIT_COMPARE_VALUE1);
        #else
            motorPWM_2_WriteCompare1(motorPWM_2_INIT_COMPARE_VALUE1);
            motorPWM_2_WriteCompare2(motorPWM_2_INIT_COMPARE_VALUE2);
        #endif /* (motorPWM_2_UseOneCompareMode) */

        #if (motorPWM_2_KillModeMinTime)
            motorPWM_2_WriteKillTime(motorPWM_2_MinimumKillTime);
        #endif /* (motorPWM_2_KillModeMinTime) */

        #if (motorPWM_2_DeadBandUsed)
            motorPWM_2_WriteDeadTime(motorPWM_2_INIT_DEAD_TIME);
        #endif /* (motorPWM_2_DeadBandUsed) */

    #if (motorPWM_2_UseStatus || motorPWM_2_UsingFixedFunction)
        motorPWM_2_SetInterruptMode(motorPWM_2_INIT_INTERRUPTS_MODE);
    #endif /* (motorPWM_2_UseStatus || motorPWM_2_UsingFixedFunction) */

    #if (motorPWM_2_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        motorPWM_2_GLOBAL_ENABLE |= motorPWM_2_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        motorPWM_2_CONTROL2 |= motorPWM_2_CTRL2_IRQ_SEL;
    #else
        #if(motorPWM_2_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            motorPWM_2_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            motorPWM_2_STATUS_AUX_CTRL |= motorPWM_2_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(motorPWM_2_interruptState);

            /* Clear the FIFO to enable the motorPWM_2_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            motorPWM_2_ClearFIFO();
        #endif /* (motorPWM_2_UseStatus) */
    #endif /* (motorPWM_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorPWM_2_Enable
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
void motorPWM_2_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (motorPWM_2_UsingFixedFunction)
        motorPWM_2_GLOBAL_ENABLE |= motorPWM_2_BLOCK_EN_MASK;
        motorPWM_2_GLOBAL_STBY_ENABLE |= motorPWM_2_BLOCK_STBY_EN_MASK;
    #endif /* (motorPWM_2_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (motorPWM_2_UseControl || motorPWM_2_UsingFixedFunction)
        motorPWM_2_CONTROL |= motorPWM_2_CTRL_ENABLE;
    #endif /* (motorPWM_2_UseControl || motorPWM_2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorPWM_2_Stop
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
void motorPWM_2_Stop(void) 
{
    #if (motorPWM_2_UseControl || motorPWM_2_UsingFixedFunction)
        motorPWM_2_CONTROL &= ((uint8)(~motorPWM_2_CTRL_ENABLE));
    #endif /* (motorPWM_2_UseControl || motorPWM_2_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (motorPWM_2_UsingFixedFunction)
        motorPWM_2_GLOBAL_ENABLE &= ((uint8)(~motorPWM_2_BLOCK_EN_MASK));
        motorPWM_2_GLOBAL_STBY_ENABLE &= ((uint8)(~motorPWM_2_BLOCK_STBY_EN_MASK));
    #endif /* (motorPWM_2_UsingFixedFunction) */
}

#if (motorPWM_2_UseOneCompareMode)
    #if (motorPWM_2_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motorPWM_2_SetCompareMode
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
        void motorPWM_2_SetCompareMode(uint8 comparemode) 
        {
            #if(motorPWM_2_UsingFixedFunction)

                #if(0 != motorPWM_2_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << motorPWM_2_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != motorPWM_2_CTRL_CMPMODE1_SHIFT) */

                motorPWM_2_CONTROL3 &= ((uint8)(~motorPWM_2_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                motorPWM_2_CONTROL3 |= comparemodemasked;

            #elif (motorPWM_2_UseControl)

                #if(0 != motorPWM_2_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << motorPWM_2_CTRL_CMPMODE1_SHIFT)) &
                                                motorPWM_2_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & motorPWM_2_CTRL_CMPMODE1_MASK;
                #endif /* (0 != motorPWM_2_CTRL_CMPMODE1_SHIFT) */

                #if(0 != motorPWM_2_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << motorPWM_2_CTRL_CMPMODE2_SHIFT)) &
                                               motorPWM_2_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & motorPWM_2_CTRL_CMPMODE2_MASK;
                #endif /* (0 != motorPWM_2_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                motorPWM_2_CONTROL &= ((uint8)(~(motorPWM_2_CTRL_CMPMODE1_MASK |
                                            motorPWM_2_CTRL_CMPMODE2_MASK)));
                motorPWM_2_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (motorPWM_2_UsingFixedFunction) */
        }
    #endif /* motorPWM_2_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (motorPWM_2_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motorPWM_2_SetCompareMode1
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
        void motorPWM_2_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != motorPWM_2_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << motorPWM_2_CTRL_CMPMODE1_SHIFT)) &
                                           motorPWM_2_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & motorPWM_2_CTRL_CMPMODE1_MASK;
            #endif /* (0 != motorPWM_2_CTRL_CMPMODE1_SHIFT) */

            #if (motorPWM_2_UseControl)
                motorPWM_2_CONTROL &= ((uint8)(~motorPWM_2_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                motorPWM_2_CONTROL |= comparemodemasked;
            #endif /* (motorPWM_2_UseControl) */
        }
    #endif /* motorPWM_2_CompareMode1SW */

#if (motorPWM_2_CompareMode2SW)


    /*******************************************************************************
    * Function Name: motorPWM_2_SetCompareMode2
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
    void motorPWM_2_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != motorPWM_2_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << motorPWM_2_CTRL_CMPMODE2_SHIFT)) &
                                                 motorPWM_2_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & motorPWM_2_CTRL_CMPMODE2_MASK;
        #endif /* (0 != motorPWM_2_CTRL_CMPMODE2_SHIFT) */

        #if (motorPWM_2_UseControl)
            motorPWM_2_CONTROL &= ((uint8)(~motorPWM_2_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            motorPWM_2_CONTROL |= comparemodemasked;
        #endif /* (motorPWM_2_UseControl) */
    }
    #endif /*motorPWM_2_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!motorPWM_2_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteCounter
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
    void motorPWM_2_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(motorPWM_2_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadCounter
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
    uint8 motorPWM_2_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(motorPWM_2_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(motorPWM_2_CAPTURE_LSB_PTR));
    }

    #if (motorPWM_2_UseStatus)


        /*******************************************************************************
        * Function Name: motorPWM_2_ClearFIFO
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
        void motorPWM_2_ClearFIFO(void) 
        {
            while(0u != (motorPWM_2_ReadStatusRegister() & motorPWM_2_STATUS_FIFONEMPTY))
            {
                (void)motorPWM_2_ReadCapture();
            }
        }

    #endif /* motorPWM_2_UseStatus */

#endif /* !motorPWM_2_UsingFixedFunction */


/*******************************************************************************
* Function Name: motorPWM_2_WritePeriod
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
void motorPWM_2_WritePeriod(uint8 period) 
{
    #if(motorPWM_2_UsingFixedFunction)
        CY_SET_REG16(motorPWM_2_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(motorPWM_2_PERIOD_LSB_PTR, period);
    #endif /* (motorPWM_2_UsingFixedFunction) */
}

#if (motorPWM_2_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteCompare
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
    void motorPWM_2_WriteCompare(uint8 compare) \
                                       
    {
        #if(motorPWM_2_UsingFixedFunction)
            CY_SET_REG16(motorPWM_2_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(motorPWM_2_COMPARE1_LSB_PTR, compare);
        #endif /* (motorPWM_2_UsingFixedFunction) */

        #if (motorPWM_2_PWMMode == motorPWM_2__B_PWM__DITHER)
            #if(motorPWM_2_UsingFixedFunction)
                CY_SET_REG16(motorPWM_2_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(motorPWM_2_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (motorPWM_2_UsingFixedFunction) */
        #endif /* (motorPWM_2_PWMMode == motorPWM_2__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteCompare1
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
    void motorPWM_2_WriteCompare1(uint8 compare) \
                                        
    {
        #if(motorPWM_2_UsingFixedFunction)
            CY_SET_REG16(motorPWM_2_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(motorPWM_2_COMPARE1_LSB_PTR, compare);
        #endif /* (motorPWM_2_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteCompare2
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
    void motorPWM_2_WriteCompare2(uint8 compare) \
                                        
    {
        #if(motorPWM_2_UsingFixedFunction)
            CY_SET_REG16(motorPWM_2_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(motorPWM_2_COMPARE2_LSB_PTR, compare);
        #endif /* (motorPWM_2_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (motorPWM_2_DeadBandUsed)


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteDeadTime
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
    void motorPWM_2_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!motorPWM_2_DeadBand2_4)
            CY_SET_REG8(motorPWM_2_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            motorPWM_2_DEADBAND_COUNT &= ((uint8)(~motorPWM_2_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(motorPWM_2_DEADBAND_COUNT_SHIFT)
                motorPWM_2_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << motorPWM_2_DEADBAND_COUNT_SHIFT)) &
                                                    motorPWM_2_DEADBAND_COUNT_MASK;
            #else
                motorPWM_2_DEADBAND_COUNT |= deadtime & motorPWM_2_DEADBAND_COUNT_MASK;
            #endif /* (motorPWM_2_DEADBAND_COUNT_SHIFT) */

        #endif /* (!motorPWM_2_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadDeadTime
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
    uint8 motorPWM_2_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!motorPWM_2_DeadBand2_4)
            return (CY_GET_REG8(motorPWM_2_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(motorPWM_2_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(motorPWM_2_DEADBAND_COUNT & motorPWM_2_DEADBAND_COUNT_MASK)) >>
                                                                           motorPWM_2_DEADBAND_COUNT_SHIFT));
            #else
                return (motorPWM_2_DEADBAND_COUNT & motorPWM_2_DEADBAND_COUNT_MASK);
            #endif /* (motorPWM_2_DEADBAND_COUNT_SHIFT) */
        #endif /* (!motorPWM_2_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (motorPWM_2_UseStatus || motorPWM_2_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motorPWM_2_SetInterruptMode
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
    void motorPWM_2_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(motorPWM_2_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadStatusRegister
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
    uint8 motorPWM_2_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(motorPWM_2_STATUS_PTR));
    }

#endif /* (motorPWM_2_UseStatus || motorPWM_2_UsingFixedFunction) */


#if (motorPWM_2_UseControl)


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadControlRegister
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
    uint8 motorPWM_2_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(motorPWM_2_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteControlRegister
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
    void motorPWM_2_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(motorPWM_2_CONTROL_PTR, control);
    }

#endif /* (motorPWM_2_UseControl) */


#if (!motorPWM_2_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadCapture
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
    uint8 motorPWM_2_ReadCapture(void) 
    {
        return (CY_GET_REG8(motorPWM_2_CAPTURE_LSB_PTR));
    }

#endif /* (!motorPWM_2_UsingFixedFunction) */


#if (motorPWM_2_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadCompare
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
    uint8 motorPWM_2_ReadCompare(void) 
    {
        #if(motorPWM_2_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(motorPWM_2_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(motorPWM_2_COMPARE1_LSB_PTR));
        #endif /* (motorPWM_2_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadCompare1
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
    uint8 motorPWM_2_ReadCompare1(void) 
    {
        return (CY_GET_REG8(motorPWM_2_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadCompare2
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
    uint8 motorPWM_2_ReadCompare2(void) 
    {
        return (CY_GET_REG8(motorPWM_2_COMPARE2_LSB_PTR));
    }

#endif /* (motorPWM_2_UseOneCompareMode) */


/*******************************************************************************
* Function Name: motorPWM_2_ReadPeriod
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
uint8 motorPWM_2_ReadPeriod(void) 
{
    #if(motorPWM_2_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(motorPWM_2_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(motorPWM_2_PERIOD_LSB_PTR));
    #endif /* (motorPWM_2_UsingFixedFunction) */
}

#if ( motorPWM_2_KillModeMinTime)


    /*******************************************************************************
    * Function Name: motorPWM_2_WriteKillTime
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
    void motorPWM_2_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(motorPWM_2_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: motorPWM_2_ReadKillTime
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
    uint8 motorPWM_2_ReadKillTime(void) 
    {
        return (CY_GET_REG8(motorPWM_2_KILLMODEMINTIME_PTR));
    }

#endif /* ( motorPWM_2_KillModeMinTime) */

/* [] END OF FILE */
