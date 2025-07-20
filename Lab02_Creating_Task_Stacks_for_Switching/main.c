#include <stdint.h>
#include "tm4c.h"

#define RED_LED     0x02  // PF1
#define BLUE_LED    0x04  // PF2 

// Global counter for SysTick timing
volatile uint32_t counter = 0;

// Task stacks (40 words each)
uint32_t stack_RedBlinky[40];
uint32_t *sp_RedBlinky = &stack_RedBlinky[40];

uint32_t stack_BlueBlinky[40];
uint32_t *sp_BlueBlinky = &stack_BlueBlinky[40];

// Function prototypes
void PortF_Init(void);
void SysTick_Init(void);
void Task1(void);  // Red LED task
void Task2(void);  // Blue LED task
void Prepare_Task_Stacks(void);

int main(void){
    PortF_Init();
    SysTick_Init();
    Prepare_Task_Stacks();  // Fill both stacks to simulate interrupt frame

    while(1){
        // Switching between tasks is done manually via debugger
    }
}

// Task 1: Toggle Red LED every 500ms (5 * 100ms)
void Task1(void){
    uint32_t lastToggleTime = 0;

    // Red LED starts OFF
    GPIO_PORTF_DATA_R &= ~RED_LED;

    for(;;){
        if((counter - lastToggleTime) >= 5){
            lastToggleTime = counter;
            GPIO_PORTF_DATA_R &= ~BLUE_LED;     // Ensure Blue is OFF
            GPIO_PORTF_DATA_R ^= RED_LED;       // Toggle Red LED
        }
    }
}

// Task 2: Toggle Blue LED every 1000ms (10 * 100ms)
void Task2(void){
    uint32_t lastToggleTime = 0;

    // Blue LED starts OFF
    GPIO_PORTF_DATA_R &= ~BLUE_LED;

    for(;;){
        if((counter - lastToggleTime) >= 10){
            lastToggleTime = counter;
            GPIO_PORTF_DATA_R &= ~RED_LED;      // Ensure Red is OFF
            GPIO_PORTF_DATA_R ^= BLUE_LED;      // Toggle Blue LED
        }
    }
}

// Port F Initialization
void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x20;           // Enable clock for Port F
    while((SYSCTL_PRGPIO_R & 0x20) == 0); // Wait until ready

    GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED);   // Set PF1, PF2 as output
    GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED);   // Enable digital function
}

// SysTick Initialization
void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0;                  // Disable SysTick
    NVIC_ST_RELOAD_R = 1600000 - 1;      // Reload for 100ms (16MHz clock)
    NVIC_ST_CURRENT_R = 0;               // Clear current value
    NVIC_ST_CTRL_R = 0x07;               // Enable with core clock and interrupt
}

// SysTick ISR
void SysTick_Handler(void){
    counter++;  // Called every 100ms
}

// Prepare the stack for each task as if preempted by interrupt
void Prepare_Task_Stacks(void){
    // ---- Stack for Task1 (Red) ----
    sp_RedBlinky--;             // xPSR
    *sp_RedBlinky = 0x01000000; // Thumb bit set

    sp_RedBlinky--;             // PC
    *sp_RedBlinky = (uint32_t)Task1;

    sp_RedBlinky--; *sp_RedBlinky = 0x14141414; // LR
    sp_RedBlinky--; *sp_RedBlinky = 0x12121212; // R12
    sp_RedBlinky--; *sp_RedBlinky = 0x03030303; // R3
    sp_RedBlinky--; *sp_RedBlinky = 0x02020202; // R2
    sp_RedBlinky--; *sp_RedBlinky = 0x01010101; // R1
    sp_RedBlinky--; *sp_RedBlinky = 0x00000000; // R0

    // ---- Stack for Task2 (Blue) ----
    sp_BlueBlinky--;
    *sp_BlueBlinky = 0x01000000;

    sp_BlueBlinky--;
    *sp_BlueBlinky = (uint32_t)Task2;

    sp_BlueBlinky--; *sp_BlueBlinky = 0x25252525; // LR
    sp_BlueBlinky--; *sp_BlueBlinky = 0x12121212; // R12
    sp_BlueBlinky--; *sp_BlueBlinky = 0x03030303; // R3
    sp_BlueBlinky--; *sp_BlueBlinky = 0x02020202; // R2
    sp_BlueBlinky--; *sp_BlueBlinky = 0x01010101; // R1
    sp_BlueBlinky--; *sp_BlueBlinky = 0x00000000; // R0
}