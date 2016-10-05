/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef NIMBELINK_H
#define NIMBELINK_H
    
    #include <project.h>
    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>
    #include "CMD_Handler.h"
    
    #define DATATXRX    150
    #define P2P         0x00
    #define SMS         0x01

    #define NET_REG_REPORT          "AT+CREG?"
    #define BAUD_RATE_Q             "AT+IPR?"
    #define SIGNAL_QUALITY          "AT+CSQ"
    #define SERVICE_STATE           "AT#SVCSTAT?"
    #define DEVICE_SHUTDOWN         "AT#SHDN"
    
    #define SCKT_CONFIG             "AT#SCFG="
    #define SCKT_CONFIG_Q           "AT#SCFG?"
    #define SCKT_STATUS             "AT#SS"
    #define SCKT_SHUTDOWN           "AT#SH="
    #define CONTEXT_ACTIVATE        "AT#SGACT="
    #define CONTEXT_ACTIVATE_Q      "AT#SGACT?"
    #define SCKT_CONNECT            "AT#SD="
    #define SCKT_RESTORE            "AT#SO=?"
    #define SKIP_ESC_SEQUENCE       "AT#SKIPESC="
    #define SKIP_ESC_SEQUENCE_Q     "AT#SKIPESC?"
    #define SCKT_SEND               "AT#SSEND="
    #define SCKT_RECV               "AT#SRECV="
    
    #define MESSAGE_FORMAT_Q        "AT+CMGF?"
    #define MESSAGE_FORMAT          "AT+CMGF="
    #define SEND_TEXT_MESSAGE       "AT+CMGS="
    #define LIST_TEXT_MESSAGE       "AT+CMGL="
    #define DEL_TEXT_MESSAGE        "AT+CMGD="
    
    void Nimbelink_Start( void );
    void Nimbelink_Stop( void );
    void Nimbelink_InitCheck( void );
    void Nimbelink_ConnectGS( char * port , char * IP );
    void Nimbelink_SendText( char * number , char * message );
    void Nimbelink_GSDataXfer( uint8 sckt , char * data );
    void Nimbelink_AddData( const uint8 param , int value );
    void Nimbelink_SendData( void );
    void Nimbelink_RecvData( void );
    
    uint8 radioSwitch;    
    
    typedef struct
    {
        char * ipAddress;
        char * portNum;
    }hostData;
    
#endif

/* [] END OF FILE */
