/*******************************************************************************
* File Name: soundUART.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_soundUART_H)
#define CY_UART_soundUART_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define soundUART_RX_ENABLED                     (0u)
#define soundUART_TX_ENABLED                     (1u)
#define soundUART_HD_ENABLED                     (0u)
#define soundUART_RX_INTERRUPT_ENABLED           (0u)
#define soundUART_TX_INTERRUPT_ENABLED           (0u)
#define soundUART_INTERNAL_CLOCK_USED            (1u)
#define soundUART_RXHW_ADDRESS_ENABLED           (0u)
#define soundUART_OVER_SAMPLE_COUNT              (8u)
#define soundUART_PARITY_TYPE                    (0u)
#define soundUART_PARITY_TYPE_SW                 (0u)
#define soundUART_BREAK_DETECT                   (0u)
#define soundUART_BREAK_BITS_TX                  (13u)
#define soundUART_BREAK_BITS_RX                  (13u)
#define soundUART_TXCLKGEN_DP                    (1u)
#define soundUART_USE23POLLING                   (1u)
#define soundUART_FLOW_CONTROL                   (0u)
#define soundUART_CLK_FREQ                       (0u)
#define soundUART_TX_BUFFER_SIZE                 (4u)
#define soundUART_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define soundUART_CONTROL_REG_REMOVED            (0u)
#else
    #define soundUART_CONTROL_REG_REMOVED            (1u)
#endif /* End soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct soundUART_backupStruct_
{
    uint8 enableState;

    #if(soundUART_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End soundUART_CONTROL_REG_REMOVED */

} soundUART_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void soundUART_Start(void) ;
void soundUART_Stop(void) ;
uint8 soundUART_ReadControlRegister(void) ;
void soundUART_WriteControlRegister(uint8 control) ;

void soundUART_Init(void) ;
void soundUART_Enable(void) ;
void soundUART_SaveConfig(void) ;
void soundUART_RestoreConfig(void) ;
void soundUART_Sleep(void) ;
void soundUART_Wakeup(void) ;

/* Only if RX is enabled */
#if( (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) )

    #if (soundUART_RX_INTERRUPT_ENABLED)
        #define soundUART_EnableRxInt()  CyIntEnable (soundUART_RX_VECT_NUM)
        #define soundUART_DisableRxInt() CyIntDisable(soundUART_RX_VECT_NUM)
        CY_ISR_PROTO(soundUART_RXISR);
    #endif /* soundUART_RX_INTERRUPT_ENABLED */

    void soundUART_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void soundUART_SetRxAddress1(uint8 address) ;
    void soundUART_SetRxAddress2(uint8 address) ;

    void  soundUART_SetRxInterruptMode(uint8 intSrc) ;
    uint8 soundUART_ReadRxData(void) ;
    uint8 soundUART_ReadRxStatus(void) ;
    uint8 soundUART_GetChar(void) ;
    uint16 soundUART_GetByte(void) ;
    uint8 soundUART_GetRxBufferSize(void)
                                                            ;
    void soundUART_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define soundUART_GetRxInterruptSource   soundUART_ReadRxStatus

#endif /* End (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) */

/* Only if TX is enabled */
#if(soundUART_TX_ENABLED || soundUART_HD_ENABLED)

    #if(soundUART_TX_INTERRUPT_ENABLED)
        #define soundUART_EnableTxInt()  CyIntEnable (soundUART_TX_VECT_NUM)
        #define soundUART_DisableTxInt() CyIntDisable(soundUART_TX_VECT_NUM)
        #define soundUART_SetPendingTxInt() CyIntSetPending(soundUART_TX_VECT_NUM)
        #define soundUART_ClearPendingTxInt() CyIntClearPending(soundUART_TX_VECT_NUM)
        CY_ISR_PROTO(soundUART_TXISR);
    #endif /* soundUART_TX_INTERRUPT_ENABLED */

    void soundUART_SetTxInterruptMode(uint8 intSrc) ;
    void soundUART_WriteTxData(uint8 txDataByte) ;
    uint8 soundUART_ReadTxStatus(void) ;
    void soundUART_PutChar(uint8 txDataByte) ;
    void soundUART_PutString(const char8 string[]) ;
    void soundUART_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void soundUART_PutCRLF(uint8 txDataByte) ;
    void soundUART_ClearTxBuffer(void) ;
    void soundUART_SetTxAddressMode(uint8 addressMode) ;
    void soundUART_SendBreak(uint8 retMode) ;
    uint8 soundUART_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define soundUART_PutStringConst         soundUART_PutString
    #define soundUART_PutArrayConst          soundUART_PutArray
    #define soundUART_GetTxInterruptSource   soundUART_ReadTxStatus

#endif /* End soundUART_TX_ENABLED || soundUART_HD_ENABLED */

#if(soundUART_HD_ENABLED)
    void soundUART_LoadRxConfig(void) ;
    void soundUART_LoadTxConfig(void) ;
#endif /* End soundUART_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_soundUART) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    soundUART_CyBtldrCommStart(void) CYSMALL ;
    void    soundUART_CyBtldrCommStop(void) CYSMALL ;
    void    soundUART_CyBtldrCommReset(void) CYSMALL ;
    cystatus soundUART_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus soundUART_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_soundUART)
        #define CyBtldrCommStart    soundUART_CyBtldrCommStart
        #define CyBtldrCommStop     soundUART_CyBtldrCommStop
        #define CyBtldrCommReset    soundUART_CyBtldrCommReset
        #define CyBtldrCommWrite    soundUART_CyBtldrCommWrite
        #define CyBtldrCommRead     soundUART_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_soundUART) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define soundUART_BYTE2BYTE_TIME_OUT (25u)
    #define soundUART_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define soundUART_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define soundUART_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define soundUART_SET_SPACE      (0x00u)
#define soundUART_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (soundUART_TX_ENABLED) || (soundUART_HD_ENABLED) )
    #if(soundUART_TX_INTERRUPT_ENABLED)
        #define soundUART_TX_VECT_NUM            (uint8)soundUART_TXInternalInterrupt__INTC_NUMBER
        #define soundUART_TX_PRIOR_NUM           (uint8)soundUART_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* soundUART_TX_INTERRUPT_ENABLED */

    #define soundUART_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define soundUART_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define soundUART_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(soundUART_TX_ENABLED)
        #define soundUART_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (soundUART_HD_ENABLED) */
        #define soundUART_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (soundUART_TX_ENABLED) */

    #define soundUART_TX_STS_COMPLETE            (uint8)(0x01u << soundUART_TX_STS_COMPLETE_SHIFT)
    #define soundUART_TX_STS_FIFO_EMPTY          (uint8)(0x01u << soundUART_TX_STS_FIFO_EMPTY_SHIFT)
    #define soundUART_TX_STS_FIFO_FULL           (uint8)(0x01u << soundUART_TX_STS_FIFO_FULL_SHIFT)
    #define soundUART_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << soundUART_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (soundUART_TX_ENABLED) || (soundUART_HD_ENABLED)*/

#if( (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) )
    #if(soundUART_RX_INTERRUPT_ENABLED)
        #define soundUART_RX_VECT_NUM            (uint8)soundUART_RXInternalInterrupt__INTC_NUMBER
        #define soundUART_RX_PRIOR_NUM           (uint8)soundUART_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* soundUART_RX_INTERRUPT_ENABLED */
    #define soundUART_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define soundUART_RX_STS_BREAK_SHIFT             (0x01u)
    #define soundUART_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define soundUART_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define soundUART_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define soundUART_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define soundUART_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define soundUART_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define soundUART_RX_STS_MRKSPC           (uint8)(0x01u << soundUART_RX_STS_MRKSPC_SHIFT)
    #define soundUART_RX_STS_BREAK            (uint8)(0x01u << soundUART_RX_STS_BREAK_SHIFT)
    #define soundUART_RX_STS_PAR_ERROR        (uint8)(0x01u << soundUART_RX_STS_PAR_ERROR_SHIFT)
    #define soundUART_RX_STS_STOP_ERROR       (uint8)(0x01u << soundUART_RX_STS_STOP_ERROR_SHIFT)
    #define soundUART_RX_STS_OVERRUN          (uint8)(0x01u << soundUART_RX_STS_OVERRUN_SHIFT)
    #define soundUART_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << soundUART_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define soundUART_RX_STS_ADDR_MATCH       (uint8)(0x01u << soundUART_RX_STS_ADDR_MATCH_SHIFT)
    #define soundUART_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << soundUART_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define soundUART_RX_HW_MASK                     (0x7Fu)
#endif /* End (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) */

/* Control Register definitions */
#define soundUART_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define soundUART_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define soundUART_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define soundUART_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define soundUART_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define soundUART_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define soundUART_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define soundUART_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define soundUART_CTRL_HD_SEND               (uint8)(0x01u << soundUART_CTRL_HD_SEND_SHIFT)
#define soundUART_CTRL_HD_SEND_BREAK         (uint8)(0x01u << soundUART_CTRL_HD_SEND_BREAK_SHIFT)
#define soundUART_CTRL_MARK                  (uint8)(0x01u << soundUART_CTRL_MARK_SHIFT)
#define soundUART_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << soundUART_CTRL_PARITY_TYPE0_SHIFT)
#define soundUART_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << soundUART_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define soundUART_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define soundUART_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define soundUART_SEND_BREAK                         (0x00u)
#define soundUART_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define soundUART_REINIT                             (0x02u)
#define soundUART_SEND_WAIT_REINIT                   (0x03u)

#define soundUART_OVER_SAMPLE_8                      (8u)
#define soundUART_OVER_SAMPLE_16                     (16u)

#define soundUART_BIT_CENTER                         (soundUART_OVER_SAMPLE_COUNT - 2u)

#define soundUART_FIFO_LENGTH                        (4u)
#define soundUART_NUMBER_OF_START_BIT                (1u)
#define soundUART_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define soundUART_TXBITCTR_BREAKBITS8X   ((soundUART_BREAK_BITS_TX * soundUART_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define soundUART_TXBITCTR_BREAKBITS ((soundUART_BREAK_BITS_TX * soundUART_OVER_SAMPLE_COUNT) - 1u)

#define soundUART_HALF_BIT_COUNT   \
                            (((soundUART_OVER_SAMPLE_COUNT / 2u) + (soundUART_USE23POLLING * 1u)) - 2u)
#if (soundUART_OVER_SAMPLE_COUNT == soundUART_OVER_SAMPLE_8)
    #define soundUART_HD_TXBITCTR_INIT   (((soundUART_BREAK_BITS_TX + \
                            soundUART_NUMBER_OF_START_BIT) * soundUART_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define soundUART_RXBITCTR_INIT  ((((soundUART_BREAK_BITS_RX + soundUART_NUMBER_OF_START_BIT) \
                            * soundUART_OVER_SAMPLE_COUNT) + soundUART_HALF_BIT_COUNT) - 1u)

#else /* soundUART_OVER_SAMPLE_COUNT == soundUART_OVER_SAMPLE_16 */
    #define soundUART_HD_TXBITCTR_INIT   ((8u * soundUART_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define soundUART_RXBITCTR_INIT      (((7u * soundUART_OVER_SAMPLE_COUNT) - 1u) + \
                                                      soundUART_HALF_BIT_COUNT)
#endif /* End soundUART_OVER_SAMPLE_COUNT */

#define soundUART_HD_RXBITCTR_INIT                   soundUART_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 soundUART_initVar;
#if (soundUART_TX_INTERRUPT_ENABLED && soundUART_TX_ENABLED)
    extern volatile uint8 soundUART_txBuffer[soundUART_TX_BUFFER_SIZE];
    extern volatile uint8 soundUART_txBufferRead;
    extern uint8 soundUART_txBufferWrite;
#endif /* (soundUART_TX_INTERRUPT_ENABLED && soundUART_TX_ENABLED) */
#if (soundUART_RX_INTERRUPT_ENABLED && (soundUART_RX_ENABLED || soundUART_HD_ENABLED))
    extern uint8 soundUART_errorStatus;
    extern volatile uint8 soundUART_rxBuffer[soundUART_RX_BUFFER_SIZE];
    extern volatile uint8 soundUART_rxBufferRead;
    extern volatile uint8 soundUART_rxBufferWrite;
    extern volatile uint8 soundUART_rxBufferLoopDetect;
    extern volatile uint8 soundUART_rxBufferOverflow;
    #if (soundUART_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 soundUART_rxAddressMode;
        extern volatile uint8 soundUART_rxAddressDetected;
    #endif /* (soundUART_RXHW_ADDRESS_ENABLED) */
#endif /* (soundUART_RX_INTERRUPT_ENABLED && (soundUART_RX_ENABLED || soundUART_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define soundUART__B_UART__AM_SW_BYTE_BYTE 1
#define soundUART__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define soundUART__B_UART__AM_HW_BYTE_BY_BYTE 3
#define soundUART__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define soundUART__B_UART__AM_NONE 0

#define soundUART__B_UART__NONE_REVB 0
#define soundUART__B_UART__EVEN_REVB 1
#define soundUART__B_UART__ODD_REVB 2
#define soundUART__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define soundUART_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define soundUART_NUMBER_OF_STOP_BITS    (1u)

#if (soundUART_RXHW_ADDRESS_ENABLED)
    #define soundUART_RX_ADDRESS_MODE    (0u)
    #define soundUART_RX_HW_ADDRESS1     (0u)
    #define soundUART_RX_HW_ADDRESS2     (0u)
#endif /* (soundUART_RXHW_ADDRESS_ENABLED) */

#define soundUART_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((0 << soundUART_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << soundUART_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << soundUART_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << soundUART_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << soundUART_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << soundUART_RX_STS_BREAK_SHIFT) \
                                        | (0 << soundUART_RX_STS_OVERRUN_SHIFT))

#define soundUART_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << soundUART_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << soundUART_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << soundUART_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << soundUART_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define soundUART_CONTROL_REG \
                            (* (reg8 *) soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define soundUART_CONTROL_PTR \
                            (  (reg8 *) soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(soundUART_TX_ENABLED)
    #define soundUART_TXDATA_REG          (* (reg8 *) soundUART_BUART_sTX_TxShifter_u0__F0_REG)
    #define soundUART_TXDATA_PTR          (  (reg8 *) soundUART_BUART_sTX_TxShifter_u0__F0_REG)
    #define soundUART_TXDATA_AUX_CTL_REG  (* (reg8 *) soundUART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define soundUART_TXDATA_AUX_CTL_PTR  (  (reg8 *) soundUART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define soundUART_TXSTATUS_REG        (* (reg8 *) soundUART_BUART_sTX_TxSts__STATUS_REG)
    #define soundUART_TXSTATUS_PTR        (  (reg8 *) soundUART_BUART_sTX_TxSts__STATUS_REG)
    #define soundUART_TXSTATUS_MASK_REG   (* (reg8 *) soundUART_BUART_sTX_TxSts__MASK_REG)
    #define soundUART_TXSTATUS_MASK_PTR   (  (reg8 *) soundUART_BUART_sTX_TxSts__MASK_REG)
    #define soundUART_TXSTATUS_ACTL_REG   (* (reg8 *) soundUART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define soundUART_TXSTATUS_ACTL_PTR   (  (reg8 *) soundUART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(soundUART_TXCLKGEN_DP)
        #define soundUART_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define soundUART_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define soundUART_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define soundUART_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define soundUART_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define soundUART_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define soundUART_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define soundUART_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define soundUART_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define soundUART_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) soundUART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* soundUART_TXCLKGEN_DP */

#endif /* End soundUART_TX_ENABLED */

#if(soundUART_HD_ENABLED)

    #define soundUART_TXDATA_REG             (* (reg8 *) soundUART_BUART_sRX_RxShifter_u0__F1_REG )
    #define soundUART_TXDATA_PTR             (  (reg8 *) soundUART_BUART_sRX_RxShifter_u0__F1_REG )
    #define soundUART_TXDATA_AUX_CTL_REG     (* (reg8 *) soundUART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define soundUART_TXDATA_AUX_CTL_PTR     (  (reg8 *) soundUART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define soundUART_TXSTATUS_REG           (* (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_REG )
    #define soundUART_TXSTATUS_PTR           (  (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_REG )
    #define soundUART_TXSTATUS_MASK_REG      (* (reg8 *) soundUART_BUART_sRX_RxSts__MASK_REG )
    #define soundUART_TXSTATUS_MASK_PTR      (  (reg8 *) soundUART_BUART_sRX_RxSts__MASK_REG )
    #define soundUART_TXSTATUS_ACTL_REG      (* (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define soundUART_TXSTATUS_ACTL_PTR      (  (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End soundUART_HD_ENABLED */

#if( (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) )
    #define soundUART_RXDATA_REG             (* (reg8 *) soundUART_BUART_sRX_RxShifter_u0__F0_REG )
    #define soundUART_RXDATA_PTR             (  (reg8 *) soundUART_BUART_sRX_RxShifter_u0__F0_REG )
    #define soundUART_RXADDRESS1_REG         (* (reg8 *) soundUART_BUART_sRX_RxShifter_u0__D0_REG )
    #define soundUART_RXADDRESS1_PTR         (  (reg8 *) soundUART_BUART_sRX_RxShifter_u0__D0_REG )
    #define soundUART_RXADDRESS2_REG         (* (reg8 *) soundUART_BUART_sRX_RxShifter_u0__D1_REG )
    #define soundUART_RXADDRESS2_PTR         (  (reg8 *) soundUART_BUART_sRX_RxShifter_u0__D1_REG )
    #define soundUART_RXDATA_AUX_CTL_REG     (* (reg8 *) soundUART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define soundUART_RXBITCTR_PERIOD_REG    (* (reg8 *) soundUART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define soundUART_RXBITCTR_PERIOD_PTR    (  (reg8 *) soundUART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define soundUART_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) soundUART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define soundUART_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) soundUART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define soundUART_RXBITCTR_COUNTER_REG   (* (reg8 *) soundUART_BUART_sRX_RxBitCounter__COUNT_REG )
    #define soundUART_RXBITCTR_COUNTER_PTR   (  (reg8 *) soundUART_BUART_sRX_RxBitCounter__COUNT_REG )

    #define soundUART_RXSTATUS_REG           (* (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_REG )
    #define soundUART_RXSTATUS_PTR           (  (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_REG )
    #define soundUART_RXSTATUS_MASK_REG      (* (reg8 *) soundUART_BUART_sRX_RxSts__MASK_REG )
    #define soundUART_RXSTATUS_MASK_PTR      (  (reg8 *) soundUART_BUART_sRX_RxSts__MASK_REG )
    #define soundUART_RXSTATUS_ACTL_REG      (* (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define soundUART_RXSTATUS_ACTL_PTR      (  (reg8 *) soundUART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) */

#if(soundUART_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define soundUART_INTCLOCK_CLKEN_REG     (* (reg8 *) soundUART_IntClock__PM_ACT_CFG)
    #define soundUART_INTCLOCK_CLKEN_PTR     (  (reg8 *) soundUART_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define soundUART_INTCLOCK_CLKEN_MASK    soundUART_IntClock__PM_ACT_MSK
#endif /* End soundUART_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(soundUART_TX_ENABLED)
    #define soundUART_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End soundUART_TX_ENABLED */

#if(soundUART_HD_ENABLED)
    #define soundUART_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End soundUART_HD_ENABLED */

#if( (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) )
    #define soundUART_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define soundUART_WAIT_1_MS      soundUART_BL_CHK_DELAY_MS   

#define soundUART_TXBUFFERSIZE   soundUART_TX_BUFFER_SIZE
#define soundUART_RXBUFFERSIZE   soundUART_RX_BUFFER_SIZE

#if (soundUART_RXHW_ADDRESS_ENABLED)
    #define soundUART_RXADDRESSMODE  soundUART_RX_ADDRESS_MODE
    #define soundUART_RXHWADDRESS1   soundUART_RX_HW_ADDRESS1
    #define soundUART_RXHWADDRESS2   soundUART_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define soundUART_RXAddressMode  soundUART_RXADDRESSMODE
#endif /* (soundUART_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define soundUART_initvar                    soundUART_initVar

#define soundUART_RX_Enabled                 soundUART_RX_ENABLED
#define soundUART_TX_Enabled                 soundUART_TX_ENABLED
#define soundUART_HD_Enabled                 soundUART_HD_ENABLED
#define soundUART_RX_IntInterruptEnabled     soundUART_RX_INTERRUPT_ENABLED
#define soundUART_TX_IntInterruptEnabled     soundUART_TX_INTERRUPT_ENABLED
#define soundUART_InternalClockUsed          soundUART_INTERNAL_CLOCK_USED
#define soundUART_RXHW_Address_Enabled       soundUART_RXHW_ADDRESS_ENABLED
#define soundUART_OverSampleCount            soundUART_OVER_SAMPLE_COUNT
#define soundUART_ParityType                 soundUART_PARITY_TYPE

#if( soundUART_TX_ENABLED && (soundUART_TXBUFFERSIZE > soundUART_FIFO_LENGTH))
    #define soundUART_TXBUFFER               soundUART_txBuffer
    #define soundUART_TXBUFFERREAD           soundUART_txBufferRead
    #define soundUART_TXBUFFERWRITE          soundUART_txBufferWrite
#endif /* End soundUART_TX_ENABLED */
#if( ( soundUART_RX_ENABLED || soundUART_HD_ENABLED ) && \
     (soundUART_RXBUFFERSIZE > soundUART_FIFO_LENGTH) )
    #define soundUART_RXBUFFER               soundUART_rxBuffer
    #define soundUART_RXBUFFERREAD           soundUART_rxBufferRead
    #define soundUART_RXBUFFERWRITE          soundUART_rxBufferWrite
    #define soundUART_RXBUFFERLOOPDETECT     soundUART_rxBufferLoopDetect
    #define soundUART_RXBUFFER_OVERFLOW      soundUART_rxBufferOverflow
#endif /* End soundUART_RX_ENABLED */

#ifdef soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define soundUART_CONTROL                soundUART_CONTROL_REG
#endif /* End soundUART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(soundUART_TX_ENABLED)
    #define soundUART_TXDATA                 soundUART_TXDATA_REG
    #define soundUART_TXSTATUS               soundUART_TXSTATUS_REG
    #define soundUART_TXSTATUS_MASK          soundUART_TXSTATUS_MASK_REG
    #define soundUART_TXSTATUS_ACTL          soundUART_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(soundUART_TXCLKGEN_DP)
        #define soundUART_TXBITCLKGEN_CTR        soundUART_TXBITCLKGEN_CTR_REG
        #define soundUART_TXBITCLKTX_COMPLETE    soundUART_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define soundUART_TXBITCTR_PERIOD        soundUART_TXBITCTR_PERIOD_REG
        #define soundUART_TXBITCTR_CONTROL       soundUART_TXBITCTR_CONTROL_REG
        #define soundUART_TXBITCTR_COUNTER       soundUART_TXBITCTR_COUNTER_REG
    #endif /* soundUART_TXCLKGEN_DP */
#endif /* End soundUART_TX_ENABLED */

#if(soundUART_HD_ENABLED)
    #define soundUART_TXDATA                 soundUART_TXDATA_REG
    #define soundUART_TXSTATUS               soundUART_TXSTATUS_REG
    #define soundUART_TXSTATUS_MASK          soundUART_TXSTATUS_MASK_REG
    #define soundUART_TXSTATUS_ACTL          soundUART_TXSTATUS_ACTL_REG
#endif /* End soundUART_HD_ENABLED */

#if( (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) )
    #define soundUART_RXDATA                 soundUART_RXDATA_REG
    #define soundUART_RXADDRESS1             soundUART_RXADDRESS1_REG
    #define soundUART_RXADDRESS2             soundUART_RXADDRESS2_REG
    #define soundUART_RXBITCTR_PERIOD        soundUART_RXBITCTR_PERIOD_REG
    #define soundUART_RXBITCTR_CONTROL       soundUART_RXBITCTR_CONTROL_REG
    #define soundUART_RXBITCTR_COUNTER       soundUART_RXBITCTR_COUNTER_REG
    #define soundUART_RXSTATUS               soundUART_RXSTATUS_REG
    #define soundUART_RXSTATUS_MASK          soundUART_RXSTATUS_MASK_REG
    #define soundUART_RXSTATUS_ACTL          soundUART_RXSTATUS_ACTL_REG
#endif /* End  (soundUART_RX_ENABLED) || (soundUART_HD_ENABLED) */

#if(soundUART_INTERNAL_CLOCK_USED)
    #define soundUART_INTCLOCK_CLKEN         soundUART_INTCLOCK_CLKEN_REG
#endif /* End soundUART_INTERNAL_CLOCK_USED */

#define soundUART_WAIT_FOR_COMLETE_REINIT    soundUART_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_soundUART_H */


/* [] END OF FILE */
