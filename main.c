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
#include "project.h"
#include "Interrupt_Routines.h"
#include "Define.h"

uint8_t tx_data_rate;
uint8_t n_samples = 0b00010100;
uint8_t status_channels = 0b00;
uint8_t n_channels = 0;
uint32 count = 0;
int32 mean_LDR =0;
int32 mean_TRM =0;
int32 value_digit;
int32 value_mv;

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; 

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    EZI2C_1_Start();
    ADC_DelSig_1_Start();
    AMux_1_Start();       
    isr_TIMER_StartEx(Custom_ISR_ADC);
    Timer_ADC_Start();                      
    tx_data_rate = 50;    //Starts at 50 Hz 
    
    /* Initialization/startup code  */
    
    slaveBuffer[EZI2C_1_CTRL_REG0] = 0b00010100;             //set n_samples = 5 and status 0b00 (stop)
    slaveBuffer[EZI2C_1_CTRL_REG1] = tx_data_rate;
    slaveBuffer[EZI2C_1_WHO_AM_I] = EZI2C_1_WHO_AM_I_ADDR;   //set WHO AM I register to 0xBC
    slaveBuffer[CH0_BIT_15_08] = 0b00; 
    slaveBuffer[CH0_BIT_07_00] = 0b00; 
    slaveBuffer[CH1_BIT_15_08] = 0b00; 
    slaveBuffer[CH1_BIT_07_00] = 0b00; 
    
    EZI2C_1_SetBuffer1(SLAVE_BUFFER_SIZE, 2, slaveBuffer);
    
    for(;;)
    { 
    }
}

/* [] END OF FILE */
