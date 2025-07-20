#include <stdint.h>
#include "tm4c.h"

#define RED_LED     0x02  // PF1
#define BLUE_LED    0x04  // PF2 

// Global counter for SysTick timing
volatile uint32_t counter = 0;

// Function prototypes
void PortF_Init(void);
void SysTick_Init(void);
void RED_Task(void);  // Red LED task
void BLUE_Task(void);  // Blue LED task

int main(void){
    // Initialize Port F for LEDs
    PortF_Init();
    
    // Initialize SysTick timer for 100ms interrupts
    SysTick_Init();
    
		volatile uint32_t task = 1;
	
		if (task){
			RED_Task();
		}else{
			BLUE_Task();
		}
    
    // This infinite loop should never be reached
    while(1){
        // Should never execute
    }
}

// Task 1: Toggle Red LED every 500ms (5 * 100ms)
void RED_Task(void){
    uint32_t lastToggleTime = 0;
    
    // Task initialization code (executed once)
    // Red LED starts OFF
    GPIO_PORTF_DATA_R &= ~RED_LED;
    
    // Infinite loop for the task
    for(;;){
        // Check if 500ms have passed (5 counter increments)
        if((counter - lastToggleTime) >= 5){
            lastToggleTime = counter;
						// Ensure bLUE led is off
					  GPIO_PORTF_DATA_R &= ~BLUE_LED;
					
            GPIO_PORTF_DATA_R ^= RED_LED;  // Toggle Red LED
        }
    }
}

// Task 2: Toggle Blue LED every 1000ms (10 * 100ms)
void BLUE_Task(void){
    uint32_t lastToggleTime = 0;
    
    // Task initialization code (executed once)
    // Blue LED starts OFF
    GPIO_PORTF_DATA_R &= ~BLUE_LED;
    
    // Infinite loop for the task
    for(;;){
        // Check if 1000ms have passed (10 counter increments)
        if((counter - lastToggleTime) >= 10){
            lastToggleTime = counter;
						// Ensure Red led is off
					  GPIO_PORTF_DATA_R &= ~RED_LED;

            GPIO_PORTF_DATA_R ^= BLUE_LED;  // Toggle Blue LED
        }
    }
}

// Port F Initialization Function
void PortF_Init(void){
    // Enable clock for Port F
    SYSCTL_RCGCGPIO_R |= 0x20;
    
    // Wait until Port F is ready
    while((SYSCTL_PRGPIO_R & 0x20) == 0);
    
    // Set PF1 and PF2 as outputs
    GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED);
    
    // Enable digital function for PF1 and PF2
    GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED);
    
    // Initialize LEDs to OFF state
    GPIO_PORTF_DATA_R &= ~(RED_LED | BLUE_LED);
}

// SysTick Initialization Function
void SysTick_Init(void){
    // Disable SysTick during setup
    NVIC_ST_CTRL_R = 0;
    
    // Set reload value for 100ms interrupt
    // Assuming 16 MHz system clock: 16,000,000 * 0.1 = 1,600,000
    NVIC_ST_RELOAD_R = 1600000 - 1;
    
    // Clear current register
    NVIC_ST_CURRENT_R = 0;
    
    // Enable SysTick with core clock and interrupt
    NVIC_ST_CTRL_R = 0x07;  // Enable, interrupt enable, use system clock
}

// SysTick Interrupt Handler
void SysTick_Handler(void){
    // Increment counter every 100ms
    counter++;
}