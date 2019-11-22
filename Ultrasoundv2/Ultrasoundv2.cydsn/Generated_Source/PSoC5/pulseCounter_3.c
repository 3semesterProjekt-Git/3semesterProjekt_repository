/*******************************************************************************
* File Name: pulseCounter_3.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 pulseCounter_3_initVar = 0u;


/*******************************************************************************
* Function Name: pulseCounter_3_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void pulseCounter_3_Init(void) 
{
        #if (!pulseCounter_3_UsingFixedFunction && !pulseCounter_3_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!pulseCounter_3_UsingFixedFunction && !pulseCounter_3_ControlRegRemoved) */
        
        #if(!pulseCounter_3_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 pulseCounter_3_interruptState;
        #endif /* (!pulseCounter_3_UsingFixedFunction) */
        
        #if (pulseCounter_3_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            pulseCounter_3_CONTROL &= pulseCounter_3_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                pulseCounter_3_CONTROL2 &= ((uint8)(~pulseCounter_3_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                pulseCounter_3_CONTROL3 &= ((uint8)(~pulseCounter_3_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (pulseCounter_3_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                pulseCounter_3_CONTROL |= pulseCounter_3_ONESHOT;
            #endif /* (pulseCounter_3_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            pulseCounter_3_CONTROL2 |= pulseCounter_3_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            pulseCounter_3_RT1 &= ((uint8)(~pulseCounter_3_RT1_MASK));
            pulseCounter_3_RT1 |= pulseCounter_3_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            pulseCounter_3_RT1 &= ((uint8)(~pulseCounter_3_SYNCDSI_MASK));
            pulseCounter_3_RT1 |= pulseCounter_3_SYNCDSI_EN;

        #else
            #if(!pulseCounter_3_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = pulseCounter_3_CONTROL & ((uint8)(~pulseCounter_3_CTRL_CMPMODE_MASK));
            pulseCounter_3_CONTROL = ctrl | pulseCounter_3_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = pulseCounter_3_CONTROL & ((uint8)(~pulseCounter_3_CTRL_CAPMODE_MASK));
            
            #if( 0 != pulseCounter_3_CAPTURE_MODE_CONF)
                pulseCounter_3_CONTROL = ctrl | pulseCounter_3_DEFAULT_CAPTURE_MODE;
            #else
                pulseCounter_3_CONTROL = ctrl;
            #endif /* 0 != pulseCounter_3_CAPTURE_MODE */ 
            
            #endif /* (!pulseCounter_3_ControlRegRemoved) */
        #endif /* (pulseCounter_3_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!pulseCounter_3_UsingFixedFunction)
            pulseCounter_3_ClearFIFO();
        #endif /* (!pulseCounter_3_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        pulseCounter_3_WritePeriod(pulseCounter_3_INIT_PERIOD_VALUE);
        #if (!(pulseCounter_3_UsingFixedFunction && (CY_PSOC5A)))
            pulseCounter_3_WriteCounter(pulseCounter_3_INIT_COUNTER_VALUE);
        #endif /* (!(pulseCounter_3_UsingFixedFunction && (CY_PSOC5A))) */
        pulseCounter_3_SetInterruptMode(pulseCounter_3_INIT_INTERRUPTS_MASK);
        
        #if (!pulseCounter_3_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)pulseCounter_3_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            pulseCounter_3_WriteCompare(pulseCounter_3_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            pulseCounter_3_interruptState = CyEnterCriticalSection();
            
            pulseCounter_3_STATUS_AUX_CTRL |= pulseCounter_3_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(pulseCounter_3_interruptState);
            
        #endif /* (!pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void pulseCounter_3_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (pulseCounter_3_UsingFixedFunction)
        pulseCounter_3_GLOBAL_ENABLE |= pulseCounter_3_BLOCK_EN_MASK;
        pulseCounter_3_GLOBAL_STBY_ENABLE |= pulseCounter_3_BLOCK_STBY_EN_MASK;
    #endif /* (pulseCounter_3_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!pulseCounter_3_ControlRegRemoved || pulseCounter_3_UsingFixedFunction)
        pulseCounter_3_CONTROL |= pulseCounter_3_CTRL_ENABLE;                
    #endif /* (!pulseCounter_3_ControlRegRemoved || pulseCounter_3_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: pulseCounter_3_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  pulseCounter_3_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void pulseCounter_3_Start(void) 
{
    if(pulseCounter_3_initVar == 0u)
    {
        pulseCounter_3_Init();
        
        pulseCounter_3_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    pulseCounter_3_Enable();        
}


/*******************************************************************************
* Function Name: pulseCounter_3_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void pulseCounter_3_Stop(void) 
{
    /* Disable Counter */
    #if(!pulseCounter_3_ControlRegRemoved || pulseCounter_3_UsingFixedFunction)
        pulseCounter_3_CONTROL &= ((uint8)(~pulseCounter_3_CTRL_ENABLE));        
    #endif /* (!pulseCounter_3_ControlRegRemoved || pulseCounter_3_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (pulseCounter_3_UsingFixedFunction)
        pulseCounter_3_GLOBAL_ENABLE &= ((uint8)(~pulseCounter_3_BLOCK_EN_MASK));
        pulseCounter_3_GLOBAL_STBY_ENABLE &= ((uint8)(~pulseCounter_3_BLOCK_STBY_EN_MASK));
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void pulseCounter_3_SetInterruptMode(uint8 interruptsMask) 
{
    pulseCounter_3_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: pulseCounter_3_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   pulseCounter_3_ReadStatusRegister(void) 
{
    return pulseCounter_3_STATUS;
}


#if(!pulseCounter_3_ControlRegRemoved)
/*******************************************************************************
* Function Name: pulseCounter_3_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   pulseCounter_3_ReadControlRegister(void) 
{
    return pulseCounter_3_CONTROL;
}


/*******************************************************************************
* Function Name: pulseCounter_3_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    pulseCounter_3_WriteControlRegister(uint8 control) 
{
    pulseCounter_3_CONTROL = control;
}

#endif  /* (!pulseCounter_3_ControlRegRemoved) */


#if (!(pulseCounter_3_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: pulseCounter_3_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void pulseCounter_3_WriteCounter(uint32 counter) \
                                   
{
    #if(pulseCounter_3_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (pulseCounter_3_GLOBAL_ENABLE & pulseCounter_3_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        pulseCounter_3_GLOBAL_ENABLE |= pulseCounter_3_BLOCK_EN_MASK;
        CY_SET_REG16(pulseCounter_3_COUNTER_LSB_PTR, (uint16)counter);
        pulseCounter_3_GLOBAL_ENABLE &= ((uint8)(~pulseCounter_3_BLOCK_EN_MASK));
    #else
        CY_SET_REG32(pulseCounter_3_COUNTER_LSB_PTR, counter);
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}
#endif /* (!(pulseCounter_3_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: pulseCounter_3_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) The present value of the counter.
*
*******************************************************************************/
uint32 pulseCounter_3_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(pulseCounter_3_UsingFixedFunction)
		(void)CY_GET_REG16(pulseCounter_3_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(pulseCounter_3_COUNTER_LSB_PTR_8BIT);
	#endif/* (pulseCounter_3_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(pulseCounter_3_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(pulseCounter_3_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG32(pulseCounter_3_STATICCOUNT_LSB_PTR));
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint32) Present Capture value.
*
*******************************************************************************/
uint32 pulseCounter_3_ReadCapture(void) 
{
    #if(pulseCounter_3_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(pulseCounter_3_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG32(pulseCounter_3_STATICCOUNT_LSB_PTR));
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint32) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void pulseCounter_3_WritePeriod(uint32 period) 
{
    #if(pulseCounter_3_UsingFixedFunction)
        CY_SET_REG16(pulseCounter_3_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG32(pulseCounter_3_PERIOD_LSB_PTR, period);
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) Present period value.
*
*******************************************************************************/
uint32 pulseCounter_3_ReadPeriod(void) 
{
    #if(pulseCounter_3_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(pulseCounter_3_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG32(pulseCounter_3_PERIOD_LSB_PTR));
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}


#if (!pulseCounter_3_UsingFixedFunction)
/*******************************************************************************
* Function Name: pulseCounter_3_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void pulseCounter_3_WriteCompare(uint32 compare) \
                                   
{
    #if(pulseCounter_3_UsingFixedFunction)
        CY_SET_REG16(pulseCounter_3_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG32(pulseCounter_3_COMPARE_LSB_PTR, compare);
    #endif /* (pulseCounter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_3_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint32) Present compare value.
*
*******************************************************************************/
uint32 pulseCounter_3_ReadCompare(void) 
{
    return (CY_GET_REG32(pulseCounter_3_COMPARE_LSB_PTR));
}


#if (pulseCounter_3_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: pulseCounter_3_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void pulseCounter_3_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    pulseCounter_3_CONTROL &= ((uint8)(~pulseCounter_3_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    pulseCounter_3_CONTROL |= compareMode;
}
#endif  /* (pulseCounter_3_COMPARE_MODE_SOFTWARE) */


#if (pulseCounter_3_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: pulseCounter_3_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void pulseCounter_3_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    pulseCounter_3_CONTROL &= ((uint8)(~pulseCounter_3_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    pulseCounter_3_CONTROL |= ((uint8)((uint8)captureMode << pulseCounter_3_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (pulseCounter_3_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: pulseCounter_3_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void pulseCounter_3_ClearFIFO(void) 
{

    while(0u != (pulseCounter_3_ReadStatusRegister() & pulseCounter_3_STATUS_FIFONEMP))
    {
        (void)pulseCounter_3_ReadCapture();
    }

}
#endif  /* (!pulseCounter_3_UsingFixedFunction) */


/* [] END OF FILE */

