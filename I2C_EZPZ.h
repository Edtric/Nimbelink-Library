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

#ifndef I2C_EZPZ_H
#define I2C_EZPZ_H
    
    #include <project.h>
    
    #define WRITE   0x00u
    #define READ    0x01u
    
    void I2C_Write2Reg( uint8 address , uint8 reg );
    void I2C_WriteByte( uint8 address , uint8 reg , uint8 data );
    uint8 I2C_ReadByteNW( uint8 address );
    void I2C_ReadBytesNW( uint8 address , uint8 bytesToRead , uint8 * data );
    uint8 I2C_ReadByte( uint8 address , uint8 reg );
    void I2C_ReadBytes( uint8 address , uint8 reg , uint8 bytesToRead , uint8 * data );
    
#endif    

/* [] END OF FILE */
