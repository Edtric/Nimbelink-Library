/* ========================================
 *
 * Copyright edtric corp, 2015
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF edtric corp.
 *
 * ========================================
*/


#ifndef HERKULEX2_H
#define HERKULEX2_H
    
/*=========================================*/
/*=========== File Dependencies ===========*/    
/*=========================================*/    
    #include <project.h>

/*=========================================*/

    #define HEADER      0xFFu
    #define PIDALL      0xFEu
    #define DATASIZE    30u
    #define DATAMOVE    50u
    #define TIMEOUT     5u

/*=========================================*/
/*=============== Commands ================*/    
/*=========================================*/
   
    #define EEP_WRITE   0x01u
    #define EEP_READ    0x02u
    #define RAM_WRITE   0x03
    #define RAM_READ    0x04u
    #define I_JOG       0x05u
    #define S_JOG       0x06u
    #define STAT        0x07u
    #define ROLLBACK    0x08u
    #define REBOOT      0x09u 
    
/*=========================================*/
/*================ Errors =================*/
/*=========================================*/

    #define STATUS_OK                   0x00u
    #define ERROR_INPUT_VOLTAGE_LIM     0x01u
    #define ERROR_POT_LIM               0x02u
    #define ERROR_TEMP_LIM              0x04u
    #define ERROR_INVALID_PCKT          0x08u
    #define ERROR_OVERLOAD              0x10u
    #define ERROR_DRIVER_FAULT          0x20u
    #define ERROR_EEPREG_DISTORTED      0x40u

/*=========================================*/
/*============== LED Colors ===============*/    
/*=========================================*/
    
    #define LED_GREEN   0x01u
    #define LED_BLUE    0x02u
    #define LED_CYAN    0x03u
    #define LED_RED     0x04u
    #define LED_GREEN2  0x05u
    #define LED_PINK    0x06u
    #define LED_WHITE   0x07u

/*=========================================*/
/*========== Function Prototypes ==========*/
/*=========================================*/
    
    void Servo_Start( void );
    uint8 Servo_status( uint8 servoID );
    void Servo_ACK( uint8 valueACK );
    uint8 Servo_model( void );
    void Servo_setID( uint8 oldID , uint8 newID );
    void Servo_clearError( uint8 servoID );
    
    void Servo_torqueON( uint8 servoID );
    void Servo_torqueOFF( uint8 servoID );
    
    void Servo_moveAll (uint8 servoID , int goal , uint8 LED );
    void Servo_moveSpeedAll (uint8 servoID , int goal , uint8 LED );
    void Servo_moveAllAngle (uint8 servoID , float angle , uint8 LED );
    void Servo_actionAll ( int time );
    
    void Servo_moveSpeedOne ( uint8 servoID , int goal , int time , uint8 LED );
    void Servo_moveOne (uint8 servoID , int goal , int time , uint8 LED );
    void Servo_moveOneAngle (uint8 servoID , float angle , int time , uint8 LED );
    
    int Servo_getPosition ( uint8 servoID );
    float Servo_getAngle ( uint8 servoID );
    int Servo_getSpeed ( uint8 servoID );
    
    void Servo_reboot( uint8 servoID );
    void Servo_setLED( uint8 servoID , uint8 LED_Value );
    
    void Servo_writeRegistryRAM( uint8 servoID , uint8 address , uint8 writeByte );
    void Servo_writeRegistryEEP( uint8 servoID , uint8 address , uint8 writeByte );
    
    void Servo_resetID ( void );
    
    /*Source File Functions*/
    
    
    
/*=========================================*/ 
    
    
#endif    

/* [] END OF FILE */
