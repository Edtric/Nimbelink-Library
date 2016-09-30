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

#include "L3GD20_I2C.h"
#include "I2C_EZPZ.h"

uint8 readBuf[6];

uint8 range;

int cal_counter;
int cal_resolution = 1000;
float xcal_val;
float ycal_val;
float zcal_val;

uint8 Gyro_Start( l3gd20Range_t rng )
{
    range = rng;
    
    uint8 id = I2C_ReadByte( GYRO_ADDR , L3GD20_REGISTER_WHO_AM_I );
    
    if( id != GYRO_ID ) return 0;
    
    I2C_WriteByte( GYRO_ADDR , L3GD20_REGISTER_CTRL_REG1 , 0x0F );
    I2C_WriteByte( GYRO_ADDR , L3GD20_REGISTER_CTRL_REG4 , 0x80 );
    
    switch( range ) //Changes the accuracy of the device (DPS: Degrees Per Second)
    {
        case L3GD20_RANGE_250DPS:
            I2C_WriteByte( GYRO_ADDR , L3GD20_REGISTER_CTRL_REG4 , 0x00 );
            break;
        case L3GD20_RANGE_500DPS:
            I2C_WriteByte( GYRO_ADDR , L3GD20_REGISTER_CTRL_REG4 , 0x10 );
            break;
        case L3GD20_RANGE_2000DPS:
            I2C_WriteByte( GYRO_ADDR , L3GD20_REGISTER_CTRL_REG4 , 0x20 );
            break;
    }
    
    return 1;
}

void Gyro_Calibrate( void )
{
    for( cal_counter = 0 ; cal_counter < cal_resolution ; cal_counter++ )
    {
        Gyro_Read();
        xcal_val += gyro.x;
        ycal_val += gyro.y;
        zcal_val += gyro.z;
        CyDelay(10);
    }
    
    xcal_val /= cal_resolution;
    ycal_val /= cal_resolution;
    zcal_val /= cal_resolution;
}

void Gyro_Read( void )
{
    uint8_t xhi , xlo , yhi , ylo , zhi , zlo;
    
    I2C_ReadBytes( GYRO_ADDR , L3GD20_REGISTER_OUT_X_L | 0x80 , 6 , readBuf );
    
    xlo = readBuf[0];
    xhi = readBuf[1];
    ylo = readBuf[2];
    yhi = readBuf[3];
    zlo = readBuf[4];
    zhi = readBuf[5];
    
    //Shifts high byte combines it with low byte to create 2-byte integer
    gyro.x = (int16_t)( xlo | ( xhi << 8 ) );
    gyro.y = (int16_t)( ylo | ( yhi << 8 ) );
    gyro.z = (int16_t)( zlo | ( zhi << 8 ) );
    
    //Invert signs for aircraft axis
    gyro.y *= -1;
    gyro.z *= -1;
    
    switch( range )
    {
        case L3GD20_RANGE_250DPS:
            gyro.x *= L3GD20_SENSITIVITY_250DPS;
            gyro.y *= L3GD20_SENSITIVITY_250DPS;
            gyro.z *= L3GD20_SENSITIVITY_250DPS;
            break;
        case L3GD20_RANGE_500DPS:
            gyro.x *= L3GD20_SENSITIVITY_500DPS;
            gyro.y *= L3GD20_SENSITIVITY_500DPS;
            gyro.z *= L3GD20_SENSITIVITY_500DPS;
            break;
        case L3GD20_RANGE_2000DPS:
            gyro.x *= L3GD20_SENSITIVITY_2000DPS;
            gyro.y *= L3GD20_SENSITIVITY_2000DPS;
            gyro.z *= L3GD20_SENSITIVITY_2000DPS;
            break;
    }
    
    if( cal_counter == cal_resolution )
    {
        gyro.x -= xcal_val;
        gyro.y -= ycal_val;
        gyro.z -= zcal_val;
    }
}

/* [] END OF FILE */
