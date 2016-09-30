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
#include "herkulex2.h"
char message[150];

int main()
{
    CyGlobalIntEnable;
    UART_NL_Start();
    UART_SRV_Start();
    UART_DEBUG_Start();
    CyDelay(250);
//    Gyro_Start( L3GD20_SENSITIVITY_250DPS );
    Servo_Start();
    Nimbelink_Start();
    Nimbelink_ConnectGS("4975","76.240.165.169");
    
//    CyDelay(250);

    for(;;)
    {
//        Gyro_Read();
//        sprintf( message , "X: %d    Y: %d    Z: %d" , (int)gyro.x , (int)gyro.y , (int)gyro.z );
//        CyDelay(10);
        Nimbelink_GSDataXfer( 1 , "Test message" );
        CyDelay(250);
    }
}

/* [] END OF FILE */
