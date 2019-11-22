/*******************************************************************************
* File Name: pulseCounter_1.c  
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

#include "pulseCounter_1.h"

uint8 pulseCounter_1_initVar = 0u;


/*******************************************************************************
* Function Name: pulseCounter_1_Init
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
void pulseCounter_1_Init(void) 
{
        #if (!pulseCounter_1_UsingFixedFunction && !pulseCounter_1_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!pulseCounter_1_UsingFixedFunction && !pulseCounter_1_ControlRegRemoved) */
        
        #if(!pulseCounter_1_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 pulseCounter_1_interruptState;
        #endif /* (!pulseCounter_1_UsingFixedFunction) */
        
        #if (pulseCounter_1_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            pulseCounter_1_CONTROL &= pulseCounter_1_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                pulseCounter_1_CONTROL2 &= ((uint8)(~pulseCounter_1_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                pulseCounter_1_CONTROL3 &= ((uint8)(~pulseCounter_1_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (pulseCounter_1_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                pulseCounter_1_CONTROL |= pulseCounter_1_ONESHOT;
            #endif /* (pulseCounter_1_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            pulseCounter_1_CONTROL2 |= pulseCounter_1_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            pulseCounter_1_RT1 &= ((uint8)(~pulseCounter_1_RT1_MASK));
            pulseCounter_1_RT1 |= pulseCounter_1_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            pulseCounter_1_RT1 &= ((uint8)(~pulseCounter_1_SYNCDSI_MASK));
            pulseCounter_1_RT1 |= pulseCounter_1_SYNCDSI_EN;

        #else
            #if(!pulseCounter_1_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = pulseCounter_1_CONTROL & ((uint8)(~pulseCounter_1_CTRL_CMPMODE_MASK));
            pulseCounter_1_CONTROL = ctrl | pulseCounter_1_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = pulseCounter_1_CONTROL & ((uint8)(~pulseCounter_1_CTRL_CAPMODE_MASK));
            
            #if( 0 != pulseCounter_1_CAPTURE_MODE_CONF)
                pulseCounter_1_CONTROL = ctrl | pulseCounter_1_DEFAULT_CAPTURE_MODE;
            #else
                pulseCounter_1_CONTROL = ctrl;
            #endif /* 0 != pulseCounter_1_CAPTURE_MODE */ 
            
            #endif /* (!pulseCounter_1_ControlRegRemoved) */
        #endif /* (pulseCounter_1_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!pulseCounter_1_UsingFixedFunction)
            pulseCounter_1_ClearFIFO();
        #endif /* (!pulseCounter_1_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        pulseCounter_1_WritePeriod(pulseCounter_1_INIT_PERIOD_VALUE);
        #if (!(pulseCounter_1_UsingFixedFunction && (CY_PSOC5A)))
            pulseCounter_1_WriteCounter(pulseCounter_1_INIT_COUNTER_VALUE);
        #endif /* (!(pulseCounter_1_UsingFixedFunction && (CY_PSOC5A))) */
        pulseCounter_1_SetInterruptMode(pulseCounter_1_INIT_INTERRUPTS_MASK);
        
        #if (!pulseCounter_1_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)pulseCounter_1_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            pulseCounter_1_WriteCompare(pulseCounter_1_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            pulseCounter_1_interruptState = CyEnterCriticalSection();
            
            pulseCounter_1_STATUS_AUX_CTRL |= pulseCounter_1_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(pulseCounter_1_interruptState);
            
        #endif /* (!pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_Enable
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
void pulseCounter_1_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (pulseCounter_1_UsingFixedFunction)
        pulseCounter_1_GLOBAL_ENABLE |= pulseCounter_1_BLOCK_EN_MASK;
        pulseCounter_1_GLOBAL_STBY_ENABLE |= pulseCounter_1_BLOCK_STBY_EN_MASK;
    #endif /* (pulseCounter_1_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!pulseCounter_1_ControlRegRemoved || pulseCounter_1_UsingFixedFunction)
        pulseCounter_1_CONTROL |= pulseCounter_1_CTRL_ENABLE;                
    #endif /* (!pulseCounter_1_ControlRegRemoved || pulseCounter_1_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: pulseCounter_1_Start
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
*  pulseCounter_1_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void pulseCounter_1_Start(void) 
{
    if(pulseCounter_1_initVar == 0u)
    {
        pulseCounter_1_Init();
        
        pulseCounter_1_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    pulseCounter_1_Enable();        
}


/*******************************************************************************
* Function Name: pulseCounter_1_Stop
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
void pulseCounter_1_Stop(void) 
{
    /* Disable Counter */
    #if(!pulseCounter_1_ControlRegRemoved || pulseCounter_1_UsingFixedFunction)
        pulseCounter_1_CONTROL &= ((uint8)(~pulseCounter_1_CTRL_ENABLE));        
    #endif /* (!pulseCounter_1_ControlRegRemoved || pulseCounter_1_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (pulseCounter_1_UsingFixedFunction)
        pulseCounter_1_GLOBAL_ENABLE &= ((uint8)(~pulseCounter_1_BLOCK_EN_MASK));
        pulseCounter_1_GLOBAL_STBY_ENABLE &= ((uint8)(~pulseCounter_1_BLOCK_STBY_EN_MASK));
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_SetInterruptMode
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
void pulseCounter_1_SetInterruptMode(uint8 interruptsMask) 
{
    pulseCounter_1_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: pulseCounter_1_ReadStatusRegister
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
uint8   pulseCounter_1_ReadStatusRegister(void) 
{
    return pulseCounter_1_STATUS;
}


#if(!pulseCounter_1_ControlRegRemoved)
/*******************************************************************************
* Function Name: pulseCounter_1_ReadControlRegister
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
uint8   pulseCounter_1_ReadControlRegister(void) 
{
    return pulseCounter_1_CONTROL;
}


/*******************************************************************************
* Function Name: pulseCounter_1_WriteControlRegister
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
void    pulseCounter_1_WriteControlRegister(uint8 control) 
{
    pulseCounter_1_CONTROL = control;
}

#endif  /* (!pulseCounter_1_ControlRegRemoved) */


#if (!(pulseCounter_1_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: pulseCounter_1_WriteCounter
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
void pulseCounter_1_WriteCounter(uint16 counter) \
                                   
{
    #if(pulseCounter_1_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (pulseCounter_1_GLOBAL_ENABLE & pulseCounter_1_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        pulseCounter_1_GLOBAL_ENABLE |= pulseCounter_1_BLOCK_EN_MASK;
        CY_SET_REG16(pulseCounter_1_COUNTER_LSB_PTR, (uint16)counter);
        pulseCounter_1_GLOBAL_ENABLE &= ((uint8)(~pulseCounter_1_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(pulseCounter_1_COUNTER_LSB_PTR, counter);
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}
#endif /* (!(pulseCounter_1_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: pulseCounter_1_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 pulseCounter_1_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(pulseCounter_1_UsingFixedFunction)
		(void)CY_GET_REG16(pulseCounter_1_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(pulseCounter_1_COUNTER_LSB_PTR_8BIT);
	#endif/* (pulseCounter_1_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(pulseCounter_1_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(pulseCounter_1_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(pulseCounter_1_STATICCOUNT_LSB_PTR));
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 pulseCounter_1_ReadCapture(void) 
{
    #if(pulseCounter_1_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(pulseCounter_1_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(pulseCounter_1_STATICCOUNT_LSB_PTR));
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void pulseCounter_1_WritePeriod(uint16 period) 
{
    #if(pulseCounter_1_UsingFixedFunction)
        CY_SET_REG16(pulseCounter_1_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(pulseCounter_1_PERIOD_LSB_PTR, period);
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 pulseCounter_1_ReadPeriod(void) 
{
    #if(pulseCounter_1_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(pulseCounter_1_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(pulseCounter_1_PERIOD_LSB_PTR));
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}


#if (!pulseCounter_1_UsingFixedFunction)
/*******************************************************************************
* Function Name: pulseCounter_1_WriteCompare
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
void pulseCounter_1_WriteCompare(uint16 compare) \
                                   
{
    #if(pulseCounter_1_UsingFixedFunction)
        CY_SET_REG16(pulseCounter_1_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(pulseCounter_1_COMPARE_LSB_PTR, compare);
    #endif /* (pulseCounter_1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: pulseCounter_1_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 pulseCounter_1_ReadCompare(void) 
{
    return (CY_GET_REG16(pulseCounter_1_COMPARE_LSB_PTR));
}


#if (pulseCounter_1_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: pulseCounter_1_SetCompareMode
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
void pulseCounter_1_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    pulseCounter_1_CONTROL &= ((uint8)(~pulseCounter_1_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    pulseCounter_1_CONTROL |= compareMode;
}
#endif  /* (pulseCounter_1_COMPARE_MODE_SOFTWARE) */


#if (pulseCounter_1_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: pulseCounter_1_SetCaptureMode
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
void pulseCounter_1_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    pulseCounter_1_CONTROL &= ((uint8)(~pulseCounter_1_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    pulseCounter_1_CONTROL |= ((uint8)((uint8)captureMode << pulseCounter_1_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (pulseCounter_1_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: pulseCounter_1_ClearFIFO
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
void pulseCounter_1_ClearFIFO(void) 
{

    while(0u != (pulseCounter_1_ReadStatusRegister() & pulseCounter_1_STATUS_FIFONEMP))
    {
        (void)pulseCounter_1_ReadCapture();
    }

}
#endif  /* (!pulseCounter_1_UsingFixedFunction) */


/* [] END OF FILE */
