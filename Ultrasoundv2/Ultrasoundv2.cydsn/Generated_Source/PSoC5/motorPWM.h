/*******************************************************************************
* File Name: motorPWM.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_motorPWM_H)
#define CY_PWM_motorPWM_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motorPWM_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motorPWM_Resolution                     (8u)
#define motorPWM_UsingFixedFunction             (0u)
#define motorPWM_DeadBandMode                   (0u)
#define motorPWM_KillModeMinTime                (0u)
#define motorPWM_KillMode                       (0u)
#define motorPWM_PWMMode                        (1u)
#define motorPWM_PWMModeIsCenterAligned         (0u)
#define motorPWM_DeadBandUsed                   (0u)
#define motorPWM_DeadBand2_4                    (0u)

#if !defined(motorPWM_PWMUDB_genblk8_stsreg__REMOVED)
    #define motorPWM_UseStatus                  (1u)
#else
    #define motorPWM_UseStatus                  (0u)
#endif /* !defined(motorPWM_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motorPWM_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motorPWM_UseControl                 (1u)
#else
    #define motorPWM_UseControl                 (0u)
#endif /* !defined(motorPWM_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motorPWM_UseOneCompareMode              (0u)
#define motorPWM_MinimumKillTime                (1u)
#define motorPWM_EnableMode                     (0u)

#define motorPWM_CompareMode1SW                 (0u)
#define motorPWM_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motorPWM__B_PWM__DISABLED 0
#define motorPWM__B_PWM__ASYNCHRONOUS 1
#define motorPWM__B_PWM__SINGLECYCLE 2
#define motorPWM__B_PWM__LATCHED 3
#define motorPWM__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motorPWM__B_PWM__DBMDISABLED 0
#define motorPWM__B_PWM__DBM_2_4_CLOCKS 1
#define motorPWM__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motorPWM__B_PWM__ONE_OUTPUT 0
#define motorPWM__B_PWM__TWO_OUTPUTS 1
#define motorPWM__B_PWM__DUAL_EDGE 2
#define motorPWM__B_PWM__CENTER_ALIGN 3
#define motorPWM__B_PWM__DITHER 5
#define motorPWM__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motorPWM__B_PWM__LESS_THAN 1
#define motorPWM__B_PWM__LESS_THAN_OR_EQUAL 2
#define motorPWM__B_PWM__GREATER_THAN 3
#define motorPWM__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motorPWM__B_PWM__EQUAL 0
#define motorPWM__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motorPWM_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!motorPWM_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!motorPWM_PWMModeIsCenterAligned) */
        #if (motorPWM_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motorPWM_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_256_CLOCKS || \
            motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motorPWM_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motorPWM_KillModeMinTime) */

        /* Backup control register */
        #if(motorPWM_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motorPWM_UseControl) */

    #endif /* (!motorPWM_UsingFixedFunction) */

}motorPWM_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motorPWM_Start(void) ;
void    motorPWM_Stop(void) ;

#if (motorPWM_UseStatus || motorPWM_UsingFixedFunction)
    void  motorPWM_SetInterruptMode(uint8 interruptMode) ;
    uint8 motorPWM_ReadStatusRegister(void) ;
#endif /* (motorPWM_UseStatus || motorPWM_UsingFixedFunction) */

#define motorPWM_GetInterruptSource() motorPWM_ReadStatusRegister()

#if (motorPWM_UseControl)
    uint8 motorPWM_ReadControlRegister(void) ;
    void  motorPWM_WriteControlRegister(uint8 control)
          ;
#endif /* (motorPWM_UseControl) */

#if (motorPWM_UseOneCompareMode)
   #if (motorPWM_CompareMode1SW)
       void    motorPWM_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motorPWM_CompareMode1SW) */
#else
    #if (motorPWM_CompareMode1SW)
        void    motorPWM_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motorPWM_CompareMode1SW) */
    #if (motorPWM_CompareMode2SW)
        void    motorPWM_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motorPWM_CompareMode2SW) */
#endif /* (motorPWM_UseOneCompareMode) */

#if (!motorPWM_UsingFixedFunction)
    uint8   motorPWM_ReadCounter(void) ;
    uint8 motorPWM_ReadCapture(void) ;

    #if (motorPWM_UseStatus)
            void motorPWM_ClearFIFO(void) ;
    #endif /* (motorPWM_UseStatus) */

    void    motorPWM_WriteCounter(uint8 counter)
            ;
#endif /* (!motorPWM_UsingFixedFunction) */

void    motorPWM_WritePeriod(uint8 period)
        ;
uint8 motorPWM_ReadPeriod(void) ;

#if (motorPWM_UseOneCompareMode)
    void    motorPWM_WriteCompare(uint8 compare)
            ;
    uint8 motorPWM_ReadCompare(void) ;
#else
    void    motorPWM_WriteCompare1(uint8 compare)
            ;
    uint8 motorPWM_ReadCompare1(void) ;
    void    motorPWM_WriteCompare2(uint8 compare)
            ;
    uint8 motorPWM_ReadCompare2(void) ;
#endif /* (motorPWM_UseOneCompareMode) */


#if (motorPWM_DeadBandUsed)
    void    motorPWM_WriteDeadTime(uint8 deadtime) ;
    uint8   motorPWM_ReadDeadTime(void) ;
#endif /* (motorPWM_DeadBandUsed) */

#if ( motorPWM_KillModeMinTime)
    void motorPWM_WriteKillTime(uint8 killtime) ;
    uint8 motorPWM_ReadKillTime(void) ;
#endif /* ( motorPWM_KillModeMinTime) */

void motorPWM_Init(void) ;
void motorPWM_Enable(void) ;
void motorPWM_Sleep(void) ;
void motorPWM_Wakeup(void) ;
void motorPWM_SaveConfig(void) ;
void motorPWM_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motorPWM_INIT_PERIOD_VALUE          (100u)
#define motorPWM_INIT_COMPARE_VALUE1        (100u)
#define motorPWM_INIT_COMPARE_VALUE2        (100u)
#define motorPWM_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motorPWM_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorPWM_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorPWM_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorPWM_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motorPWM_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motorPWM_CTRL_CMPMODE2_SHIFT)
#define motorPWM_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motorPWM_CTRL_CMPMODE1_SHIFT)
#define motorPWM_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (motorPWM_UsingFixedFunction)
   #define motorPWM_PERIOD_LSB              (*(reg16 *) motorPWM_PWMHW__PER0)
   #define motorPWM_PERIOD_LSB_PTR          ( (reg16 *) motorPWM_PWMHW__PER0)
   #define motorPWM_COMPARE1_LSB            (*(reg16 *) motorPWM_PWMHW__CNT_CMP0)
   #define motorPWM_COMPARE1_LSB_PTR        ( (reg16 *) motorPWM_PWMHW__CNT_CMP0)
   #define motorPWM_COMPARE2_LSB            (0x00u)
   #define motorPWM_COMPARE2_LSB_PTR        (0x00u)
   #define motorPWM_COUNTER_LSB             (*(reg16 *) motorPWM_PWMHW__CNT_CMP0)
   #define motorPWM_COUNTER_LSB_PTR         ( (reg16 *) motorPWM_PWMHW__CNT_CMP0)
   #define motorPWM_CAPTURE_LSB             (*(reg16 *) motorPWM_PWMHW__CAP0)
   #define motorPWM_CAPTURE_LSB_PTR         ( (reg16 *) motorPWM_PWMHW__CAP0)
   #define motorPWM_RT1                     (*(reg8 *)  motorPWM_PWMHW__RT1)
   #define motorPWM_RT1_PTR                 ( (reg8 *)  motorPWM_PWMHW__RT1)

#else
   #if (motorPWM_Resolution == 8u) /* 8bit - PWM */

       #if(motorPWM_PWMModeIsCenterAligned)
           #define motorPWM_PERIOD_LSB      (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define motorPWM_PERIOD_LSB_PTR  ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define motorPWM_PERIOD_LSB      (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define motorPWM_PERIOD_LSB_PTR  ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (motorPWM_PWMModeIsCenterAligned) */

       #define motorPWM_COMPARE1_LSB        (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorPWM_COMPARE1_LSB_PTR    ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorPWM_COMPARE2_LSB        (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorPWM_COMPARE2_LSB_PTR    ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorPWM_COUNTERCAP_LSB      (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorPWM_COUNTERCAP_LSB_PTR  ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorPWM_COUNTER_LSB         (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorPWM_COUNTER_LSB_PTR     ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorPWM_CAPTURE_LSB         (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define motorPWM_CAPTURE_LSB_PTR     ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motorPWM_PWMModeIsCenterAligned)
               #define motorPWM_PERIOD_LSB      (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define motorPWM_PERIOD_LSB_PTR  ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define motorPWM_PERIOD_LSB      (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define motorPWM_PERIOD_LSB_PTR  ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (motorPWM_PWMModeIsCenterAligned) */

            #define motorPWM_COMPARE1_LSB       (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorPWM_COMPARE1_LSB_PTR   ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorPWM_COMPARE2_LSB       (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorPWM_COMPARE2_LSB_PTR   ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorPWM_COUNTERCAP_LSB     (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorPWM_COUNTERCAP_LSB_PTR ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorPWM_COUNTER_LSB        (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorPWM_COUNTER_LSB_PTR    ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorPWM_CAPTURE_LSB        (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define motorPWM_CAPTURE_LSB_PTR    ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(motorPWM_PWMModeIsCenterAligned)
               #define motorPWM_PERIOD_LSB      (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define motorPWM_PERIOD_LSB_PTR  ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motorPWM_PERIOD_LSB      (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define motorPWM_PERIOD_LSB_PTR  ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motorPWM_PWMModeIsCenterAligned) */

            #define motorPWM_COMPARE1_LSB       (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorPWM_COMPARE1_LSB_PTR   ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorPWM_COMPARE2_LSB       (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorPWM_COMPARE2_LSB_PTR   ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorPWM_COUNTERCAP_LSB     (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorPWM_COUNTERCAP_LSB_PTR ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorPWM_COUNTER_LSB        (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorPWM_COUNTER_LSB_PTR    ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorPWM_CAPTURE_LSB        (*(reg16 *) motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define motorPWM_CAPTURE_LSB_PTR    ((reg16 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motorPWM_AUX_CONTROLDP1          (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define motorPWM_AUX_CONTROLDP1_PTR      ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motorPWM_Resolution == 8) */

   #define motorPWM_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define motorPWM_AUX_CONTROLDP0          (*(reg8 *)  motorPWM_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define motorPWM_AUX_CONTROLDP0_PTR      ((reg8 *)   motorPWM_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motorPWM_UsingFixedFunction) */

#if(motorPWM_KillModeMinTime )
    #define motorPWM_KILLMODEMINTIME        (*(reg8 *)  motorPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motorPWM_KILLMODEMINTIME_PTR    ((reg8 *)   motorPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motorPWM_KillModeMinTime ) */

#if(motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_256_CLOCKS)
    #define motorPWM_DEADBAND_COUNT         (*(reg8 *)  motorPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorPWM_DEADBAND_COUNT_PTR     ((reg8 *)   motorPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorPWM_DEADBAND_LSB_PTR       ((reg8 *)   motorPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motorPWM_DEADBAND_LSB           (*(reg8 *)  motorPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motorPWM_UsingFixedFunction)
        #define motorPWM_DEADBAND_COUNT         (*(reg8 *)  motorPWM_PWMHW__CFG0)
        #define motorPWM_DEADBAND_COUNT_PTR     ((reg8 *)   motorPWM_PWMHW__CFG0)
        #define motorPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motorPWM_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motorPWM_DEADBAND_COUNT         (*(reg8 *)  motorPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorPWM_DEADBAND_COUNT_PTR     ((reg8 *)   motorPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motorPWM_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motorPWM_UsingFixedFunction) */
#endif /* (motorPWM_DeadBandMode == motorPWM__B_PWM__DBM_256_CLOCKS) */



#if (motorPWM_UsingFixedFunction)
    #define motorPWM_STATUS                 (*(reg8 *) motorPWM_PWMHW__SR0)
    #define motorPWM_STATUS_PTR             ((reg8 *) motorPWM_PWMHW__SR0)
    #define motorPWM_STATUS_MASK            (*(reg8 *) motorPWM_PWMHW__SR0)
    #define motorPWM_STATUS_MASK_PTR        ((reg8 *) motorPWM_PWMHW__SR0)
    #define motorPWM_CONTROL                (*(reg8 *) motorPWM_PWMHW__CFG0)
    #define motorPWM_CONTROL_PTR            ((reg8 *) motorPWM_PWMHW__CFG0)
    #define motorPWM_CONTROL2               (*(reg8 *) motorPWM_PWMHW__CFG1)
    #define motorPWM_CONTROL3               (*(reg8 *) motorPWM_PWMHW__CFG2)
    #define motorPWM_GLOBAL_ENABLE          (*(reg8 *) motorPWM_PWMHW__PM_ACT_CFG)
    #define motorPWM_GLOBAL_ENABLE_PTR      ( (reg8 *) motorPWM_PWMHW__PM_ACT_CFG)
    #define motorPWM_GLOBAL_STBY_ENABLE     (*(reg8 *) motorPWM_PWMHW__PM_STBY_CFG)
    #define motorPWM_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motorPWM_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motorPWM_BLOCK_EN_MASK          (motorPWM_PWMHW__PM_ACT_MSK)
    #define motorPWM_BLOCK_STBY_EN_MASK     (motorPWM_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motorPWM_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motorPWM_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motorPWM_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motorPWM_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motorPWM_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motorPWM_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motorPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << motorPWM_CTRL_ENABLE_SHIFT)
    #define motorPWM_CTRL_RESET             (uint8)((uint8)0x01u << motorPWM_CTRL_RESET_SHIFT)
    #define motorPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorPWM_CTRL_CMPMODE2_SHIFT)
    #define motorPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorPWM_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motorPWM_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motorPWM_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motorPWM_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motorPWM_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motorPWM_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motorPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motorPWM_STATUS_TC_INT_EN_MASK_SHIFT            (motorPWM_STATUS_TC_SHIFT - 4u)
    #define motorPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motorPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorPWM_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motorPWM_STATUS_TC              (uint8)((uint8)0x01u << motorPWM_STATUS_TC_SHIFT)
    #define motorPWM_STATUS_CMP1            (uint8)((uint8)0x01u << motorPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motorPWM_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motorPWM_STATUS_TC >> 4u)
    #define motorPWM_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motorPWM_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motorPWM_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motorPWM_RT1_MASK              (uint8)((uint8)0x03u << motorPWM_RT1_SHIFT)
    #define motorPWM_SYNC                  (uint8)((uint8)0x03u << motorPWM_RT1_SHIFT)
    #define motorPWM_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motorPWM_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motorPWM_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motorPWM_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motorPWM_SYNCDSI_SHIFT)


#else
    #define motorPWM_STATUS                (*(reg8 *)   motorPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorPWM_STATUS_PTR            ((reg8 *)    motorPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorPWM_STATUS_MASK           (*(reg8 *)   motorPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorPWM_STATUS_MASK_PTR       ((reg8 *)    motorPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorPWM_STATUS_AUX_CTRL       (*(reg8 *)   motorPWM_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motorPWM_CONTROL               (*(reg8 *)   motorPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motorPWM_CONTROL_PTR           ((reg8 *)    motorPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motorPWM_CTRL_ENABLE_SHIFT      (0x07u)
    #define motorPWM_CTRL_RESET_SHIFT       (0x06u)
    #define motorPWM_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motorPWM_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motorPWM_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motorPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << motorPWM_CTRL_ENABLE_SHIFT)
    #define motorPWM_CTRL_RESET             (uint8)((uint8)0x01u << motorPWM_CTRL_RESET_SHIFT)
    #define motorPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorPWM_CTRL_CMPMODE2_SHIFT)
    #define motorPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorPWM_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motorPWM_STATUS_KILL_SHIFT          (0x05u)
    #define motorPWM_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motorPWM_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motorPWM_STATUS_TC_SHIFT            (0x02u)
    #define motorPWM_STATUS_CMP2_SHIFT          (0x01u)
    #define motorPWM_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motorPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (motorPWM_STATUS_KILL_SHIFT)
    #define motorPWM_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motorPWM_STATUS_FIFONEMPTY_SHIFT)
    #define motorPWM_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motorPWM_STATUS_FIFOFULL_SHIFT)
    #define motorPWM_STATUS_TC_INT_EN_MASK_SHIFT            (motorPWM_STATUS_TC_SHIFT)
    #define motorPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (motorPWM_STATUS_CMP2_SHIFT)
    #define motorPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorPWM_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motorPWM_STATUS_KILL            (uint8)((uint8)0x00u << motorPWM_STATUS_KILL_SHIFT )
    #define motorPWM_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motorPWM_STATUS_FIFOFULL_SHIFT)
    #define motorPWM_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motorPWM_STATUS_FIFONEMPTY_SHIFT)
    #define motorPWM_STATUS_TC              (uint8)((uint8)0x01u << motorPWM_STATUS_TC_SHIFT)
    #define motorPWM_STATUS_CMP2            (uint8)((uint8)0x01u << motorPWM_STATUS_CMP2_SHIFT)
    #define motorPWM_STATUS_CMP1            (uint8)((uint8)0x01u << motorPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motorPWM_STATUS_KILL_INT_EN_MASK            (motorPWM_STATUS_KILL)
    #define motorPWM_STATUS_FIFOFULL_INT_EN_MASK        (motorPWM_STATUS_FIFOFULL)
    #define motorPWM_STATUS_FIFONEMPTY_INT_EN_MASK      (motorPWM_STATUS_FIFONEMPTY)
    #define motorPWM_STATUS_TC_INT_EN_MASK              (motorPWM_STATUS_TC)
    #define motorPWM_STATUS_CMP2_INT_EN_MASK            (motorPWM_STATUS_CMP2)
    #define motorPWM_STATUS_CMP1_INT_EN_MASK            (motorPWM_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motorPWM_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motorPWM_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motorPWM_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motorPWM_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motorPWM_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motorPWM_UsingFixedFunction */

#endif  /* CY_PWM_motorPWM_H */


/* [] END OF FILE */
