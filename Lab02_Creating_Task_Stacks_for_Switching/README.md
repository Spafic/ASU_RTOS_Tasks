# Lab02: Creating Task Stacks for Switching

## Objective

Create individual task stacks and implement manual context switching using debugger tools. This lab introduces the fundamental concepts of task context storage and stack-based task switching that form the foundation of real-time operating systems.

## Learning Goals

- Understand ARM Cortex-M exception frame layout
- Create and initialize task stacks manually
- Learn stack pointer management and manipulation
- Practice manual context switching using debugger
- Understand how RTOS kernel switching works internally

## Hardware Setup

### Required Components
- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and debugging

### Pin Configuration
- **PF1 (Red LED)**: Output for Task1 (RedBlinky)
- **PF2 (Blue LED)**: Output for Task2 (BlueBlinky)

### Connections
No external connections required - uses on-board LEDs.

## Background

### Why Stack-Based Switching?

In Lab 1, we manually changed the PC register value, which is not a legal or practical approach for real applications. Real-time operating systems need automatic task switching without manual intervention. This requires understanding:

- **Exception Frame Layout**: How ARM Cortex-M stores context during interrupts
- **Stack Management**: Each task needs its own stack space
- **Context Switching**: Saving and restoring complete task state

### ARM Exception Frame Layout (without FPU)

When an interrupt occurs, the ARM Cortex-M automatically pushes registers onto the stack:

```
Higher Memory Address
+------------------+
| xPSR             | <- Status register with THUMB bit
+------------------+
| PC               | <- Program Counter (return address)
+------------------+
| LR               | <- Link Register  
+------------------+
| R12              | <- General purpose register
+------------------+
| R3               | <- General purpose register
+------------------+
| R2               | <- General purpose register
+------------------+
| R1               | <- General purpose register
+------------------+
| R0               | <- General purpose register
+------------------+ <- Stack Pointer (SP) points here
Lower Memory Address
```

**Important**: ARM stack grows downward (from high to low memory addresses).

## Implementation Tasks

### Task 1: Use Lab 1 Code Base

Start with your Lab 1 implementation ensuring you have:
- SysTick timer configured for 100ms interrupts
- Two task functions: one for RED LED, one for BLUE LED
- Basic GPIO initialization for Port F

### Task 2: Create Task Stacks

Create dedicated memory space for each task's stack:

```c
// Task 1 Stack (Red LED)
uint32_t stack_RedBlinky[40];
uint32_t *sp_RedBlinky = &stack_RedBlinky[40];  // Points past end

// Task 2 Stack (Blue LED)  
uint32_t stack_BlueBlinky[40];
uint32_t *sp_BlueBlinky = &stack_BlueBlinky[40]; // Points past end
```

**Key Points:**
- Each stack is 40 words (160 bytes) 
- Stack pointer initialized to point one word past the array end
- Stack grows downward, so we start from the high address

### Task 3: Fill Stacks with Exception Frame Data

In main function, prepare both stacks as if preempted by interrupt:

```c
void Prepare_Task_Stacks(void){
    // ---- Red LED Task Stack ----
    sp_RedBlinky--;                          // Pre-decrement pointer
    *sp_RedBlinky = 0x01000000;             // xPSR with THUMB bit set
    
    sp_RedBlinky--;                          // Pre-decrement for PC
    *sp_RedBlinky = (uint32_t)Task1;        // PC points to task function
    
    sp_RedBlinky--; *sp_RedBlinky = 0x14141414; // LR (test pattern)
    sp_RedBlinky--; *sp_RedBlinky = 0x12121212; // R12 (test pattern)
    sp_RedBlinky--; *sp_RedBlinky = 0x03030303; // R3 (test pattern)
    sp_RedBlinky--; *sp_RedBlinky = 0x02020202; // R2 (test pattern)
    sp_RedBlinky--; *sp_RedBlinky = 0x01010101; // R1 (test pattern)
    sp_RedBlinky--; *sp_RedBlinky = 0x00000000; // R0 (test pattern)
    
    // ---- Blue LED Task Stack ----
    sp_BlueBlinky--;
    *sp_BlueBlinky = 0x01000000;            // xPSR with THUMB bit
    
    sp_BlueBlinky--;
    *sp_BlueBlinky = (uint32_t)Task2;       // PC points to task function
    
    sp_BlueBlinky--; *sp_BlueBlinky = 0x25252525; // LR (different pattern)
    sp_BlueBlinky--; *sp_BlueBlinky = 0x12121212; // R12
    sp_BlueBlinky--; *sp_BlueBlinky = 0x03030303; // R3
    sp_BlueBlinky--; *sp_BlueBlinky = 0x02020202; // R2
    sp_BlueBlinky--; *sp_BlueBlinky = 0x01010101; // R1
    sp_BlueBlinky--; *sp_BlueBlinky = 0x00000000; // R0
}
```

### Task 4: Debug and Verify Stack Contents

#### Step 1: Verify Stack Memory Layout
- Open **Memory Window** in debugger
- Navigate to stack addresses to verify correct values
- Check that xPSR = 0x01000000 and PC points to task functions

#### Step 2: Setup Watch Window
- Add `sp_RedBlinky` to watch window
- Add `sp_BlueBlinky` to watch window  
- Monitor stack pointer values during debugging

#### Step 3: Manual Task Switching to Task1
1. Set breakpoint in `SysTick_Handler()`
2. When breakpoint hits, open **Registers** window
3. Manually set **SP register** to `sp_RedBlinky` value
4. Remove breakpoint and continue execution
5. **Result**: Should jump to Task1 (Red LED function)

#### Step 4: Switch to Task2 (Advanced)
1. Set breakpoint in `SysTick_Handler()` again
2. **Save current context**: Copy current SP value to `sp_RedBlinky`
3. **Load new context**: Set SP register to `sp_BlueBlinky` value  
4. Continue execution
5. **Result**: Should switch to Task2 (Blue LED function)

## Expected Behavior

### Normal Operation
- **Task1**: Red LED blinks every 500ms when active
- **Task2**: Blue LED blinks every 1000ms when active
- **Switching**: Manual via debugger SP register modification
- **Timing**: Maintained by SysTick 100ms interrupts

### Debug Screenshots Required
1. **Screenshot_1**: Memory view showing stack contents and watch window with stack pointers
2. **Screenshot_2**: Debugger after manually setting SP to Task1, showing execution in RedBlinky function
3. **Screenshot_3**: Debugger showing successful switch to Task2 (BlueBlinky function)

## Key Code Structure

### Main Function
```c
int main(void){
    PortF_Init();                    // Initialize GPIO
    SysTick_Init();                  // Configure 100ms timer
    Prepare_Task_Stacks();           // Setup both task stacks
    
    while(1){
        // Infinite loop - switching done manually via debugger
    }
}
```

### Task Implementation
```c
void Task1(void){  // Red LED Task
    uint32_t lastToggleTime = 0;
    GPIO_PORTF_DATA_R &= ~RED_LED;   // Initialize OFF
    
    for(;;){  // Task infinite loop
        if((counter - lastToggleTime) >= 5){  // 500ms timing
            lastToggleTime = counter;
            GPIO_PORTF_DATA_R &= ~BLUE_LED;   // Ensure other LED OFF
            GPIO_PORTF_DATA_R ^= RED_LED;     // Toggle Red LED
        }
    }
}
```

## Building and Testing

### Using Keil µVision
1. Open project file
2. Build project (F7)
3. Start debugging (Ctrl+F5)
4. Follow Task 4 debugging procedure
5. Take required screenshots
6. Verify task switching works correctly

### Debug Configuration
- **FPU**: Must be disabled (matches exception frame layout)
- **Optimization**: Disable for easier debugging
- **Debug Info**: Enable full symbols

## Troubleshooting

### Common Issues

1. **System Hangs After SP Change**
   - Verify THUMB bit is set in xPSR (0x01000000)
   - Check PC points to valid task function address
   - Ensure stack pointer is properly aligned

2. **Tasks Don't Execute Properly**
   - Verify task functions have infinite loops (`for(;;)`)
   - Check SysTick interrupt is running (`counter` incrementing)
   - Ensure GPIO initialization completed successfully

3. **Stack Preparation Errors**
   - Verify pre-decrement operations (`--pointer` before assignment)
   - Check stack grows downward (high to low addresses)
   - Ensure sufficient stack space (40 words minimum)

### Debug Tips
- Use memory window to inspect stack contents visually
- Monitor register values during context switch
- Verify exception frame matches ARM documentation
- Check stack alignment (8-byte boundary)

## Educational Value

This lab demonstrates:
- **Foundation Concepts**: How RTOS task switching works internally
- **Hardware Understanding**: ARM Cortex-M exception handling mechanism
- **Debug Skills**: Advanced debugger usage for system analysis
- **Memory Management**: Stack allocation and pointer manipulation

## Next Steps

Future labs will build on these concepts to implement:
- Automatic context switching
- Task scheduling algorithms  
- Complete RTOS kernel functionality
- Hardware-based task switching

## Files Description

- `main.c`: Complete implementation with manual stack-based switching
- `tm4c.h`: TM4C123 register definitions
- Project files: Keil µVision configuration

---

**Note**: This lab provides hands-on experience with the low-level mechanisms that real-time operating systems use for task management. Understanding these fundamentals is essential for embedded systems development.