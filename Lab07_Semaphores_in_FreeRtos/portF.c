#include "portF.h"

void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= (1<<PORTF);    // Enable clock for Port F
    GPIO_PORTF_DIR_R = 0x0E;            // Set PF1,PF2,PF3 as outputs, PF0,PF4 as inputs
    GPIO_PORTF_LOCK_R=0x4C4F434B;        // Unlock Port F (required for PF0)
    GPIO_PORTF_CR_R=0x1F;               // Allow changes to PF4-PF0
    GPIO_PORTF_PUR_R=0x11;              // Enable pull-up resistors on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;            // Enable digital function on PF4-PF0
    GPIO_PORTF_DATA_R = 0x00;           // Initialize all pins to 0
}