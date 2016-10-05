/*******************************************************************************
* File Name: UART_SRV_PM.c
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

#include "UART_SRV.h"


/***************************************
* Local data allocation
***************************************/

static UART_SRV_BACKUP_STRUCT  UART_SRV_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UART_SRV_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the UART_SRV_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_SRV_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_SRV_SaveConfig(void)
{
    #if(UART_SRV_CONTROL_REG_REMOVED == 0u)
        UART_SRV_backup.cr = UART_SRV_CONTROL_REG;
    #endif /* End UART_SRV_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART_SRV_RestoreConfig
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
*  UART_SRV_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling UART_SRV_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void UART_SRV_RestoreConfig(void)
{
    #if(UART_SRV_CONTROL_REG_REMOVED == 0u)
        UART_SRV_CONTROL_REG = UART_SRV_backup.cr;
    #endif /* End UART_SRV_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART_SRV_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The UART_SRV_Sleep() API saves the current component state. Then it
*  calls the UART_SRV_Stop() function and calls 
*  UART_SRV_SaveConfig() to save the hardware configuration.
*  Call the UART_SRV_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_SRV_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_SRV_Sleep(void)
{
    #if(UART_SRV_RX_ENABLED || UART_SRV_HD_ENABLED)
        if((UART_SRV_RXSTATUS_ACTL_REG  & UART_SRV_INT_ENABLE) != 0u)
        {
            UART_SRV_backup.enableState = 1u;
        }
        else
        {
            UART_SRV_backup.enableState = 0u;
        }
    #else
        if((UART_SRV_TXSTATUS_ACTL_REG  & UART_SRV_INT_ENABLE) !=0u)
        {
            UART_SRV_backup.enableState = 1u;
        }
        else
        {
            UART_SRV_backup.enableState = 0u;
        }
    #endif /* End UART_SRV_RX_ENABLED || UART_SRV_HD_ENABLED*/

    UART_SRV_Stop();
    UART_SRV_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_SRV_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  UART_SRV_Sleep() was called. The UART_SRV_Wakeup() function
*  calls the UART_SRV_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  UART_SRV_Sleep() function was called, the UART_SRV_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_SRV_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_SRV_Wakeup(void)
{
    UART_SRV_RestoreConfig();
    #if( (UART_SRV_RX_ENABLED) || (UART_SRV_HD_ENABLED) )
        UART_SRV_ClearRxBuffer();
    #endif /* End (UART_SRV_RX_ENABLED) || (UART_SRV_HD_ENABLED) */
    #if(UART_SRV_TX_ENABLED || UART_SRV_HD_ENABLED)
        UART_SRV_ClearTxBuffer();
    #endif /* End UART_SRV_TX_ENABLED || UART_SRV_HD_ENABLED */

    if(UART_SRV_backup.enableState != 0u)
    {
        UART_SRV_Enable();
    }
}


/* [] END OF FILE */
