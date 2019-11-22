/*******************************************************************************
* File Name: soundUARTINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "soundUART.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (soundUART_RX_INTERRUPT_ENABLED && (soundUART_RX_ENABLED || soundUART_HD_ENABLED))
    /*******************************************************************************
    * Function Name: soundUART_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_rxBuffer - RAM buffer pointer for save received data.
    *  soundUART_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  soundUART_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  soundUART_rxBufferOverflow - software overflow flag. Set to one
    *     when soundUART_rxBufferWrite index overtakes
    *     soundUART_rxBufferRead index.
    *  soundUART_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when soundUART_rxBufferWrite is equal to
    *    soundUART_rxBufferRead
    *  soundUART_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  soundUART_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(soundUART_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef soundUART_RXISR_ENTRY_CALLBACK
        soundUART_RXISR_EntryCallback();
    #endif /* soundUART_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START soundUART_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = soundUART_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in soundUART_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (soundUART_RX_STS_BREAK | 
                            soundUART_RX_STS_PAR_ERROR |
                            soundUART_RX_STS_STOP_ERROR | 
                            soundUART_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                soundUART_errorStatus |= readStatus & ( soundUART_RX_STS_BREAK | 
                                                            soundUART_RX_STS_PAR_ERROR | 
                                                            soundUART_RX_STS_STOP_ERROR | 
                                                            soundUART_RX_STS_OVERRUN);
                /* `#START soundUART_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef soundUART_RXISR_ERROR_CALLBACK
                soundUART_RXISR_ERROR_Callback();
            #endif /* soundUART_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & soundUART_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = soundUART_RXDATA_REG;
            #if (soundUART_RXHW_ADDRESS_ENABLED)
                if(soundUART_rxAddressMode == (uint8)soundUART__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & soundUART_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & soundUART_RX_STS_ADDR_MATCH) != 0u)
                        {
                            soundUART_rxAddressDetected = 1u;
                        }
                        else
                        {
                            soundUART_rxAddressDetected = 0u;
                        }
                    }
                    if(soundUART_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        soundUART_rxBuffer[soundUART_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    soundUART_rxBuffer[soundUART_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                soundUART_rxBuffer[soundUART_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (soundUART_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(soundUART_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        soundUART_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    soundUART_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(soundUART_rxBufferWrite >= soundUART_RX_BUFFER_SIZE)
                    {
                        soundUART_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(soundUART_rxBufferWrite == soundUART_rxBufferRead)
                    {
                        soundUART_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (soundUART_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            soundUART_RXSTATUS_MASK_REG  &= (uint8)~soundUART_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(soundUART_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (soundUART_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & soundUART_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START soundUART_RXISR_END` */

        /* `#END` */

    #ifdef soundUART_RXISR_EXIT_CALLBACK
        soundUART_RXISR_ExitCallback();
    #endif /* soundUART_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (soundUART_RX_INTERRUPT_ENABLED && (soundUART_RX_ENABLED || soundUART_HD_ENABLED)) */


#if (soundUART_TX_INTERRUPT_ENABLED && soundUART_TX_ENABLED)
    /*******************************************************************************
    * Function Name: soundUART_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  soundUART_txBuffer - RAM buffer pointer for transmit data from.
    *  soundUART_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  soundUART_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(soundUART_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef soundUART_TXISR_ENTRY_CALLBACK
        soundUART_TXISR_EntryCallback();
    #endif /* soundUART_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START soundUART_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((soundUART_txBufferRead != soundUART_txBufferWrite) &&
             ((soundUART_TXSTATUS_REG & soundUART_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(soundUART_txBufferRead >= soundUART_TX_BUFFER_SIZE)
            {
                soundUART_txBufferRead = 0u;
            }

            soundUART_TXDATA_REG = soundUART_txBuffer[soundUART_txBufferRead];

            /* Set next pointer */
            soundUART_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START soundUART_TXISR_END` */

        /* `#END` */

    #ifdef soundUART_TXISR_EXIT_CALLBACK
        soundUART_TXISR_ExitCallback();
    #endif /* soundUART_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (soundUART_TX_INTERRUPT_ENABLED && soundUART_TX_ENABLED) */


/* [] END OF FILE */
