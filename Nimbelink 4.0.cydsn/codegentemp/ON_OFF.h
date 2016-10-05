/*******************************************************************************
* File Name: ON_OFF.h  
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

#if !defined(CY_PINS_ON_OFF_H) /* Pins ON_OFF_H */
#define CY_PINS_ON_OFF_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ON_OFF_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ON_OFF__PORT == 15 && ((ON_OFF__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ON_OFF_Write(uint8 value);
void    ON_OFF_SetDriveMode(uint8 mode);
uint8   ON_OFF_ReadDataReg(void);
uint8   ON_OFF_Read(void);
void    ON_OFF_SetInterruptMode(uint16 position, uint16 mode);
uint8   ON_OFF_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ON_OFF_SetDriveMode() function.
     *  @{
     */
        #define ON_OFF_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ON_OFF_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ON_OFF_DM_RES_UP          PIN_DM_RES_UP
        #define ON_OFF_DM_RES_DWN         PIN_DM_RES_DWN
        #define ON_OFF_DM_OD_LO           PIN_DM_OD_LO
        #define ON_OFF_DM_OD_HI           PIN_DM_OD_HI
        #define ON_OFF_DM_STRONG          PIN_DM_STRONG
        #define ON_OFF_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ON_OFF_MASK               ON_OFF__MASK
#define ON_OFF_SHIFT              ON_OFF__SHIFT
#define ON_OFF_WIDTH              1u

/* Interrupt constants */
#if defined(ON_OFF__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ON_OFF_SetInterruptMode() function.
     *  @{
     */
        #define ON_OFF_INTR_NONE      (uint16)(0x0000u)
        #define ON_OFF_INTR_RISING    (uint16)(0x0001u)
        #define ON_OFF_INTR_FALLING   (uint16)(0x0002u)
        #define ON_OFF_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ON_OFF_INTR_MASK      (0x01u) 
#endif /* (ON_OFF__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ON_OFF_PS                     (* (reg8 *) ON_OFF__PS)
/* Data Register */
#define ON_OFF_DR                     (* (reg8 *) ON_OFF__DR)
/* Port Number */
#define ON_OFF_PRT_NUM                (* (reg8 *) ON_OFF__PRT) 
/* Connect to Analog Globals */                                                  
#define ON_OFF_AG                     (* (reg8 *) ON_OFF__AG)                       
/* Analog MUX bux enable */
#define ON_OFF_AMUX                   (* (reg8 *) ON_OFF__AMUX) 
/* Bidirectional Enable */                                                        
#define ON_OFF_BIE                    (* (reg8 *) ON_OFF__BIE)
/* Bit-mask for Aliased Register Access */
#define ON_OFF_BIT_MASK               (* (reg8 *) ON_OFF__BIT_MASK)
/* Bypass Enable */
#define ON_OFF_BYP                    (* (reg8 *) ON_OFF__BYP)
/* Port wide control signals */                                                   
#define ON_OFF_CTL                    (* (reg8 *) ON_OFF__CTL)
/* Drive Modes */
#define ON_OFF_DM0                    (* (reg8 *) ON_OFF__DM0) 
#define ON_OFF_DM1                    (* (reg8 *) ON_OFF__DM1)
#define ON_OFF_DM2                    (* (reg8 *) ON_OFF__DM2) 
/* Input Buffer Disable Override */
#define ON_OFF_INP_DIS                (* (reg8 *) ON_OFF__INP_DIS)
/* LCD Common or Segment Drive */
#define ON_OFF_LCD_COM_SEG            (* (reg8 *) ON_OFF__LCD_COM_SEG)
/* Enable Segment LCD */
#define ON_OFF_LCD_EN                 (* (reg8 *) ON_OFF__LCD_EN)
/* Slew Rate Control */
#define ON_OFF_SLW                    (* (reg8 *) ON_OFF__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ON_OFF_PRTDSI__CAPS_SEL       (* (reg8 *) ON_OFF__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ON_OFF_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ON_OFF__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ON_OFF_PRTDSI__OE_SEL0        (* (reg8 *) ON_OFF__PRTDSI__OE_SEL0) 
#define ON_OFF_PRTDSI__OE_SEL1        (* (reg8 *) ON_OFF__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ON_OFF_PRTDSI__OUT_SEL0       (* (reg8 *) ON_OFF__PRTDSI__OUT_SEL0) 
#define ON_OFF_PRTDSI__OUT_SEL1       (* (reg8 *) ON_OFF__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ON_OFF_PRTDSI__SYNC_OUT       (* (reg8 *) ON_OFF__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ON_OFF__SIO_CFG)
    #define ON_OFF_SIO_HYST_EN        (* (reg8 *) ON_OFF__SIO_HYST_EN)
    #define ON_OFF_SIO_REG_HIFREQ     (* (reg8 *) ON_OFF__SIO_REG_HIFREQ)
    #define ON_OFF_SIO_CFG            (* (reg8 *) ON_OFF__SIO_CFG)
    #define ON_OFF_SIO_DIFF           (* (reg8 *) ON_OFF__SIO_DIFF)
#endif /* (ON_OFF__SIO_CFG) */

/* Interrupt Registers */
#if defined(ON_OFF__INTSTAT)
    #define ON_OFF_INTSTAT            (* (reg8 *) ON_OFF__INTSTAT)
    #define ON_OFF_SNAP               (* (reg8 *) ON_OFF__SNAP)
    
	#define ON_OFF_0_INTTYPE_REG 		(* (reg8 *) ON_OFF__0__INTTYPE)
#endif /* (ON_OFF__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ON_OFF_H */


/* [] END OF FILE */
