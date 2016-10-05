/*******************************************************************************
* File Name: UART_NL_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_NL.h"


/***************************************
* Local data allocation
***************************************/

static UART_NL_BACKUP_STRUCT  UART_NL_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UART_NL_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the UART_NL_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_NL_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_NL_SaveConfig(void)
{
    #if(UART_NL_CONTROL_REG_REMOVED == 0u)
        UART_NL_backup.cr = UART_NL_CONTROL_REG;
    #endif /* End UART_NL_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART_NL_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_NL_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling UART_NL_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void UART_NL_RestoreConfig(void)
{
    #if(UART_NL_CONTROL_REG_REMOVED == 0u)
        UART_NL_CONTROL_REG = UART_NL_backup.cr;
    #endif /* End UART_NL_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART_NL_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The UART_NL_Sleep() API saves the current component state. Then it
*  calls the UART_NL_Stop() function and calls 
*  UART_NL_SaveConfig() to save the hardware configuration.
*  Call the UART_NL_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_NL_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_NL_Sleep(void)
{
    #if(UART_NL_RX_ENABLED || UART_NL_HD_ENABLED)
        if((UART_NL_RXSTATUS_ACTL_REG  & UART_NL_INT_ENABLE) != 0u)
        {
            UART_NL_backup.enableState = 1u;
        }
        else
        {
            UART_NL_backup.enableState = 0u;
        }
    #else
        if((UART_NL_TXSTATUS_ACTL_REG  & UART_NL_INT_ENABLE) !=0u)
        {
            UART_NL_backup.enableState = 1u;
        }
        else
        {
            UART_NL_backup.enableState = 0u;
        }
    #endif /* End UART_NL_RX_ENABLED || UART_NL_HD_ENABLED*/

    UART_NL_Stop();
    UART_NL_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_NL_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  UART_NL_Sleep() was called. The UART_NL_Wakeup() function
*  calls the UART_NL_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  UART_NL_Sleep() function was called, the UART_NL_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_NL_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_NL_Wakeup(void)
{
    UART_NL_RestoreConfig();
    #if( (UART_NL_RX_ENABLED) || (UART_NL_HD_ENABLED) )
        UART_NL_ClearRxBuffer();
    #endif /* End (UART_NL_RX_ENABLED) || (UART_NL_HD_ENABLED) */
    #if(UART_NL_TX_ENABLED || UART_NL_HD_ENABLED)
        UART_NL_ClearTxBuffer();
    #endif /* End UART_NL_TX_ENABLED || UART_NL_HD_ENABLED */

    if(UART_NL_backup.enableState != 0u)
    {
        UART_NL_Enable();
    }
}


/* [] END OF FILE */
