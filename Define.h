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
#ifndef __DEFINE_H_
    #define __DEFINE_H
    
    // Initialization of the Slave Buffer Size. Change it accordingly to the requirements. 
    #define SLAVE_BUFFER_SIZE 7
    
    #define TIMER_CLOCK_FREQ 50000
    
    #define EZI2C_1_CTRL_REG0 0 
    #define EZI2C_1_CTRL_REG1 1
    #define EZI2C_1_WHO_AM_I 2
    #define CH0_BIT_15_08 3
    #define CH0_BIT_07_00 4
    #define CH1_BIT_15_08 5
    #define CH1_BIT_07_00 6
    
    // Initialization of the Slave Buffer Addresses. Change it accordingly to the requirements. 
    #define EZI2C_1_DEVICE_ADDR 0x08   
    #define EZI2C_1_WHO_AM_I_ADDR 0xBC   
    #define EZI2C_1_CTRL_REG0_ADDR 0x00    
    #define EZI2C_1_CTRL_REG1_ADDR 0x01    
    #define EZI2C_1_CH0_BIT_15_8_ADDR 0x03   
    #define EZI2C_1_CH0_BIT_7_0_ADDR 0x04   
    #define EZI2C_1_CH1_BIT_15_8_ADDR 0x05
    #define EZI2C_1_CH1_BIT_7_0_ADDR 0x06  
    
    
    #define MASK_BIT_SAMPLES 0b00111100    
    #define MASK_BIT_STATUS 0b00000011
    
    #define NO_CH_SELECTED 0b00
    #define CH0_SELECTED 0b01
    #define CH1_SELECTED 0b10
    #define BOTH_SELECTED 0b11
    
    #define CHANEL_0 0
    #define CHANEL_1 1
    
    
#endif    
    

/* [] END OF FILE */
