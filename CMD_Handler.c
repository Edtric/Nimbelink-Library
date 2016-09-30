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

#include "CMD_Handler.h"
#include "herkulex2.h"

char RECVD_CMD[4][50];

void ServoCMD( void );

void CMD_SEND( char * cmd_string )
{
    char * tkn;
    uint8 cnt = 1;
    
    memset( RECVD_CMD , 0 , sizeof( RECVD_CMD ) );
    CyDelay(5);
    
    tkn = strtok( cmd_string , "_" );
    strcpy( RECVD_CMD[0] , tkn );
    
    while( tkn != NULL )
    {
        tkn = strtok( NULL , "_" );
        strcpy( RECVD_CMD[cnt++] , tkn );
    }
    
    if( strstr( RECVD_CMD[0] , "SRV" ) ) ServoCMD();
    else return;
    
}

void ServoCMD( void )
{
    int ID = atoi( RECVD_CMD[2] );
    
    if( strstr( RECVD_CMD[1] , "MOVEONEPOS" ) )
    {
        int pos_num = atoi( RECVD_CMD[3] );
        Servo_moveOne( (uint8)ID , pos_num , 500 , LED_BLUE );
    }
    if( strstr( RECVD_CMD[1] , "MOVEONEANG" ) )
    {
        double ang_num = atof( RECVD_CMD[3] );
        Servo_moveOneAngle( (uint8)ID , (float)ang_num , 500 , LED_BLUE );
    }
    if( strstr( RECVD_CMD[1] , "MOVEONESPD" ) )
    {
        int spd_num = atoi( RECVD_CMD[3] );
        Servo_moveSpeedOne( (uint8)ID , spd_num , 500 , LED_BLUE );
    }
}

/* [] END OF FILE */
