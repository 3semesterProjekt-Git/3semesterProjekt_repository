/*******************************************************************************
* File Name: soundUART.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
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
#if (soundUART_INTERNAL_CLOCK_USED)
    #include "soundUART_IntClock.h"
#endif /* End soundUART_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 soundUART_initVar = 0u;

#if (soundUART_TX_INTERRUPT_ENABLED && soundUART_TX_ENABLED)
    volatile uint8 soundUART_txBuffer[soundUART_TX_BUFFER_SIZE];
    volatile uint8 soundUART_txBufferRead = 0u;
    uint8 soundUART_txBufferWrite = 0u;
#endif /* (soundUART_TX_INTERRUPT_ENABLED && soundUART_TX_ENABLED) */

#if (soundUART_RX_INTERRUPT_ENABLED && (soundUART_RX_ENABLED || soundUART_HD_ENABLED))
    uint8 soundUART_errorStatus = 0u;
    volatile uint8 soundUART_rxBuffer[soundUART_RX_BUFFER_SIZE];
    volatile uint8 soundUART_rxBufferRead  = 0u;
    volatile uint8 soundUART_rxBufferWrite = 0u;
    volatile uint8 soundUART_rxBufferLoopDetect = 0u;
    volatile uint8 soundUART_rxBufferOverflow   = 0u;
    #if (soundUART_RXHW_ADDRESS_ENABLED)
        volatile uint8 soundUART_rxAddressMode = soundUART_RX_ADDRESS_MODE;
        volatile uint8 soundUART_rxAddressDetected = 0u;
    #endif /* (soundUART_RXHW_ADDRESS_ENABLED) */
#endif /* (soundUART_RX_INTERRUPT_ENABLED && (soundUART_RX_ENABLED || soundUART_HD_ENABLED)) */


/*******************************************************************************
* Function Name: soundUART_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  soundUART_Start() sets the initVar variable, calls the
*  soundUART_Init() function, and then calls the
*  soundUART_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The soundUART_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time soundUART_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the soundUART_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void soundUART_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(soundUART_initVar == 0u)
    {
        soundUART_Init();
        soundUART_initVar = 1u;
    }

    soundUART_Enable();
}


/*******************************************************************************
* Function Name: soundUART_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call soundUART_Init() because
*  the soundUART_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void soundUART_Init(void) 
{
    #if(soundUART_RX_ENABLED || soundUART_HD_ENABLED)

        #if (soundUART_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(soundUART_RX_VECT_NUM, &soundUART_RXISR);
            CyIntSetPriority(soundUART_RX_VECT_NUM, soundUART_RX_PRIOR_NUM);
            soundUART_errorStatus = 0u;
        #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

        #if (soundUART_RXHW_ADDRESS_ENABLED)
            soundUART_SetRxAddressMode(soundUART_RX_ADDRESS_MODE);
            soundUART_SetRxAddress1(soundUART_RX_HW_ADDRESS1);
            soundUART_SetRxAddress2(soundUART_RX_HW_ADDRESS2);
        #endif /* End soundUART_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        soundUART_RXBITCTR_PERIOD_REG = soundUART_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        soundUART_RXSTATUS_MASK_REG  = soundUART_INIT_RX_INTERRUPTS_MASK;
    #endif /* End soundUART_RX_ENABLED || soundUART_HD_ENABLED*/

    #if(soundUART_TX_ENABLED)
        #if (soundUART_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(soundUART_TX_VECT_NUM, &soundUART_TXISR);
            CyIntSetPriority(soundUART_TX_VECT_NUM, soundUART_TX_PRIOR_NUM);
        #endif /* (soundUART_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (soundUART_TXCLKGEN_DP)
            soundUART_TXBITCLKGEN_CTR_REG = soundUART_BIT_CENTER;
            soundUART_TXBITCLKTX_COMPLETE_REG = ((soundUART_NUMBER_OF_DATA_BITS +
                        soundUART_NUMBER_OF_START_BIT) * soundUART_OVER_SAMPLE_COUNT) - 1u;
        #else
            soundUART_TXBITCTR_PERIOD_REG = ((soundUART_NUMBER_OF_DATA_BITS +
                        soundUART_NUMBER_OF_START_BIT) * soundUART_OVER_SAMPLE_8) - 1u;
        #endif /* End soundUART_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (soundUART_TX_INTERRUPT_ENABLED)
            soundUART_TXSTATUS_MASK_REG = soundUART_TX_STS_FIFO_EMPTY;
        #else
            soundUART_TXSTATUS_MASK_REG = soundUART_INIT_TX_INTERRUPTS_MASK;
        #endif /*End soundUART_TX_INTERRUPT_ENABLED*/

    #endif /* End soundUART_TX_ENABLED */

    #if(soundUART_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        soundUART_WriteControlRegister( \
            (soundUART_ReadControlRegister() & (uint8)~soundUART_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(soundUART_PARITY_TYPE << soundUART_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End soundUART_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: soundUART_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call soundUART_Enable() because the soundUART_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  soundUART_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void soundUART_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (soundUART_RX_ENABLED || soundUART_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        soundUART_RXBITCTR_CONTROL_REG |= soundUART_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        soundUART_RXSTATUS_ACTL_REG  |= soundUART_INT_ENABLE;

        #if (soundUART_RX_INTERRUPT_ENABLED)
            soundUART_EnableRxInt();

            #if (soundUART_RXHW_ADDRESS_ENABLED)
                soundUART_rxAddressDetected = 0u;
            #endif /* (soundUART_RXHW_ADDRESS_ENABLED) */
        #endif /* (soundUART_RX_INTERRUPT_ENABLED) */
    #endif /* (soundUART_RX_ENABLED || soundUART_HD_ENABLED) */

    #if(soundUART_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!soundUART_TXCLKGEN_DP)
            soundUART_TXBITCTR_CONTROL_REG |= soundUART_CNTR_ENABLE;
        #endif /* End soundUART_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        soundUART_TXSTATUS_ACTL_REG |= soundUART_INT_ENABLE;
        #if (soundUART_TX_INTERRUPT_ENABLED)
            soundUART_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            soundUART_EnableTxInt();
        #endif /* (soundUART_TX_INTERRUPT_ENABLED) */
     #endif /* (soundUART_TX_INTERRUPT_ENABLED) */

    #if (soundUART_INTERNAL_CLOCK_USED)
        soundUART_IntClock_Start();  /* Enable the clock */
    #endif /* (soundUART_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: soundUART_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void soundUART_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (soundUART_RX_ENABLED || soundUART_HD_ENABLED)
        soundUART_RXBITCTR_CONTROL_REG &= (uint8) ~soundUART_CNTR_ENABLE;
    #endif /* (soundUART_RX_ENABLED || soundUART_HD_ENABLED) */

    #if (soundUART_TX_ENABLED)
        #if(!soundUART_TXCLKGEN_DP)
            soundUART_TXBITCTR_CONTROL_REG &= (uint8) ~soundUART_CNTR_ENABLE;
        #endif /* (!soundUART_TXCLKGEN_DP) */
    #endif /* (soundUART_TX_ENABLED) */

    #if (soundUART_INTERNAL_CLOCK_USED)
        soundUART_IntClock_Stop();   /* Disable the clock */
    #endif /* (soundUART_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (soundUART_RX_ENABLED || soundUART_HD_ENABLED)
        soundUART_RXSTATUS_ACTL_REG  &= (uint8) ~soundUART_INT_ENABLE;

        #if (soundUART_RX_INTERRUPT_ENABLED)
            soundUART_DisableRxInt();
        #endif /* (soundUART_RX_INTERRUPT_ENABLED) */
    #endif /* (soundUART_RX_ENABLED || soundUART_HD_ENABLED) */

    #if (soundUART_TX_ENABLED)
        soundUART_TXSTATUS_ACTL_REG &= (uint8) ~soundUART_INT_ENABLE;

        #if (soundUART_TX_INTERRUPT_ENABLED)
            soundUART_DisableTxInt();
        #endif /* (soundUART_TX_INTERRUPT_ENABLED) */
    #endif /* (soundUART_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: soundUART_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 soundUART_ReadControlRegister(void) 
{
    #if (soundUART_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(soundUART_CONTROL_REG);
    #endif /* (soundUART_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: soundUART_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  soundUART_WriteControlRegister(uint8 control) 
{
    #if (soundUART_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       soundUART_CONTROL_REG = control;
    #endif /* (soundUART_CONTROL_REG_REMOVED) */
}


#if(soundUART_RX_ENABLED || soundUART_HD_ENABLED)
    /*******************************************************************************
    * Function Name: soundUART_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      soundUART_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      soundUART_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      soundUART_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      soundUART_RX_STS_BREAK            Interrupt on break.
    *      soundUART_RX_STS_OVERRUN          Interrupt on overrun error.
    *      soundUART_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      soundUART_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void soundUART_SetRxInterruptMode(uint8 intSrc) 
    {
        soundUART_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: soundUART_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  soundUART_rxBuffer - RAM buffer pointer for save received data.
    *  soundUART_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  soundUART_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  soundUART_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 soundUART_ReadRxData(void) 
    {
        uint8 rxData;

    #if (soundUART_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        soundUART_DisableRxInt();

        locRxBufferRead  = soundUART_rxBufferRead;
        locRxBufferWrite = soundUART_rxBufferWrite;

        if( (soundUART_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = soundUART_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= soundUART_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            soundUART_rxBufferRead = locRxBufferRead;

            if(soundUART_rxBufferLoopDetect != 0u)
            {
                soundUART_rxBufferLoopDetect = 0u;
                #if ((soundUART_RX_INTERRUPT_ENABLED) && (soundUART_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( soundUART_HD_ENABLED )
                        if((soundUART_CONTROL_REG & soundUART_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            soundUART_RXSTATUS_MASK_REG  |= soundUART_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        soundUART_RXSTATUS_MASK_REG  |= soundUART_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end soundUART_HD_ENABLED */
                #endif /* ((soundUART_RX_INTERRUPT_ENABLED) && (soundUART_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = soundUART_RXDATA_REG;
        }

        soundUART_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = soundUART_RXDATA_REG;

    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: soundUART_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  soundUART_RX_STS_FIFO_NOTEMPTY.
    *  soundUART_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  soundUART_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   soundUART_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   soundUART_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 soundUART_ReadRxStatus(void) 
    {
        uint8 status;

        status = soundUART_RXSTATUS_REG & soundUART_RX_HW_MASK;

    #if (soundUART_RX_INTERRUPT_ENABLED)
        if(soundUART_rxBufferOverflow != 0u)
        {
            status |= soundUART_RX_STS_SOFT_BUFF_OVER;
            soundUART_rxBufferOverflow = 0u;
        }
    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: soundUART_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. soundUART_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  soundUART_rxBuffer - RAM buffer pointer for save received data.
    *  soundUART_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  soundUART_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  soundUART_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 soundUART_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (soundUART_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        soundUART_DisableRxInt();

        locRxBufferRead  = soundUART_rxBufferRead;
        locRxBufferWrite = soundUART_rxBufferWrite;

        if( (soundUART_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = soundUART_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= soundUART_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            soundUART_rxBufferRead = locRxBufferRead;

            if(soundUART_rxBufferLoopDetect != 0u)
            {
                soundUART_rxBufferLoopDetect = 0u;
                #if( (soundUART_RX_INTERRUPT_ENABLED) && (soundUART_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( soundUART_HD_ENABLED )
                        if((soundUART_CONTROL_REG & soundUART_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            soundUART_RXSTATUS_MASK_REG |= soundUART_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        soundUART_RXSTATUS_MASK_REG |= soundUART_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end soundUART_HD_ENABLED */
                #endif /* soundUART_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = soundUART_RXSTATUS_REG;
            if((rxStatus & soundUART_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = soundUART_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (soundUART_RX_STS_BREAK | soundUART_RX_STS_PAR_ERROR |
                                soundUART_RX_STS_STOP_ERROR | soundUART_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        soundUART_EnableRxInt();

    #else

        rxStatus =soundUART_RXSTATUS_REG;
        if((rxStatus & soundUART_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = soundUART_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (soundUART_RX_STS_BREAK | soundUART_RX_STS_PAR_ERROR |
                            soundUART_RX_STS_STOP_ERROR | soundUART_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: soundUART_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 soundUART_GetByte(void) 
    {
        
    #if (soundUART_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        soundUART_DisableRxInt();
        locErrorStatus = (uint16)soundUART_errorStatus;
        soundUART_errorStatus = 0u;
        soundUART_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | soundUART_ReadRxData() );
    #else
        return ( ((uint16)soundUART_ReadRxStatus() << 8u) | soundUART_ReadRxData() );
    #endif /* soundUART_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: soundUART_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  soundUART_rxBufferWrite - used to calculate left bytes.
    *  soundUART_rxBufferRead - used to calculate left bytes.
    *  soundUART_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 soundUART_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (soundUART_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        soundUART_DisableRxInt();

        if(soundUART_rxBufferRead == soundUART_rxBufferWrite)
        {
            if(soundUART_rxBufferLoopDetect != 0u)
            {
                size = soundUART_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(soundUART_rxBufferRead < soundUART_rxBufferWrite)
        {
            size = (soundUART_rxBufferWrite - soundUART_rxBufferRead);
        }
        else
        {
            size = (soundUART_RX_BUFFER_SIZE - soundUART_rxBufferRead) + soundUART_rxBufferWrite;
        }

        soundUART_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((soundUART_RXSTATUS_REG & soundUART_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: soundUART_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_rxBufferWrite - cleared to zero.
    *  soundUART_rxBufferRead - cleared to zero.
    *  soundUART_rxBufferLoopDetect - cleared to zero.
    *  soundUART_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void soundUART_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        soundUART_RXDATA_AUX_CTL_REG |= (uint8)  soundUART_RX_FIFO_CLR;
        soundUART_RXDATA_AUX_CTL_REG &= (uint8) ~soundUART_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (soundUART_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        soundUART_DisableRxInt();

        soundUART_rxBufferRead = 0u;
        soundUART_rxBufferWrite = 0u;
        soundUART_rxBufferLoopDetect = 0u;
        soundUART_rxBufferOverflow = 0u;

        soundUART_EnableRxInt();

    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: soundUART_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  soundUART__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  soundUART__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  soundUART__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  soundUART__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  soundUART__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  soundUART_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void soundUART_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(soundUART_RXHW_ADDRESS_ENABLED)
            #if(soundUART_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* soundUART_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = soundUART_CONTROL_REG & (uint8)~soundUART_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << soundUART_CTRL_RXADDR_MODE0_SHIFT);
                soundUART_CONTROL_REG = tmpCtrl;

                #if(soundUART_RX_INTERRUPT_ENABLED && \
                   (soundUART_RXBUFFERSIZE > soundUART_FIFO_LENGTH) )
                    soundUART_rxAddressMode = addressMode;
                    soundUART_rxAddressDetected = 0u;
                #endif /* End soundUART_RXBUFFERSIZE > soundUART_FIFO_LENGTH*/
            #endif /* End soundUART_CONTROL_REG_REMOVED */
        #else /* soundUART_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End soundUART_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: soundUART_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void soundUART_SetRxAddress1(uint8 address) 
    {
        soundUART_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: soundUART_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void soundUART_SetRxAddress2(uint8 address) 
    {
        soundUART_RXADDRESS2_REG = address;
    }

#endif  /* soundUART_RX_ENABLED || soundUART_HD_ENABLED*/


#if( (soundUART_TX_ENABLED) || (soundUART_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: soundUART_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   soundUART_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   soundUART_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   soundUART_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   soundUART_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void soundUART_SetTxInterruptMode(uint8 intSrc) 
    {
        soundUART_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: soundUART_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  soundUART_txBuffer - RAM buffer pointer for save data for transmission
    *  soundUART_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  soundUART_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  soundUART_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void soundUART_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(soundUART_initVar != 0u)
        {
        #if (soundUART_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            soundUART_DisableTxInt();

            if( (soundUART_txBufferRead == soundUART_txBufferWrite) &&
                ((soundUART_TXSTATUS_REG & soundUART_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                soundUART_TXDATA_REG = txDataByte;
            }
            else
            {
                if(soundUART_txBufferWrite >= soundUART_TX_BUFFER_SIZE)
                {
                    soundUART_txBufferWrite = 0u;
                }

                soundUART_txBuffer[soundUART_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                soundUART_txBufferWrite++;
            }

            soundUART_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            soundUART_TXDATA_REG = txDataByte;

        #endif /*(soundUART_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: soundUART_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 soundUART_ReadTxStatus(void) 
    {
        return(soundUART_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: soundUART_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_txBuffer - RAM buffer pointer for save data for transmission
    *  soundUART_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  soundUART_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  soundUART_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void soundUART_PutChar(uint8 txDataByte) 
    {
    #if (soundUART_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            soundUART_DisableTxInt();
        #endif /* (soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = soundUART_txBufferWrite;
            locTxBufferRead  = soundUART_txBufferRead;

        #if ((soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            soundUART_EnableTxInt();
        #endif /* (soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(soundUART_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((soundUART_TXSTATUS_REG & soundUART_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            soundUART_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= soundUART_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            soundUART_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3))
            soundUART_DisableTxInt();
        #endif /* (soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3) */

            soundUART_txBufferWrite = locTxBufferWrite;

        #if ((soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3))
            soundUART_EnableTxInt();
        #endif /* (soundUART_TX_BUFFER_SIZE > soundUART_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (soundUART_TXSTATUS_REG & soundUART_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                soundUART_SetPendingTxInt();
            }
        }

    #else

        while((soundUART_TXSTATUS_REG & soundUART_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        soundUART_TXDATA_REG = txDataByte;

    #endif /* soundUART_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: soundUART_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void soundUART_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(soundUART_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                soundUART_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: soundUART_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void soundUART_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(soundUART_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                soundUART_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: soundUART_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void soundUART_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(soundUART_initVar != 0u)
        {
            soundUART_PutChar(txDataByte);
            soundUART_PutChar(0x0Du);
            soundUART_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: soundUART_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  soundUART_txBufferWrite - used to calculate left space.
    *  soundUART_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 soundUART_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (soundUART_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        soundUART_DisableTxInt();

        if(soundUART_txBufferRead == soundUART_txBufferWrite)
        {
            size = 0u;
        }
        else if(soundUART_txBufferRead < soundUART_txBufferWrite)
        {
            size = (soundUART_txBufferWrite - soundUART_txBufferRead);
        }
        else
        {
            size = (soundUART_TX_BUFFER_SIZE - soundUART_txBufferRead) +
                    soundUART_txBufferWrite;
        }

        soundUART_EnableTxInt();

    #else

        size = soundUART_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & soundUART_TX_STS_FIFO_FULL) != 0u)
        {
            size = soundUART_FIFO_LENGTH;
        }
        else if((size & soundUART_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (soundUART_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: soundUART_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_txBufferWrite - cleared to zero.
    *  soundUART_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void soundUART_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        soundUART_TXDATA_AUX_CTL_REG |= (uint8)  soundUART_TX_FIFO_CLR;
        soundUART_TXDATA_AUX_CTL_REG &= (uint8) ~soundUART_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (soundUART_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        soundUART_DisableTxInt();

        soundUART_txBufferRead = 0u;
        soundUART_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        soundUART_EnableTxInt();

    #endif /* (soundUART_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: soundUART_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   soundUART_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   soundUART_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   soundUART_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   soundUART_SEND_WAIT_REINIT - Performs both options: 
    *      soundUART_SEND_BREAK and soundUART_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with soundUART_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The soundUART_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void soundUART_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(soundUART_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(soundUART_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == soundUART_SEND_BREAK) ||
                (retMode == soundUART_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                soundUART_WriteControlRegister(soundUART_ReadControlRegister() |
                                                      soundUART_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                soundUART_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = soundUART_TXSTATUS_REG;
                }
                while((tmpStat & soundUART_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == soundUART_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == soundUART_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = soundUART_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & soundUART_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == soundUART_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == soundUART_REINIT) ||
                (retMode == soundUART_SEND_WAIT_REINIT) )
            {
                soundUART_WriteControlRegister(soundUART_ReadControlRegister() &
                                              (uint8)~soundUART_CTRL_HD_SEND_BREAK);
            }

        #else /* soundUART_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == soundUART_SEND_BREAK) ||
                (retMode == soundUART_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (soundUART_PARITY_TYPE != soundUART__B_UART__NONE_REVB) || \
                                    (soundUART_PARITY_TYPE_SW != 0u) )
                    soundUART_WriteControlRegister(soundUART_ReadControlRegister() |
                                                          soundUART_CTRL_HD_SEND_BREAK);
                #endif /* End soundUART_PARITY_TYPE != soundUART__B_UART__NONE_REVB  */

                #if(soundUART_TXCLKGEN_DP)
                    txPeriod = soundUART_TXBITCLKTX_COMPLETE_REG;
                    soundUART_TXBITCLKTX_COMPLETE_REG = soundUART_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = soundUART_TXBITCTR_PERIOD_REG;
                    soundUART_TXBITCTR_PERIOD_REG = soundUART_TXBITCTR_BREAKBITS8X;
                #endif /* End soundUART_TXCLKGEN_DP */

                /* Send zeros */
                soundUART_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = soundUART_TXSTATUS_REG;
                }
                while((tmpStat & soundUART_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == soundUART_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == soundUART_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = soundUART_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & soundUART_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == soundUART_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == soundUART_REINIT) ||
                (retMode == soundUART_SEND_WAIT_REINIT) )
            {

            #if(soundUART_TXCLKGEN_DP)
                soundUART_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                soundUART_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End soundUART_TXCLKGEN_DP */

            #if( (soundUART_PARITY_TYPE != soundUART__B_UART__NONE_REVB) || \
                 (soundUART_PARITY_TYPE_SW != 0u) )
                soundUART_WriteControlRegister(soundUART_ReadControlRegister() &
                                                      (uint8) ~soundUART_CTRL_HD_SEND_BREAK);
            #endif /* End soundUART_PARITY_TYPE != NONE */
            }
        #endif    /* End soundUART_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: soundUART_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       soundUART_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       soundUART_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears soundUART_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void soundUART_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( soundUART_CONTROL_REG_REMOVED == 0u )
            soundUART_WriteControlRegister(soundUART_ReadControlRegister() |
                                                  soundUART_CTRL_MARK);
        #endif /* End soundUART_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( soundUART_CONTROL_REG_REMOVED == 0u )
            soundUART_WriteControlRegister(soundUART_ReadControlRegister() &
                                                  (uint8) ~soundUART_CTRL_MARK);
        #endif /* End soundUART_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndsoundUART_TX_ENABLED */

#if(soundUART_HD_ENABLED)


    /*******************************************************************************
    * Function Name: soundUART_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void soundUART_LoadRxConfig(void) 
    {
        soundUART_WriteControlRegister(soundUART_ReadControlRegister() &
                                                (uint8)~soundUART_CTRL_HD_SEND);
        soundUART_RXBITCTR_PERIOD_REG = soundUART_HD_RXBITCTR_INIT;

    #if (soundUART_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        soundUART_SetRxInterruptMode(soundUART_INIT_RX_INTERRUPTS_MASK);
    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: soundUART_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void soundUART_LoadTxConfig(void) 
    {
    #if (soundUART_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        soundUART_SetRxInterruptMode(0u);
    #endif /* (soundUART_RX_INTERRUPT_ENABLED) */

        soundUART_WriteControlRegister(soundUART_ReadControlRegister() | soundUART_CTRL_HD_SEND);
        soundUART_RXBITCTR_PERIOD_REG = soundUART_HD_TXBITCTR_INIT;
    }

#endif  /* soundUART_HD_ENABLED */


/* [] END OF FILE */
