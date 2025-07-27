# Lab 4: Multiple Tasks using FreeRTOS

## Objective

Implement a multi-tasking system using FreeRTOS on the TM4C123GH6PM microcontroller to control RGB LEDs with different timing patterns. This lab demonstrates concurrent task execution, timing control, and real-time scheduling fundamentals.

## Learning Goals

- Create and manage multiple FreeRTOS tasks
- Understand task scheduling and concurrent execution
- Implement precise timing control using FreeRTOS APIs
- Analyze timing diagrams and validate expected behavior
- Explore the effects of task priorities on system behavior
- Compare different FreeRTOS delay mechanisms

## Hardware Setup

### Required Components
- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and debugging

### Pin Configuration
- **PF1 (Red LED)**: Output for Task1 - toggles every 1 second
- **PF2 (Blue LED)**: Output for Task2 - toggles every 2 seconds  
- **PF3 (Green LED)**: Output for Task3 - toggles every 3 seconds

### Connections
No external connections required - uses on-board RGB LED.

## Background

### Multi-Tasking with FreeRTOS

This lab demonstrates fundamental real-time operating system concepts:

- **Concurrent Execution**: Multiple tasks running independently
- **Task Scheduling**: FreeRTOS scheduler manages task execution
- **Timing Control**: Precise periodic task execution
- **Resource Sharing**: Multiple tasks accessing GPIO safely

### Task Timing Analysis

The system creates three tasks with different periods:
- **Task1 (Red)**: 1000ms period → toggles at 1s, 2s, 3s, 4s, 5s, 6s...
- **Task2 (Blue)**: 2000ms period → toggles at 2s, 4s, 6s...  
- **Task3 (Green)**: 3000ms period → toggles at 3s, 6s...

This creates a repeating 6-second pattern of combined LED colors.

## Implementation Tasks

### Task 1: Create New FreeRTOS Project

Set up a new project in Keil µVision:
1. Create new project for TM4C123GH6PM target
2. Add FreeRTOS kernel files to project
3. Configure FreeRTOSConfig.h for TM4C123 specifications
4. Include necessary header files and startup code

### Task 2: Initialize the Tasks

Create three concurrent tasks for LED control:

```c
#include <stdint.h>
#include "tm4c.h"
#include "FreeRTOS.h"
#include "task.h"

#define RED_LED 0x02
#define BLUE_LED 0x04
#define GREEN_LED 0x08

// Function prototypes
void PortF_Init(void);
void vTask1(void *pvParameters);  // Red LED - 1s period
void vTask2(void *pvParameters);  // Blue LED - 2s period
void vTask3(void *pvParameters);  // Green LED - 3s period
```

### Task 3: Main Function Implementation

```c
int main(void){
    PortF_Init();                    // Initialize GPIO Port F
    
    // Create three tasks with equal priority
    xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    vTaskStartScheduler();           // Start FreeRTOS scheduler
    
    for(;;);                        // Should never reach here
}
```

### Task 4: GPIO Initialization

```c
void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x20;              // Enable Port F clock
    while((SYSCTL_PRGPIO_R & 0x20) == 0);  // Wait for clock ready
    GPIO_PORTF_DIR_R = 0x0E;                // Set PF1, PF2, PF3 as outputs
    GPIO_PORTF_DEN_R = 0x0E;                // Enable digital function
    GPIO_PORTF_DATA_R = 0x00;               // Initialize all LEDs OFF
}
```

### Task 5: Task Implementation

#### Task1 - Red LED (1 second period)
```c
void vTask1(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;){
        GPIO_PORTF_DATA_R ^= RED_LED;                    // Toggle Red LED
        vTaskDelayUntil(&xLastWakeTime, 1000);          // Wait 1000ms
    }
}
```

#### Task2 - Blue LED (2 second period)  
```c
void vTask2(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;){
        GPIO_PORTF_DATA_R ^= BLUE_LED;                   // Toggle Blue LED
        vTaskDelayUntil(&xLastWakeTime, 2000);          // Wait 2000ms
    }
}
```

#### Task3 - Green LED (3 second period)
```c
void vTask3(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;){
        GPIO_PORTF_DATA_R ^= GREEN_LED;                  // Toggle Green LED
        vTaskDelayUntil(&xLastWakeTime, 3000);          // Wait 3000ms
    }
}
```

## Expected Behavior

### Individual LED Timing
- **Red LED**: ON(0-1s) → OFF(1-2s) → ON(2-3s) → OFF(3-4s) → ON(4-5s) → OFF(5-6s)
- **Blue LED**: ON(0-2s) → OFF(2-4s) → ON(4-6s)
- **Green LED**: ON(0-3s) → OFF(3-6s)

### Combined Color Output (6-second cycle)
- **0-1s**: WHITE (Red + Blue + Green)
- **1-2s**: CYAN (Blue + Green)  
- **2-3s**: YELLOW (Red + Green)
- **3-4s**: BLACK (All OFF)
- **4-5s**: MAGENTA (Red + Blue)
- **5-6s**: BLUE (Blue only)

## Building and Testing

### Using Keil µVision
1. Build project (F7)
2. Download to target (F8)
3. Run program and observe LED behavior
4. Verify timing matches expected patterns
5. Document actual vs expected behavior

### Debug Configuration
- Enable FreeRTOS awareness in debugger
- Monitor task states and switching
- Verify timing accuracy with stopwatch

## Analysis Questions

### Question 1: Timing Validation
Compare actual LED behavior with expected timing diagrams:
- Does the color sequence match predictions?
- Are the timing intervals accurate?
- Note any discrepancies and potential causes

### Question 2: Task Priority Effects
Analyze the impact of different task priorities:
- Would changing priorities affect the output?
- Why or why not?
- When do priorities matter in FreeRTOS systems?

### Question 3: Delay API Comparison
Compare `vTaskDelay()` vs `vTaskDelayUntil()`:
- Which API was used and why?
- What difference would the other API make?
- When is each API appropriate?

## Key Features

### FreeRTOS Concepts Demonstrated
- **Task Creation**: Multiple concurrent tasks
- **Task Scheduling**: Round-robin at equal priority
- **Timing Control**: Precise periodic execution
- **Resource Management**: Shared GPIO access

### Real-Time Characteristics
- **Deterministic Timing**: Predictable LED toggle intervals
- **Concurrent Operation**: Independent task execution
- **System Reliability**: Stable long-term operation

## Troubleshooting

### Common Issues

1. **LEDs Don't Toggle**
   - Verify GPIO initialization
   - Check task creation success
   - Ensure scheduler started properly

2. **Incorrect Timing**
   - Verify FreeRTOS tick configuration
   - Check delay values (1000, 2000, 3000 ticks)
   - Ensure system clock configuration

3. **System Hangs**
   - Check stack sizes are sufficient
   - Verify FreeRTOS configuration
   - Ensure infinite loops in tasks

### Debug Tips
- Use FreeRTOS-aware debugging
- Monitor task states in real-time
- Verify timing with external measurement
- Check memory usage and stack overflow

## Educational Value

This lab demonstrates:
- **Multi-tasking Fundamentals**: How RTOS manages multiple tasks
- **Timing Analysis**: Predicting and validating system behavior  
- **Real-time Design**: Creating deterministic embedded systems
- **System Integration**: Combining hardware and software timing

## Files Description

- `main.c`: Complete FreeRTOS multi-task implementation
- `tm4c.h`: TM4C123 register definitions
- `FreeRTOSConfig.h`: FreeRTOS configuration file
- Project files: Keil µVision configuration

## Lab Report Requirements

Your lab report should include:

1. **Timing Diagrams**: 
   - Individual LED timing diagram
   - Combined color output timing diagram

2. **Validation Results**:
   - Comparison of actual vs expected behavior
   - Analysis of any discrepancies

3. **Analysis Questions**:
   - Effect of different task priorities
   - Impact of using different delay APIs
   - System behavior explanations

4. **Conclusions**:
   - Understanding of FreeRTOS concepts
   - Real-time system design insights

---

**Note**: This lab provides practical experience with FreeRTOS multi-tasking and demonstrates the fundamental concepts needed for real-time embedded system development.