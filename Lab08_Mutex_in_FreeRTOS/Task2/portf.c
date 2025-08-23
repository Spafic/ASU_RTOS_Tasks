#include "portf.h"

#define PORTF 5  // Port F is at bit position 5 in the RCGCGPIO register

void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= (1 << PORTF);    // Enable clock for Port F
    GPIO_PORTF_DIR_R = 0x0E;            // Set PF1,PF2,PF3 as outputs, PF0,PF4 as inputs
    GPIO_PORTF_LOCK_R=0x4C4F434B;        // Unlock Port F (required for PF0)
    GPIO_PORTF_CR_R=0x1F;               // Allow changes to PF4-PF0
    GPIO_PORTF_PUR_R=0x11;              // Enable pull-up resistors on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;            // Enable digital function on PF4-PF0
    GPIO_PORTF_DATA_R = 0x00;           // Initialize all pins to 0
}

void PortF_Interrupt_Settings(void){
    GPIO_PORTF_IS_R &= ~0x11;     // PF0 and PF4 are edge-sensitive
    GPIO_PORTF_IBE_R &= ~0x11;    // PF0 and PF4 are not both edges
    GPIO_PORTF_IEV_R &= ~0x11;    // PF0 and PF4 falling edge event
    GPIO_PORTF_ICR_R = 0x11;      // Clear any prior interrupts
    GPIO_PORTF_IM_R |= 0x11;      // Unmask interrupt for PF0 and PF4
}

void PortF_Interrupt_Enable(void){
     /* Set Port F (IRQ30) priority to 5 which matches
         configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h
         (safe priority for ISRs that call FreeRTOS API). */
     NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | (5 << 21); // Priority 5 for Port F
    NVIC_EN0_R |= (1 << 30);      // Enable interrupt 30 (Port F) in NVIC
}