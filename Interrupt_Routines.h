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
#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    #define SLAVE_DEVICE_ADDRESS 0xBC
    #include "cytypes.h"
    #include "stdio.h"
    #define BYTES_TO_SEND 2
    
    
    CY_ISR_PROTO(Custom_ISR_ADC); 
    
    
    volatile uint8 PacketReadyFlag;
    
    
#endif
/* [] END OF FILE */
