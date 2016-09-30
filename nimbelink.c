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

#include "nimbelink.h"

char cmd[12];
char param[50];

char msgUART[DATATXRX];
char recvdMsg[DATATXRX];

char msgParse[6][60];
char infoParse[6][20];
char * nltoken;
char * spctoken;
char * cmatoken;
char reply_num[13];
char sckt[2];

uint8 counter;
uint8 recByte;

hostData GS;

CY_ISR( RX_INT_Handler )
{
    RX_ISR_ClearPending();
    
    char recv;
    recv = UART_NL_GetChar();
    
    if( recv != '$' ) return;
}

/*
    Searches for string StrToFind in the string MesToSearch
    using the string.h function strstr(). If the substring 
    is found, the returned pointer to the match is compared 
    to the original substring using the function strcmp(). 
    The substring function will then return a 1 if the 
    compared value is greater than or equal to 0 and 0 if 
    the value is less than 1.
*/
uint8 substring( const char * MesToSearch , const char * StrToFind )
{
    char * result;
    result = strstr(MesToSearch,StrToFind);
    
    if( result == NULL ) return 0;
    else return 1;
}

/*
    Takes the message that is stored in recvdMsg and parses 
    it into separate strings, stored in the array of strings 
    msgParse. It parses the message by using the string.h 
    function strtok(). It separates the message by each new 
    line that is present and ignores new lines that only have 
    a carriage return present.
*/
void msg_parse( void )
{
    memset( msgParse , 0 , sizeof( msgParse ) );
    CyDelay(5);
    
    counter = 1;
    nltoken = strtok( recvdMsg , "\r\n" );
    strcpy( msgParse[0] , nltoken );
    
    while( nltoken != NULL )
    {
        nltoken = strtok( NULL , "\r\n" );
        strcpy( msgParse[counter++] , nltoken );
    }
}

/*
    Takes the array of strings that was previously filled 
    by msg_parse and further divides the strings by spaces 
    and by carriage returns. For now, this function is meant 
    to extract the SRING data coming from the modem, e.g. 
    "SRING: 1,17" where "1,17" would be the relavant data.
*/
void data_parse( uint8 type )
{
    memset( infoParse , 0 , sizeof( infoParse ) );
    CyDelay(5);

    if( type == P2P )
    {
        spctoken = strtok( msgParse[0] , " " );
        spctoken = strtok( NULL , " " );
        strcpy( infoParse[0] , spctoken );
        
        cmatoken = strtok( infoParse[0] , "," );
        for( counter = 1 ; counter < 6 ; counter++ )
        {
            strcpy( infoParse[counter] , cmatoken );
            cmatoken = strtok( NULL , "," );
        }
    }
    
    if( type == SMS )
    {
        cmatoken = strtok( msgParse[0] , "," );
        for( counter = 0 ; counter < 6 ; counter++ )
        {
            strcpy( infoParse[counter] , cmatoken );
            cmatoken = strtok( NULL , "," );
        }
    }
}

inline void clearCMD( void )
{
    memset( cmd , 0 , sizeof( cmd ) ); 
    CyDelay(10);
}

inline void clearPARAM( void )
{
    memset( param , 0 , sizeof( param ) ); 
    CyDelay(10);
}

void writeCMD( char * command )
{
    uint8 cmd_len = strlen(cmd);
    
    if( cmd_len > 0 ) clearCMD();
    
    strcpy( cmd , command );
}

void writePARAM( char * _param )
{
    uint8 data_len = strlen(param);
    
    if( data_len > 0 ) clearPARAM();
    
    strcpy( param , _param );
}

inline void addPARAM ( char * _param_add )
{
    strcat( param , _param_add );
}

/*
    Transmits data to the modem by first assembling the 
    command and data into a packet called msgUART and then 
    sending it through the UART TX line. The command is 
    separate from the data, and depending on whether or not 
    both must be sent at the same time, the packet will be 
    constructed in the appropriate manner.
*/
void NL_TX( uint8 headCmdPSNT , uint8 paramPSNT )
{
    UART_NL_ClearTxBuffer();
    UART_NL_ClearRxBuffer();
    UART_DEBUG_ClearTxBuffer();
    
    memset( msgUART , 0 , sizeof( msgUART ) );
    CyDelay(10);
    memset( recvdMsg , 0 , sizeof( recvdMsg ) );
    CyDelay(10);    

    if( headCmdPSNT )
    {
        strcpy( msgUART , cmd );
        
        if( paramPSNT ) strcat( msgUART , param );
        
        UART_NL_PutString( msgUART );
        UART_NL_PutString( "\r" );
    } 
    else if( paramPSNT )
    {
        strcpy( msgUART , param );
        
        UART_NL_PutString( msgUART );
        UART_DEBUG_PutString( msgUART );
        UART_DEBUG_PutString( "\r\n" );
        UART_NL_PutChar( 0x1A );
    }    
}

/*
    Receives the data present in the UART RX buffer. As long 
    as there is data in the buffer, it will store each byte 
    of data in recvdMsg until the buffer is empty.
*/
void NL_RX( void )
{
    counter = 0;
    
    while( UART_NL_GetRxBufferSize() > 0 )
    {
        recByte = UART_NL_GetChar();
        UART_DEBUG_PutChar(recByte);
        
        recvdMsg[counter++] = recByte;
    }
}

/*
    Clears any strings stored in cmd and data to allow new 
    commands and data to be stored.
*/
void msg_clear( void )
{
    memset( cmd , 0 , sizeof( cmd ) );
    memset( param , 0 , sizeof( param ) ); 
    CyDelay(10);
}

/*
    Sends data through the modem network socket. There are 
    6 sockets to send data through, but for now there is only 
    one in use.
*/
void NL_scktsend( uint8 socket , char * _scktdata )
{
    sprintf( sckt , "%d" , socket );
    writeCMD( SCKT_SEND );
    writePARAM( sckt );
    NL_TX(1,1);
    CyDelay(50);
    NL_RX();
//    msg_clear();
    
    writePARAM( _scktdata );
    NL_TX(0,1);
    CyDelay(50);
    NL_RX();
//    msg_clear();
}

/*
    Receives data through the modem network socket. The function 
    waits until bytes are received in the RX buffer to start 
    reading and recording the data.
*/
void NL_scktrecv( uint8 socket )
{
    sprintf( sckt , "%d" , socket );
    writeCMD( "AT#SI=" );
    writePARAM( sckt );
    NL_TX(1,1);
    CyDelay(50);
    NL_RX();
    msg_parse();
    data_parse(P2P);
    
    if( !substring( infoParse[4] , "0" ) )
    {
        writeCMD( SCKT_RECV );
        writePARAM( sckt );
        addPARAM( "," );
        addPARAM( infoParse[4] );
        NL_TX(1,1);
        CyDelay(50);
        NL_RX();
    }
    
//    while( !(UART_NL_GetRxBufferSize() > 0) );
//    memset( recvdMsg , 0 , sizeof(recvdMsg) );
//    CyDelay(10);
//    NL_RX();
//    msg_parse();
//    data_parse();
//    
//    writeCMD( SCKT_RECV );
//    writePARAM( msgParse[2] );
//    NL_TX(1,1);
//    CyDelay(50);
//    NL_RX();
//    msg_clear();
}

void NL_sendSMS( char * number , char * message )
{
    writeCMD( SEND_TEXT_MESSAGE );
    writePARAM( number );
    NL_TX(1,1);
    CyDelay(50);
    NL_RX();
    
    writePARAM( message );
    NL_TX(0,1);
    CyDelay(50);
    NL_RX();
    UART_NL_ClearRxBuffer();
        
    while( !(UART_NL_GetRxBufferSize() > 0) );        
    memset( recvdMsg , 0 , sizeof(recvdMsg) );
    CyDelay(10);
    NL_RX();
}

void NL_recvSMS( void )
{
    UART_DEBUG_PutString( "Checking for text messages\r\n" );
    writeCMD( LIST_TEXT_MESSAGE );
    writePARAM( "\"REC UNREAD\"" );
    NL_TX(1,1);
    CyDelay(50);
    NL_RX();
    
    msg_parse();
    
    if( !substring( msgParse[0] , "OK" ) ) 
    {
//        CyDelay(10);
        CMD_SEND( msgParse[1] );
        
        data_parse(SMS);
        strcpy( reply_num , infoParse[2] );
        NL_sendSMS( reply_num ,"Text received" );

        writeCMD( DEL_TEXT_MESSAGE );
        writePARAM( "1,4" );
        NL_TX(1,1);
        CyDelay(50);
        NL_RX();
    }
}

void Nimbelink_Start( void )
{
    /*ON_OFF_Write(0);
    CyDelay(1100);
    ON_OFF_Write(1);
    CyDelay(9000);*/
    
    writeCMD( "ATE0" );
    NL_TX(1,0);
    CyDelay(50);
    NL_RX();
//    msg_clear();
    
    writeCMD( NET_REG_REPORT );
    NL_TX(1,0);
    CyDelay(50);
    NL_RX();
    if( substring( recvdMsg , "0,1" ) || substring( recvdMsg , "0,5" ) )
    {
        UART_DEBUG_PutString("Connection Successful!\r\n");
    }
    
    writeCMD( MESSAGE_FORMAT );
    writePARAM( "1" );
    NL_TX(1,1);
    CyDelay(50);
    NL_RX();
//    msg_clear();
}

void Nimbelink_ConnectGS( char * port , char * IP )
{ 
    uint8 count;
    char shutdownMes[30];
    GS.portNum = port;
    GS.ipAddress = IP;
    
    writeCMD( SCKT_STATUS );
    NL_TX(1,0);
    CyDelay(50);
    NL_RX();
    msg_parse();
//    msg_clear();
    
    for( count = 0 ; count < 6 ; count++ )
    {
        if( strstr( msgParse[count] , ",0" ) == NULL )
        {
            sprintf( sckt , "%d" , count + 1 );
            writeCMD( SCKT_SHUTDOWN );
            writePARAM( sckt );
            NL_TX(1,1);
            CyDelay(50);
            NL_RX();
            if( substring( recvdMsg , "OK" ) )
            {
                sprintf( shutdownMes , "\r\nSocket %s has been shut down.\r\n" , sckt );
                UART_DEBUG_PutString(shutdownMes);
            }
//            msg_clear();
        }
    }
    
    writeCMD( CONTEXT_ACTIVATE_Q );
    NL_TX(1,0);
    CyDelay(50);
    NL_RX();
//    msg_clear();
    if( substring( recvdMsg , "1,0" ) )
    {
        writeCMD( CONTEXT_ACTIVATE );
        writePARAM( "1,1" );
        NL_TX(1,1);
        CyDelay(50);
        NL_RX();
//        msg_clear();
        UART_NL_ClearRxBuffer();

        UART_DEBUG_PutString("\r\nActivating context...\r\n");
        
        while( !(UART_NL_GetRxBufferSize() > 0) );        
        memset( recvdMsg , 0 , sizeof(recvdMsg) );
        CyDelay(10);
        NL_RX();
        UART_DEBUG_PutString("\r\nDone!\r\n");
    }
    else if( substring( recvdMsg , "1,1" ) )
    {
        UART_DEBUG_PutString("Context has been activated\r\n");
    }
    CyDelay(1);
    
//    msg_clear();
    
    writeCMD( SCKT_CONNECT );
    writePARAM( "1,0," );
    addPARAM( GS.portNum );
    addPARAM( "," );
    addPARAM( GS.ipAddress);
    addPARAM( ",0,0,1" );
    NL_TX(1,1);
    CyDelay(50);
    NL_RX();
//    msg_clear();
    UART_NL_ClearRxBuffer();
    
    
    UART_DEBUG_PutString("\r\nConnecting to ground station...\r\n");
    
    while ( !(UART_NL_GetRxBufferSize() > 0) );
    memset( recvdMsg , 0 , sizeof(recvdMsg) );
    CyDelay(10);
    NL_RX();
    if( strstr( recvdMsg , "OK" ) )
    {
        UART_DEBUG_PutString("Successfully connected!\r\n");
//        RX_ISR_StartEx( RX_INT_Handler );
    }
    else if( strstr( recvdMsg , "ERROR" ) )
    {
        UART_DEBUG_PutString("Connection error. LAME!!!\r\n");
        while(1);
    }
}

void Nimbelink_GSDataXfer( uint8 sckt , char * data )
{
//    NL_scktrecv(sckt);
    NL_recvSMS();
    NL_scktsend(sckt,data);    
//    CyDelay(250);
    
}

/* [] END OF FILE */
