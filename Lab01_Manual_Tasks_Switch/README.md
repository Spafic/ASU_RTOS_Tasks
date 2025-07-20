# Lab01: Manual Tasks Switch

## Objective

Implement a basic cooperative task scheduling system without using an RTOS kernel. This lab introduces fundamental concepts of task management, cooperative scheduling, and timer-based task switching using the TM4C123GH6PM microcontroller.

## Learning Goals

- Understand cooperative vs. preemptive scheduling
- Implement manual task switching using function pointers
- Configure and use SysTick timer for precise timing
- Work with GPIO for LED control
- Learn basic task state management

## Hardware Setup

### Required Components
- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and debugging

### Pin Configuration
- **PF1 (Red LED)**: Output for Task1 indication
- **PF2 (Blue LED)**: Output for Task2 indication
- **PF3 (Green LED)**: Output for system status (optional)

### Connections
No external connections required - uses on-board LEDs.

## System Architecture

### Task Structure
The system implements two cooperative tasks:

```c
typedef struct {
    void (*taskFunction)(void);     // Function pointer to task
    uint32_t period;                // Task period in milliseconds
    uint32_t lastExecutionTime;     // Last execution timestamp
    uint8_t isReady;               // Task ready flag
} Task_t;
```

### Scheduling Algorithm
- **Type**: Cooperative (non-preemptive)
- **Method**: Round-robin with timing checks
- **Timer**: SysTick configured for 1ms interrupts
- **Task Selection**: Based on task periods and ready states

## Implementation Logic

### Core Components

1. **SysTick Timer Configuration**
   ```c
   // Configure SysTick for 100ms interrupts at 16MHz
   NVIC_ST_RELOAD_R = 1600000 - 1;  // 16MHz * 0.1s = 1,600,000
   NVIC_ST_CTRL_R = 0x07;           // Enable, interrupt, system clock
   ```

2. **Task Selection**
   ```c
   volatile uint32_t task = 1;
   if (task){
       RED_Task();    // Execute Red LED task
   }else{
       BLUE_Task();   // Execute Blue LED task
   }
   ```

3. **Task Implementations**
   - **RED_Task()**: Toggle Red LED every 500ms (5 * 100ms)
   - **BLUE_Task()**: Toggle Blue LED every 1000ms (10 * 100ms)

### System Flow

1. **Initialization Phase**
   - Configure system clock (16MHz)
   - Initialize GPIO for LEDs (PF1=Red, PF2=Blue)
   - Setup SysTick timer for 100ms interrupts
   - Set task variable to select which task runs

2. **Runtime Phase**
   - SysTick interrupt increments global counter every 100ms
   - Selected task runs in infinite loop
   - Task checks timing using counter variable
   - LED toggles at specified intervals
   - Only one task runs at a time (no task switching)

## Expected Behavior

### Normal Operation
- **Red LED**: Blinks every 500ms when `task = 1`
- **Blue LED**: Blinks every 1000ms when `task = 0`
- **System**: Maintains precise timing using SysTick 100ms interrupts
- **Task Selection**: Only one task runs based on task variable

### Timing Verification
Use oscilloscope or logic analyzer to verify:
- Red LED period: 500ms (toggles every 5 SysTick interrupts)
- Blue LED period: 1000ms (toggles every 10 SysTick interrupts)

## Key Code Sections

### Task Implementation Example
```c
void RED_Task(void){
    uint32_t lastToggleTime = 0;
    GPIO_PORTF_DATA_R &= ~RED_LED;  // Initialize LED OFF
    
    for(;;){  // Infinite loop
        if((counter - lastToggleTime) >= 5){  // 500ms check
            lastToggleTime = counter;
            GPIO_PORTF_DATA_R &= ~BLUE_LED;   // Ensure other LED off
            GPIO_PORTF_DATA_R ^= RED_LED;     // Toggle Red LED
        }
    }
}
```

### SysTick ISR
```c
void SysTick_Handler(void){
    counter++;  // Increment every 100ms
}
```

## Building and Running

### Using Keil µVision
1. Open `lab1.uvprojx`
2. Ensure target is set to TM4C123GH6PM
3. Build project (F7)
4. Connect LaunchPad via USB
5. Flash and debug (Ctrl+F5)

## Manual Task Switching Debug Exercise

### Steps to Switch from Red Task to Blue Task:

1. **Set Debug Breakpoint**
   - Place breakpoint on `counter++;` line in `SysTick_Handler()`

2. **Run Application**
   - Start debugging (F5)
   - Red LED should start toggling every 500ms
   - Breakpoint will hit every 100ms

3. **Locate Stack Pointer**
   - When breakpoint hits, go to **Registers** window
   - Note the **SP (Stack Pointer)** value

4. **Find Red Task Location**
   - In **Memory** window, navigate to SP location
   - This shows the current execution context (Red Task)

5. **Find Blue Task Address**
   - Open **Disassembly** window
   - Locate the first line of the infinite loop in `BLUE_Task()` (`for(;;)` loop)
   - Note the memory address of this instruction

6. **Manual Task Switch**
   - In **Memory** window, modify the return address on stack
   - Change it to the Blue Task loop address found in step 5

7. **Resume Execution**
   - Remove the breakpoint
   - Continue execution (F5)
   - Blue LED should now start toggling every 1000ms

This exercise demonstrates manual task switching by modifying the execution context stored on the stack.

## Troubleshooting

### Common Issues

1. **LEDs not blinking**
   - Check GPIO initialization in `PortF_Init()`
   - Verify SysTick configuration (1,600,000 cycles for 100ms)
   - Ensure proper pin assignments (PF1=Red, PF2=Blue)

2. **Incorrect timing**
   - Verify SysTick reload value: `1600000 - 1` for 16MHz clock
   - Check counter increments in SysTick_Handler
   - Debug timing logic with breakpoints

3. **Manual task switching not working**
   - Ensure breakpoint is set on `counter++;` line
   - Verify stack pointer location in registers window
   - Check disassembly window for correct Blue Task address
   - Modify correct memory location on stack

### Debug Tips
- Use debugger to monitor `systemTick` variable
- Set breakpoints in task functions
- Monitor GPIO register values
- Use Logic Analyzer for precise timing measurement

### Performance Analysis

### System Specifications
- **SysTick Frequency**: 10Hz (100ms intervals)
- **Red Task Period**: 500ms (5 SysTick ticks)
- **Blue Task Period**: 1000ms (10 SysTick ticks)
- **Task Selection**: Compile-time via task variable
- **CPU Utilization**: Minimal (simple LED toggle operations)

### Current Implementation Characteristics
- **No Task Switching**: Only one task runs at compile time
- **Simple Timing**: Uses global counter for timing checks
- **Mutual Exclusion**: Each task ensures other LED is off
- **Infinite Execution**: Selected task runs indefinitely

## Extension Ideas

1. **Add more tasks** with different periods
2. **Implement task priorities** within cooperative framework  
3. **Add button input** for task control
4. **Monitor CPU utilization** using spare cycles
5. **Add task execution time measurement**

## Files Description

- `main.c`: Complete implementation with task scheduler
- `tm4c.h`: TM4C123 register definitions and constants
- `lab1.uvprojx`: Keil µVision project configuration
- `RTE/`: Runtime environment files (auto-generated)

---

**Note**: This implementation demonstrates cooperative scheduling fundamentals. Real-world RTOS systems typically use preemptive scheduling with hardware timer support for better real-time performance.