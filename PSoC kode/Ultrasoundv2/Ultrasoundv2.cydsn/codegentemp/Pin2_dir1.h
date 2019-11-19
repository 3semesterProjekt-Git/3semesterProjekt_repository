/*******************************************************************************
* File Name: Pin2_dir1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin2_dir1_H) /* Pins Pin2_dir1_H */
#define CY_PINS_Pin2_dir1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin2_dir1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin2_dir1__PORT == 15 && ((Pin2_dir1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pin2_dir1_Write(uint8 value);
void    Pin2_dir1_SetDriveMode(uint8 mode);
uint8   Pin2_dir1_ReadDataReg(void);
uint8   Pin2_dir1_Read(void);
void    Pin2_dir1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pin2_dir1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pin2_dir1_SetDriveMode() function.
     *  @{
     */
        #define Pin2_dir1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pin2_dir1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pin2_dir1_DM_RES_UP          PIN_DM_RES_UP
        #define Pin2_dir1_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pin2_dir1_DM_OD_LO           PIN_DM_OD_LO
        #define Pin2_dir1_DM_OD_HI           PIN_DM_OD_HI
        #define Pin2_dir1_DM_STRONG          PIN_DM_STRONG
        #define Pin2_dir1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pin2_dir1_MASK               Pin2_dir1__MASK
#define Pin2_dir1_SHIFT              Pin2_dir1__SHIFT
#define Pin2_dir1_WIDTH              1u

/* Interrupt constants */
#if defined(Pin2_dir1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pin2_dir1_SetInterruptMode() function.
     *  @{
     */
        #define Pin2_dir1_INTR_NONE      (uint16)(0x0000u)
        #define Pin2_dir1_INTR_RISING    (uint16)(0x0001u)
        #define Pin2_dir1_INTR_FALLING   (uint16)(0x0002u)
        #define Pin2_dir1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pin2_dir1_INTR_MASK      (0x01u) 
#endif /* (Pin2_dir1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin2_dir1_PS                     (* (reg8 *) Pin2_dir1__PS)
/* Data Register */
#define Pin2_dir1_DR                     (* (reg8 *) Pin2_dir1__DR)
/* Port Number */
#define Pin2_dir1_PRT_NUM                (* (reg8 *) Pin2_dir1__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin2_dir1_AG                     (* (reg8 *) Pin2_dir1__AG)                       
/* Analog MUX bux enable */
#define Pin2_dir1_AMUX                   (* (reg8 *) Pin2_dir1__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin2_dir1_BIE                    (* (reg8 *) Pin2_dir1__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin2_dir1_BIT_MASK               (* (reg8 *) Pin2_dir1__BIT_MASK)
/* Bypass Enable */
#define Pin2_dir1_BYP                    (* (reg8 *) Pin2_dir1__BYP)
/* Port wide control signals */                                                   
#define Pin2_dir1_CTL                    (* (reg8 *) Pin2_dir1__CTL)
/* Drive Modes */
#define Pin2_dir1_DM0                    (* (reg8 *) Pin2_dir1__DM0) 
#define Pin2_dir1_DM1                    (* (reg8 *) Pin2_dir1__DM1)
#define Pin2_dir1_DM2                    (* (reg8 *) Pin2_dir1__DM2) 
/* Input Buffer Disable Override */
#define Pin2_dir1_INP_DIS                (* (reg8 *) Pin2_dir1__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin2_dir1_LCD_COM_SEG            (* (reg8 *) Pin2_dir1__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin2_dir1_LCD_EN                 (* (reg8 *) Pin2_dir1__LCD_EN)
/* Slew Rate Control */
#define Pin2_dir1_SLW                    (* (reg8 *) Pin2_dir1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin2_dir1_PRTDSI__CAPS_SEL       (* (reg8 *) Pin2_dir1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin2_dir1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin2_dir1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin2_dir1_PRTDSI__OE_SEL0        (* (reg8 *) Pin2_dir1__PRTDSI__OE_SEL0) 
#define Pin2_dir1_PRTDSI__OE_SEL1        (* (reg8 *) Pin2_dir1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin2_dir1_PRTDSI__OUT_SEL0       (* (reg8 *) Pin2_dir1__PRTDSI__OUT_SEL0) 
#define Pin2_dir1_PRTDSI__OUT_SEL1       (* (reg8 *) Pin2_dir1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin2_dir1_PRTDSI__SYNC_OUT       (* (reg8 *) Pin2_dir1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pin2_dir1__SIO_CFG)
    #define Pin2_dir1_SIO_HYST_EN        (* (reg8 *) Pin2_dir1__SIO_HYST_EN)
    #define Pin2_dir1_SIO_REG_HIFREQ     (* (reg8 *) Pin2_dir1__SIO_REG_HIFREQ)
    #define Pin2_dir1_SIO_CFG            (* (reg8 *) Pin2_dir1__SIO_CFG)
    #define Pin2_dir1_SIO_DIFF           (* (reg8 *) Pin2_dir1__SIO_DIFF)
#endif /* (Pin2_dir1__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pin2_dir1__INTSTAT)
    #define Pin2_dir1_INTSTAT            (* (reg8 *) Pin2_dir1__INTSTAT)
    #define Pin2_dir1_SNAP               (* (reg8 *) Pin2_dir1__SNAP)
    
	#define Pin2_dir1_0_INTTYPE_REG 		(* (reg8 *) Pin2_dir1__0__INTTYPE)
#endif /* (Pin2_dir1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin2_dir1_H */


/* [] END OF FILE */
