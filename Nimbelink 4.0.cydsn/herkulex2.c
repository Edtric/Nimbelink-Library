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

#include "herkulex2.h"

uint8 pSize;
uint8 pID;
uint8 cmd;
uint8 lengthString;
uint8 ck1;
uint8 ck2;

uint8 conta;

uint8 XOR;
int playTime;

uint8 data[DATASIZE];
uint8 dataEx[DATAMOVE+8];
uint8 moveData[DATAMOVE];

void clearBuffer( void )
{
    UART_SRV_ClearTxBuffer();
    UART_SRV_ClearRxBuffer();
}

uint8 checksum1( uint8 * data , uint8 lengthString )
{
    uint8 i;
    
    XOR = 0;
    XOR = XOR ^ pSize;
    XOR = XOR ^ pID;
    XOR = XOR ^ cmd;
    
    for( i = 0; i < lengthString; i++ )
    {
        XOR = XOR ^ data[i];
    }
    
    return XOR & 0xFE;
}

uint8 checksum2( uint8 XOR )
{
    return (~XOR) & 0xFE;
}

/*void printHexByte( uint8 x )
{
    char byte[4];
    
    sprintf( byte , "%x" , x );
    
    UART_UartPutString("0x");
    if( x < 16)
    {
        UART_UartPutChar('0');
    }
    UART_UartPutString(byte);
    UART_UartPutChar(' ');
}*/

void sendData( uint8 * buffer , uint8 length )
{
    clearBuffer();
    UART_SRV_PutArray( buffer , length );
    CyDelay(1);
}

void readData( uint8 size )
{
    uint8 i = 0;
    uint8 beginsave = 0;
    uint8 timeCounter = 0;
    
    while( (UART_SRV_GetRxBufferSize() < size) & (timeCounter < TIMEOUT) )
    {
        timeCounter++;
        CyDelay(1);
    }
    while( (UART_SRV_GetRxBufferSize() > 0) )
    {
        uint8 recByte = UART_SRV_GetByte();
        if( recByte == 0xFF )
        {
            beginsave = 1;
            i = 0;
        }
        if( beginsave == 1 && i < size )
        {
            dataEx[i] = recByte;
            i++;
        }
    }
    clearBuffer();
}

void addData( int goalLSB , int goalMSB , uint8 set , uint8 servoID )
{
    moveData[conta++] = goalLSB;
    moveData[conta++] = goalMSB;
    moveData[conta++] = set;
    moveData[conta++] = servoID;
}

void Servo_Start( void )
{
    conta = 0;
    lengthString = 0;
    CyDelay(100);
    Servo_clearError(PIDALL);
    CyDelay(10);
    Servo_ACK(1);
    CyDelay(10);
    Servo_torqueON(PIDALL);
    CyDelay(10);
    
    Servo_moveOneAngle( PIDALL , 0 , 1000 , LED_RED );
    CyDelay(1200);
    Servo_setLED( PIDALL , LED_GREEN );
    CyDelay(100);
    Servo_setLED( PIDALL , 0 );
    CyDelay(500);
}

uint8 Servo_status( uint8 servoID )
{
    pSize = 0x07;
    pID = servoID;
    cmd = STAT;
    
    ck1 = ( pSize ^ pID ^ cmd) & 0xFE;
    ck2 = ( ~( pSize ^ pID ^ cmd ) ) & 0xFE;
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    
    sendData( dataEx , pSize );
    CyDelay(5);
    readData(9);
    
    pSize = dataEx[2];
    pID = dataEx[3];
    cmd = dataEx[4];
    data[0] = data[7];
    data[1] = data[8];
    lengthString = 2;
    
    ck1 = ( dataEx[2] ^ dataEx[3] ^ dataEx[4] ^ dataEx[7] ^ dataEx[8] ) & 0xFE;
    ck2 = checksum2(ck1);
    
    if( ck1 != dataEx[5] ) return -1;
    if( ck2 != dataEx[6] ) return -2;
    
    return dataEx[7];
}

void Servo_torqueON( uint8 servoID )
{
    pSize = 0x0A;
    pID = servoID;
    cmd = RAM_WRITE;
    data[0] = 0x34;
    data[1] = 0x01;
    data[2] = 0x60;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    
    sendData( dataEx , pSize );
}

void Servo_torqueOFF( uint8 servoID )
{
    pSize = 0x0A;
    pID = servoID;
    cmd = RAM_WRITE;
    data[0] = 0x34;
    data[1] = 0x01;
    data[2] = 0x00;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    
    sendData( dataEx , pSize );
}

void Servo_ACK( uint8 valueACK )
{
    pSize = 0x0A;
    pID = PIDALL;
    cmd = RAM_WRITE;
    data[0] = 0x34;
    data[1] = 0x01;
    data[2] = valueACK;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    
    sendData( dataEx , pSize );    
}

uint8 Servo_model( void )
{
    pSize = 0x09;
    pID = PIDALL;
    cmd = EEP_READ;
    data[0] = 0x00;
    data[1] = 0x01;
    lengthString = 2;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    
    sendData( dataEx , pSize );
    CyDelay(5);
    readData(9);
    
    pSize = dataEx[2];
    pID = dataEx[3];
    cmd = dataEx[4];
    data[0] = data[7];
    lengthString = 1;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    if( ck1 != dataEx[5] ) return -1;
    if( ck2 != dataEx[6] ) return -2;
    
    return dataEx[7];
}

void Servo_setID( uint8 oldID , uint8 newID )
{
    pSize = 0x0A;
    pID = oldID;
    cmd = EEP_WRITE;
    data[0] = 0x06;
    data[1] = 0x01;
    data[2] = newID;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    
    sendData( dataEx , pSize );
}

void Servo_resetID( void )
{
    uint8 i;
    pSize = 0x09;
    cmd = EEP_READ;
    data[0] = 0x06;
    data[1] = 0x01;
    
    lengthString = 2u;
    
    for ( i = 1 ; i <= 253 ; i++ )
    {
        pID = i;
        ck1 = checksum1( data , lengthString );
        ck2 = checksum2( ck1 );
        
        dataEx[0] = HEADER;
        dataEx[1] = HEADER;
        dataEx[2] = pSize;
        dataEx[3] = pID;
        dataEx[4] = cmd;
        dataEx[5] = ck1;
        dataEx[6] = ck2;
        dataEx[7] = data[0];
        dataEx[8] = data[1];
        
        sendData( dataEx , pSize );
        CyDelay(5);
        readData( 15 );
        CyDelay(10);
        Servo_setID( dataEx[3] , 253 );        
    }
}

void Servo_clearError( uint8 servoID )
{
    pSize = 0x0B;
    pID = servoID;
    cmd = RAM_WRITE;
    data[0] = 0x30;
    data[1] = 0x02;
    data[2] = 0x00;
    data[3] = 0x00;
    lengthString = 4;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    dataEx[10] = data[3];
    
    sendData( dataEx , pSize );
}

void Servo_moveAll( uint8 servoID , int goal , uint8 LED )
{
    if( goal > 1023 || goal < 0 ) return;
    
    uint8 mode = 0;
    uint8 stop = 0;
    
    int posLSB = goal & 0x00FF;
    int posMSB = ( goal & 0xFF00 ) >> 8;
    
    uint8 blue = 0;
    uint8 green = 0;
    uint8 red = 0;
    
    switch( LED )
    {
        case 1:
            green = 1;
            break;
        case 2:
            blue = 1;
            break;
        case 4:
            red = 1;
            break;
    }
    
    int setValue = stop + mode * 2 + green * 4 + blue * 8 + red * 16;
    
    addData( posLSB , posMSB , setValue , servoID );
}

void Servo_moveAllAngle( uint8 servoID , float angle , uint8 LED )
{
    if( angle > 160.0 || angle < -160.0 ) return;
    int position = (int)(angle/0.325) + 512;
    Servo_moveAll( servoID , position , LED );
}

void Servo_moveSpeedAll( uint8 servoID , int goal , uint8 LED )
{
    if( goal > 1023 || goal < -1023 ) return;
    
    uint8 mode = 1;
    uint8 stop = 0;
    
    int goalSpeedSign;
    if ( goal < 0 ) 
    {
        goalSpeedSign = (-1) * goal;
        goalSpeedSign |= 0x4000;
    }
    else 
    {
        goalSpeedSign = goal;
    }
    
    int speedGoalLSB = goalSpeedSign & 0X00FF;
    int speedGoalMSB = ( goalSpeedSign & 0xFF00 ) >> 8;

    int blue = 0;
    int green = 0;
    int red = 0;
    switch (LED) 
    {
        case 1:
            green = 1;
            break;
        case 2:
            blue = 1;
            break;
        case 4:
            red = 1;
            break;
    }
    
    int setValue = stop + mode * 2 + green * 4 + blue * 8 + red * 16;
    
    addData( speedGoalLSB , speedGoalMSB , setValue , servoID );
}

void Servo_actionAll( int time )
{
    uint8 i;
    
    if( time < 0 || time > 2856 ) return;
    
    pSize = 0x08 + conta;
    cmd = S_JOG;
    playTime = (int)((float)time/11.2);
    
    pID = PIDALL ^ playTime;
    
    ck1 = checksum1 ( moveData , conta );
    ck2 = checksum2 ( ck1 );
    
    pID = PIDALL;
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = playTime;
    
    for ( i = 0; i < conta; i++ )
    {
        dataEx[i+8] = moveData[i];
    }
    
    sendData( dataEx , pSize );
    
    conta = 0;
}

int Servo_getPosition( uint8 servoID )
{
    int position = 0;
    
    pSize = 0x09;
    pID = servoID;
    cmd = RAM_READ;
    data[0] = 0x3A;
    data[1] = 0x02;
    lengthString = 2;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    
    sendData( dataEx , pSize );
    CyDelay(5);
    readData(13);
    
    pSize = dataEx[2];
    pID = dataEx[3];
    cmd = dataEx[4];
    data[0] = data[7];
    data[1] = data[8];
    data[2] = data[9];
    data[3] = data[10];
    data[4] = data[11];
    data[5] = data[12];
    lengthString = 6;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    if( ck1 != dataEx[5] ) return -1;
    if( ck2 != dataEx[6] ) return -1;
    
    position = ( (dataEx[10] & 0x03) << 8 ) | dataEx[9];
    return position;
}

float Servo_getAngle( uint8 servoID )
{
    int pos = (int)Servo_getPosition(servoID);
    return ( pos - 512 ) * 0.325;
}

int Servo_getSpeed( uint8 servoID )
{
    int speed = 0;
    
    pSize = 0x09;
    pID = servoID;
    cmd = RAM_READ;
    data[0] = 0x40;
    data[1] = 0x02;
    lengthString = 2;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    
    sendData( dataEx , pSize );
    CyDelay(5);
    readData(13);
    
    pSize = dataEx[2];
    pID = dataEx[3];
    cmd = dataEx[4];
    data[0] = data[7];
    data[1] = data[8];
    data[2] = data[9];
    data[3] = data[10];
    data[4] = data[11];
    data[5] = data[12];
    lengthString = 6;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    if( ck1 != dataEx[5] ) return -1;
    if( ck2 != dataEx[6] ) return -1;
    
    speed = ( (dataEx[10] & 0xFF) << 8 ) | dataEx[9];
    return speed;
}

void Servo_reboot( uint8 servoID )
{
    pSize = 0x07;
    pID = servoID;
    cmd = REBOOT;
    
    ck1 = ( pSize ^ pID ^ cmd) & 0xFE;
    ck2 = ( ~( pSize ^ pID ^ cmd ) ) & 0xFE;
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    
    sendData( dataEx , pSize );
}

void Servo_setLED( uint8 servoID , uint8 LED_Value )
{
    pSize = 0x0A;
    pID = servoID;
    cmd = RAM_WRITE;
    data[0] = 0x35;
    data[1] = 0x01;
    data[2] = LED_Value;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    
    sendData( dataEx , pSize );
}

void Servo_moveOne( uint8 servoID , int goal , int time , uint8 LED )
{
    if( goal > 1023 || goal < 0 ) return;
    if( time < 0 || time > 2856 ) return;
    
    int posLSB = goal & 0X00FF;
    int posMSB = ( goal & 0xFF00 ) >> 8;

    int blue = 0;
    int green = 0;
    int red = 0;
    switch (LED) 
    {
        case 1:
            green = 1;
            break;
        case 2:
            blue = 1;
            break;
        case 4:
            red = 1;
            break;
    }
    
    int setValue = green * 4 + blue * 8 + red * 16;
    
    playTime = (int)((float)time/11.2);
    
    pSize = 0x0C;
    cmd = S_JOG;
    
    data[0] = posLSB;
    data[1] = posMSB;
    data[2] = setValue;
    data[3] = servoID;
    
    pID = servoID ^ playTime;
    lengthString = 4;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    pID = servoID;
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = playTime;
    dataEx[8] = data[0];
    dataEx[9] = data[1];
    dataEx[10] = data[2];
    dataEx[11] = data[3];
    
    sendData( dataEx , pSize );
}

void Servo_moveOneAngle( uint8 servoID , float angle , int time , uint8 LED )
{
    if( angle > 160.0 || angle < -160.0 ) return;
    int position = (int)(angle/0.325) + 512;
    Servo_moveOne( servoID , position , time , LED );
}

void Servo_moveSpeedOne( uint8 servoID , int goal , int time , uint8 LED )
{
    /*if ( goal > 1023 || goal < -1023 ) return;
    if ( time < 0 || time > 2856 ) return;
    
    int goalSpeedSign;
    if ( goal < 0 ) {
        goalSpeedSign = (-1) * goal;
        goalSpeedSign |= 0x4000;
    }
    else {
        goalSpeedSign = goal;
    }
    
    int speedGoalLSB = goalSpeedSign & 0X00FF;
    int speedGoalMSB = ( goalSpeedSign & 0xFF00 ) >> 8;

    int blue = 0;
    int green = 0;
    int red = 0;
    switch (LED) {
        
        case 1:
            green = 1;
            break;
        case 2:
            blue = 1;
            break;
        case 4:
            red = 1;
            break;
    }
    
    int setValue = 2 + green * 4 + blue * 8 + red * 16;

    playTime = (int)((float)time/11.2);

    pSize = 0x0C;
    cmd = S_JOG;

    data[0] = speedGoalLSB;
    data[1] = speedGoalMSB;
    data[2] = setValue;
    data[3] = servoID;

    pID = servoID ^ playTime;

    lengthString = 4;

    ck1 = checksum1 ( data , lengthString );
    ck2 = checksum2 ( ck1 );

    pID = servoID;

    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = playTime;
    dataEx[8] = data[0];
    dataEx[9] = data[1];
    dataEx[10] = data[2];
    dataEx[11] = data[3];

    sendData ( dataEx , pSize );*/
    if( goal > 1023 || goal < -1023 ) return;
    if( time < 0 || time > 2856 ) return;
    
    int goalSpeedSign;
    if ( goal < 0 ) 
    {
        goalSpeedSign = (-1) * goal;
        goalSpeedSign |= 0x4000;
    }
    else 
    {
        goalSpeedSign = goal;
    }
    
    int speedGoalLSB = goalSpeedSign & 0x00FF;
    int speedGoalMSB = ( goalSpeedSign & 0xFF00 ) >> 8;

    int blue = 0;
    int green = 0;
    int red = 0;
    switch (LED) 
    {
        case 1:
            green = 1;
            break;
        case 2:
            blue = 1;
            break;
        case 4:
            red = 1;
            break;
    }
    
    int setValue = 2 + green * 4 + blue * 8 + red * 16;
    
    playTime = (int)((float)time/11.2);
    
    pSize = 0x0C;
    cmd = S_JOG;
    
    data[0] = speedGoalLSB;
    data[1] = speedGoalMSB;
    data[2] = setValue;
    data[3] = servoID;
    
    pID = servoID ^ playTime;
    lengthString = 4;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    pID = servoID;
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = playTime;
    dataEx[8] = data[0];
    dataEx[9] = data[1];
    dataEx[10] = data[2];
    dataEx[11] = data[3];
    
    sendData( dataEx , pSize );
}

void Servo_writeRegistryRAM( uint8 servoID , uint8 address , uint8 writeByte )
{
    pSize = 0x0A;
    pID = servoID;
    cmd = RAM_WRITE;
    data[0] = address;
    data[1] = 0x01;
    data[2] = writeByte;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    dataEx[10] = data[3];
    
    sendData( dataEx , pSize );
}

void Servo_writeRegistryEEP( uint8 servoID , uint8 address , uint8 writeByte )
{
    pSize = 0x0A;
    pID = servoID;
    cmd = EEP_WRITE;
    data[0] = address;
    data[1] = 0x01;
    data[2] = writeByte;
    lengthString = 3;
    
    ck1 = checksum1( data , lengthString );
    ck2 = checksum2( ck1 );
    
    dataEx[0] = HEADER;
    dataEx[1] = HEADER;
    dataEx[2] = pSize;
    dataEx[3] = pID;
    dataEx[4] = cmd;
    dataEx[5] = ck1;
    dataEx[6] = ck2;
    dataEx[7] = data[0];
    dataEx[8] = data[1];
    dataEx[9] = data[2];
    dataEx[10] = data[3];
    
    sendData( dataEx , pSize );
}
/* [] END OF FILE */
