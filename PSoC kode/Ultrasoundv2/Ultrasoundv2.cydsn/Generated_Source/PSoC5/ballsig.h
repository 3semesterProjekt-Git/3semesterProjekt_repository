/*******************************************************************************
* File Name: ballsig.h  
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

#if !defined(CY_PINS_ballsig_H) /* Pins ballsig_H */
#define CY_PINS_ballsig_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ballsig_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ballsig__PORT == 15 && ((ballsig__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ballsig_Write(uint8 value);
void    ballsig_SetDriveMode(uint8 mode);
uint8   ballsig_ReadDataReg(void);
uint8   ballsig_Read(void);
void    ballsig_SetInterruptMode(uint16 position, uint16 mode);
uint8   ballsig_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ballsig_SetDriveMode() function.
     *  @{
     */
        #define ballsig_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ballsig_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ballsig_DM_RES_UP          PIN_DM_RES_UP
        #define ballsig_DM_RES_DWN         PIN_DM_RES_DWN
        #define ballsig_DM_OD_LO           PIN_DM_OD_LO
        #define ballsig_DM_OD_HI           PIN_DM_OD_HI
        #define ballsig_DM_STRONG          PIN_DM_STRONG
        #define ballsig_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ballsig_MASK               ballsig__MASK
#define ballsig_SHIFT              ballsig__SHIFT
#define ballsig_WIDTH              1u

/* Interrupt constants */
#if defined(ballsig__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ballsig_SetInterruptMode() function.
     *  @{
     */
        #define ballsig_INTR_NONE      (uint16)(0x0000u)
        #define ballsig_INTR_RISING    (uint16)(0x0001u)
        #define ballsig_INTR_FALLING   (uint16)(0x0002u)
        #define ballsig_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ballsig_INTR_MASK      (0x01u) 
#endif /* (ballsig__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ballsig_PS                     (* (reg8 *) ballsig__PS)
/* Data Register */
#define ballsig_DR                     (* (reg8 *) ballsig__DR)
/* Port Number */
#define ballsig_PRT_NUM                (* (reg8 *) ballsig__PRT) 
/* Connect to Analog Globals */                                                  
#define ballsig_AG                     (* (reg8 *) ballsig__AG)                       
/* Analog MUX bux enable */
#define ballsig_AMUX                   (* (reg8 *) ballsig__AMUX) 
/* Bidirectional Enable */                                                        
#define ballsig_BIE                    (* (reg8 *) ballsig__BIE)
/* Bit-mask for Aliased Register Access */
#define ballsig_BIT_MASK               (* (reg8 *) ballsig__BIT_MASK)
/* Bypass Enable */
#define ballsig_BYP                    (* (reg8 *) ballsig__BYP)
/* Port wide control signals */                                                   
#define ballsig_CTL                    (* (reg8 *) ballsig__CTL)
/* Drive Modes */
#define ballsig_DM0                    (* (reg8 *) ballsig__DM0) 
#define ballsig_DM1                    (* (reg8 *) ballsig__DM1)
#define ballsig_DM2                    (* (reg8 *) ballsig__DM2) 
/* Input Buffer Disable Override */
#define ballsig_INP_DIS                (* (reg8 *) ballsig__INP_DIS)
/* LCD Common or Segment Drive */
#define ballsig_LCD_COM_SEG            (* (reg8 *) ballsig__LCD_COM_SEG)
/* Enable Segment LCD */
#define ballsig_LCD_EN                 (* (reg8 *) ballsig__LCD_EN)
/* Slew Rate Control */
#define ballsig_SLW                    (* (reg8 *) ballsig__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ballsig_PRTDSI__CAPS_SEL       (* (reg8 *) ballsig__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ballsig_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ballsig__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ballsig_PRTDSI__OE_SEL0        (* (reg8 *) ballsig__PRTDSI__OE_SEL0) 
#define ballsig_PRTDSI__OE_SEL1        (* (reg8 *) ballsig__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ballsig_PRTDSI__OUT_SEL0       (* (reg8 *) ballsig__PRTDSI__OUT_SEL0) 
#define ballsig_PRTDSI__OUT_SEL1       (* (reg8 *) ballsig__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ballsig_PRTDSI__SYNC_OUT       (* (reg8 *) ballsig__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ballsig__SIO_CFG)
    #define ballsig_SIO_HYST_EN        (* (reg8 *) ballsig__SIO_HYST_EN)
    #define ballsig_SIO_REG_HIFREQ     (* (reg8 *) ballsig__SIO_REG_HIFREQ)
    #define ballsig_SIO_CFG            (* (reg8 *) ballsig__SIO_CFG)
    #define ballsig_SIO_DIFF           (* (reg8 *) ballsig__SIO_DIFF)
#endif /* (ballsig__SIO_CFG) */

/* Interrupt Registers */
#if defined(ballsig__INTSTAT)
    #define ballsig_INTSTAT            (* (reg8 *) ballsig__INTSTAT)
    #define ballsig_SNAP               (* (reg8 *) ballsig__SNAP)
    
	#define ballsig_0_INTTYPE_REG 		(* (reg8 *) ballsig__0__INTTYPE)
#endif /* (ballsig__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ballsig_H */


/* [] END OF FILE */
