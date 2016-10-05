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

#ifndef L3GD20_I2C_H
#define L3GD20_I2C_H
    
    #include <project.h>
    
    #define GYRO_ADDR   0x6B
    #define GYRO_ID     0xD4
    
    #define L3GD20_SENSITIVITY_250DPS  (0.00875F)      // Roughly 22/256 for fixed point match
    #define L3GD20_SENSITIVITY_500DPS  (0.0175F)       // Roughly 45/256
    #define L3GD20_SENSITIVITY_2000DPS (0.070F)        // Roughly 18/256
    #define L3GD20_DPS_TO_RADS         (0.017453293F)  // degress/s to rad/s multiplier
    
    typedef enum
    {                                                 // DEFAULT    TYPE
        L3GD20_REGISTER_WHO_AM_I            = 0x0F,   // 11010100   r
        L3GD20_REGISTER_CTRL_REG1           = 0x20,   // 00000111   rw
        L3GD20_REGISTER_CTRL_REG2           = 0x21,   // 00000000   rw
        L3GD20_REGISTER_CTRL_REG3           = 0x22,   // 00000000   rw
        L3GD20_REGISTER_CTRL_REG4           = 0x23,   // 00000000   rw
        L3GD20_REGISTER_CTRL_REG5           = 0x24,   // 00000000   rw
        L3GD20_REGISTER_REFERENCE           = 0x25,   // 00000000   rw
        L3GD20_REGISTER_OUT_TEMP            = 0x26,   //            r
        L3GD20_REGISTER_STATUS_REG          = 0x27,   //            r
        L3GD20_REGISTER_OUT_X_L             = 0x28,   //            r
        L3GD20_REGISTER_OUT_X_H             = 0x29,   //            r
        L3GD20_REGISTER_OUT_Y_L             = 0x2A,   //            r
        L3GD20_REGISTER_OUT_Y_H             = 0x2B,   //            r
        L3GD20_REGISTER_OUT_Z_L             = 0x2C,   //            r
        L3GD20_REGISTER_OUT_Z_H             = 0x2D,   //            r
        L3GD20_REGISTER_FIFO_CTRL_REG       = 0x2E,   // 00000000   rw
        L3GD20_REGISTER_FIFO_SRC_REG        = 0x2F,   //            r
        L3GD20_REGISTER_INT1_CFG            = 0x30,   // 00000000   rw
        L3GD20_REGISTER_INT1_SRC            = 0x31,   //            r
        L3GD20_REGISTER_TSH_XH              = 0x32,   // 00000000   rw
        L3GD20_REGISTER_TSH_XL              = 0x33,   // 00000000   rw
        L3GD20_REGISTER_TSH_YH              = 0x34,   // 00000000   rw
        L3GD20_REGISTER_TSH_YL              = 0x35,   // 00000000   rw
        L3GD20_REGISTER_TSH_ZH              = 0x36,   // 00000000   rw
        L3GD20_REGISTER_TSH_ZL              = 0x37,   // 00000000   rw
        L3GD20_REGISTER_INT1_DURATION       = 0x38    // 00000000   rw
    }   l3gd20Registers_t;
    
    typedef enum
    {
        L3GD20_RANGE_250DPS,
        L3GD20_RANGE_500DPS,
        L3GD20_RANGE_2000DPS
    } l3gd20Range_t;
    
    typedef struct l3gd20Data_s
    {
        float x;
        float y;
        float z;
    } l3gd20Data;
    
    uint8 Gyro_Start( l3gd20Range_t rng );
    void Gyro_Calibrate( void );
    void Gyro_Read( void );
    
    l3gd20Data gyro;

#endif
/* [] END OF FILE */
