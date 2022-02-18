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
#include "Interrupt_Routines.h"
#include "project.h" 
#include "Define.h"

extern int32 value_digit;
extern int32 value_mv;

extern uint8_t tx_data_rate;
extern uint8_t slaveBuffer[];
extern uint8_t n_samples;
extern uint8_t status_channels;
extern uint32 count;
extern int32 mean_LDR;
extern int32 mean_TRM;
extern uint8_t n_channels;
extern uint8_t period_ADC;


CY_ISR(Custom_ISR_ADC){
    Timer_ADC_ReadStatusRegister();
    value_digit = ADC_DelSig_1_Read32(); 
    
    //limit the value between 2^16 -1 and 0
    if(value_digit < 0)
    {
        value_digit =0;
    }
    if (value_digit > 65535)
    {
        value_digit = 65535; 
    }
    value_mv = ADC_DelSig_1_CountsTo_mVolts(value_digit);
    count++;   
    if (status_channels == CH0_SELECTED)                            
    {   
        mean_TRM += value_mv;                            // Store the sum of the #n_samples iteratively
        if (count == ((n_samples>>2)))                      // If count =  n_samples, compute the mean_TRM
        {
            mean_TRM = mean_TRM/(n_samples>>2);
            slaveBuffer[CH0_BIT_15_08] = mean_TRM >> 8;     //store the mean_TRM in the slaveBuffer
            slaveBuffer[CH0_BIT_07_00] = mean_TRM & 0xFF;
            count = 0;                                      
            mean_TRM = 0;
        }
    }
                        
    else if (status_channels == CH1_SELECTED)    
        {           
            mean_LDR += value_mv;                        // Store the sum of the #n_samples iteratively
            if (count == ((n_samples>>2)))                  // If count =  n_samples, compute the mean_LDR
            {
            mean_LDR = mean_LDR/(n_samples>>2);
            slaveBuffer[CH1_BIT_15_08] = mean_LDR >> 8;     //store the mean_LDR in the slaveBuffer
            slaveBuffer[CH1_BIT_07_00] = mean_LDR & 0xFF;
            count = 0;
            mean_LDR = 0;
            }                   
        }
                        
    else if (status_channels == BOTH_SELECTED) 
    {                    
        if (count <= ((n_samples>>2)))                      // If count <= n_samples, value_digit represents sample of CH0 (TRM)
        {   
            mean_TRM += value_mv;
            if (count == ((n_samples>>2)))                  // If count =  n_samples, compute the mean_TRM
            {
                mean_TRM = mean_TRM/(n_samples>>2);
                slaveBuffer[CH0_BIT_15_08] = mean_TRM >> 8;
                slaveBuffer[CH0_BIT_07_00] = mean_TRM & 0xFF;
                mean_TRM = 0;
                AMux_1_FastSelect(CHANEL_1);                       // After having averaged samples of CH0, switch to CH1 (LDR)
            }
    }
                  
        else if (count > (n_samples>>2))                    // If count > n_samples, value_digit represents sample of CH1 (LDR)
        {
            mean_LDR += value_mv;
            if (count == ((n_samples>>2)*2))                // If count =  n_samples*2, compute the mean_LDR
            {
                mean_LDR = mean_LDR/(n_samples>>2);
                slaveBuffer[CH1_BIT_15_08] = mean_LDR >> 8; // store the mean_LDR in the slaveBuffer
                slaveBuffer[CH1_BIT_07_00] = mean_LDR & 0xFF;
                mean_LDR = 0;
                AMux_1_FastSelect(CHANEL_0);                       // After having averaged samples of CH1, switch to CH0 (TRM)
                count = 0;                                  // And reset the count
            }
        }
    }
}

void EZI2C_1_ISR_ExitCallback(void)
{
    if ((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_STATUS) == BOTH_SELECTED)
    {     
        Pin_STATUS_Write(1);                             
    }
    
    else 
    {
        Pin_STATUS_Write(0);                        
    }
    
    if (((slaveBuffer[0] & MASK_BIT_SAMPLES) != n_samples) || ((slaveBuffer[0] & MASK_BIT_STATUS) != status_channels) || (slaveBuffer[EZI2C_1_CTRL_REG1] != tx_data_rate))
        // if the number of samples OR the status_channel OR the tx_data_rate has been changed, update the working regime
    {
        count = 0; 
        mean_LDR =0;
        mean_TRM =0;
        //restart from the beginning if a change has been made in the middle of a previous sampling cycle.
        
        if ((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_SAMPLES) != n_samples)
        {   
            // update the number_samples variable according to what has been configured
             n_samples = (slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_SAMPLES);
            
        }
        
        if((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_STATUS) != status_channels)
        {   
            // update the status_channels variable according to what has been configured (e.g. no ch, ch0, ch1, or both)
            if ((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_STATUS) == NO_CH_SELECTED )
            {
                status_channels = NO_CH_SELECTED;
                n_channels = 0;
                AMux_1_DisconnectAll();                 
                Timer_ADC_Stop(); 
                // stop the ADC if no channel sampling is required.
            }
            else if ((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_STATUS) == CH0_SELECTED )
            {  
                status_channels = CH0_SELECTED;
                n_channels = 1;
                AMux_1_FastSelect(CHANEL_0);
            }
            else if ((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_STATUS) == CH1_SELECTED )
            {   
                status_channels = CH1_SELECTED;
                n_channels = 1;
                AMux_1_FastSelect(CHANEL_1);
            }
            else if ((slaveBuffer[EZI2C_1_CTRL_REG0] & MASK_BIT_STATUS) == BOTH_SELECTED )
            {   
                status_channels = BOTH_SELECTED;
                n_channels = 2;   
                AMux_1_FastSelect(CHANEL_0);       
            }
        }
               
        if(status_channels != NO_CH_SELECTED) 
        // if at least one channel has been selected, start and set the ISR frequency according to the configuration     
        // in order to guarantee the right amount of sampling every TX_DATARATE (50 Hz)
        {                            
            Timer_ADC_Stop();
            Timer_ADC_Start();
            Timer_ADC_WritePeriod((TIMER_CLOCK_FREQ/((n_samples>>2)*slaveBuffer[EZI2C_1_CTRL_REG1]*n_channels)));
            // Chosen 50'000 Hz to get entire value for max_frequency (f_max = 255(max value in reg1)*15(samp)*2(chan) = 7650 Hz)
            // and for min_frequency (f_min = 1(min value in reg1)*1*(samp)*1(chan) = 1 Hz) 
            //the frequency of the ISR depends on the number of samples, channels to be sampled and the transmission data rate.     
        }    
        
        if (slaveBuffer[EZI2C_1_CTRL_REG1] != tx_data_rate) //set the transmission data rate by taking the value from the slaveBuffer[1],
                                                            //where we have written the desidered one. 
        {
            tx_data_rate = slaveBuffer[EZI2C_1_CTRL_REG1];
        }
    }
}
/* [] END OF FILE */
