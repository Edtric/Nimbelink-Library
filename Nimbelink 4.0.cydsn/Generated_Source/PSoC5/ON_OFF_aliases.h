/*******************************************************************************
* File Name: ON_OFF.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ON_OFF_ALIASES_H) /* Pins ON_OFF_ALIASES_H */
#define CY_PINS_ON_OFF_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define ON_OFF_0			(ON_OFF__0__PC)
#define ON_OFF_0_INTR	((uint16)((uint16)0x0001u << ON_OFF__0__SHIFT))

#define ON_OFF_INTR_ALL	 ((uint16)(ON_OFF_0_INTR))

#endif /* End Pins ON_OFF_ALIASES_H */


/* [] END OF FILE */
