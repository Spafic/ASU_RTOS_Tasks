# Lab03: Introduction to FreeRTOS

## Objective

Introduce students to FreeRTOS (Free Real-Time Operating System) fundamentals by implementing a simple task-based LED control system. This lab transitions from manual task switching to using a real RTOS kernel for task management and scheduling.

## Learning Goals

- Understand FreeRTOS architecture and core concepts
- Learn task creation and management using FreeRTOS APIs
- Configure FreeRTOS for TM4C123GH6PM microcontroller
- Implement task delays using vTaskDelay()
- Compare RTOS-based scheduling vs. manual task switching
- Understand preemptive scheduling benefits

## Hardware Setup

### Required Components

- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and debugging

### Pin Configuration

- **PF1 (Red LED)**: Output for FreeRTOS task demonstration

### Connections

No external connections required - uses on-board LED.

## Background

### What is FreeRTOS?

FreeRTOS is a real-time operating system kernel for embedded devices that provides:

- **Task Management**: Create, delete, and manage multiple tasks
- **Scheduling**: Preemptive priority-based task scheduling
- **Inter-task Communication**: Queues, semaphores, and mutexes
- **Memory Management**: Dynamic memory allocation
- **Timing Services**: Software timers and delays

### Key Advantages over Manual Switching

Compared to Labs 1 and 2, FreeRTOS provides:

1. **Automatic Task Switching**: No manual debugger intervention required
2. **Preemptive Scheduling**: Higher priority tasks can interrupt lower priority ones
3. **Precise Timing**: Built-in delay functions with tick-accurate timing
4. **Resource Management**: Automatic stack management for each task
5. **Scalability**: Easy to add more tasks without complex manual management

## System Architecture

### FreeRTOS Configuration

The system uses the following FreeRTOS configuration:

```c
// Key FreeRTOSConfig.h settings
#define configCPU_CLOCK_HZ                    (SystemCoreClock)
#define configTICK_RATE_HZ                    ((TickType_t)1000)     // 1ms tick
#define configTOTAL_HEAP_SIZE                 ((size_t)10*1024)      // 10KB heap
#define configMINIMAL_STACK_SIZE              ((uint16_t)256)        // 256 words
#define configMAX_PRIORITIES                  5                      // Priority levels 0-4
#define configUSE_PREEMPTION                  1                      // Enable preemption
#define configUSE_TIME_SLICING                1                      // Enable time slicing
```

### Task Structure

```c
void Task1(void *pvParameters) {
    for( ; ; ) {
        ToggleRedLED();                          // Toggle LED
        vTaskDelay(pdMS_TO_TICKS(1000));        // Delay 1000ms
    }
}
```

**Key Components:**

- **Infinite Loop**: Task runs continuously
- **vTaskDelay()**: Non-blocking delay that yields CPU to other tasks
- **pdMS_TO_TICKS()**: Converts milliseconds to FreeRTOS ticks

## Implementation Details

### Core Components

1. **FreeRTOS Initialization**

   ```c
   int main(void) {
       PortF_Init();                               // Initialize GPIO

       // Create task with priority 1
       xTaskCreate(Task1, "Red LED Task", configMINIMAL_STACK_SIZE,
                   NULL, 1, NULL);

       vTaskStartScheduler();                      // Start FreeRTOS scheduler

       while (1) {}                               // Should never reach here
   }
   ```

2. **Task Implementation**

   ```c
   void Task1(void *pvParameters) {
       for( ; ; ) {
           ToggleRedLED();                        // Toggle Red LED
           vTaskDelay(pdMS_TO_TICKS(1000));      // Wait 1000ms
       }
   }
   ```

3. **GPIO Control**
   ```c
   void ToggleRedLED(void) {
       GPIO_PORTF_DATA_R ^= RED_LED;              // XOR to toggle
   }
   ```

### Scheduler Operation

1. **Task Creation**: `xTaskCreate()` allocates stack and creates task control block
2. **Scheduler Start**: `vTaskStartScheduler()` begins task execution
3. **Task Execution**: Task runs and toggles LED
4. **Task Delay**: `vTaskDelay()` blocks task and yields CPU
5. **Context Switch**: Scheduler switches to idle task during delay
6. **Task Resume**: After delay expires, task becomes ready to run again

## Expected Behavior

### Normal Operation

- **Red LED**: Blinks every 1000ms (1 second intervals)
- **Timing**: Precise 1-second on/off cycles
- **CPU Utilization**: Task runs briefly, then CPU idles during delay
- **Scheduler**: Automatic task management without manual intervention

### Timing Characteristics

- **Period**: 2000ms (1000ms ON + 1000ms OFF)
- **Duty Cycle**: 50% (equal ON/OFF times)
- **Accuracy**: ±1ms (determined by FreeRTOS tick rate)
- **Jitter**: Minimal due to RTOS deterministic scheduling

## Key Differences from Previous Labs

| Aspect               | Lab01 (Manual)                    | Lab02 (Stack-based)             | Lab03 (FreeRTOS)            |
| -------------------- | --------------------------------- | ------------------------------- | --------------------------- |
| **Task Switching**   | Manual variable change            | Manual debugger SP modification | Automatic by scheduler      |
| **Timing Control**   | SysTick counter checks            | SysTick counter checks          | vTaskDelay() API            |
| **Stack Management** | Single stack                      | Manual stack creation           | Automatic per task          |
| **Scalability**      | Limited to compile-time selection | Limited to 2 tasks              | Easy multiple task addition |
| **Preemption**       | None (cooperative)                | None (manual)                   | Full preemptive support     |
| **Resource Usage**   | Minimal                           | Medium                          | Higher (RTOS overhead)      |

## Building and Running

### Using Keil µVision

1. Open `lab3.uvprojx`
2. Ensure FreeRTOS is properly configured in RTE
3. Build project (F7)
4. Connect LaunchPad via USB
5. Flash and debug (Ctrl+F5)

### Debugging FreeRTOS

#### Task Information

- Use **Call Stack** window to see current task
- Monitor **RTX** windows for task states
- Check **Memory** window for heap usage

#### Common Debug Points

- Set breakpoint in `Task1()` to verify task execution
- Monitor `xTaskCreate()` return value for successful task creation
- Check that `vTaskStartScheduler()` is called and doesn't return

## Troubleshooting

### Common Issues

1. **LED Doesn't Blink**

   - Verify FreeRTOS configuration is correct
   - Check that `vTaskStartScheduler()` is called
   - Ensure task creation was successful
   - Verify GPIO initialization

2. **System Hangs or Crashes**

   - Check heap size configuration (`configTOTAL_HEAP_SIZE`)
   - Verify stack size is sufficient (`configMINIMAL_STACK_SIZE`)
   - Ensure all FreeRTOS configuration parameters are valid
   - Check for stack overflow

3. **Incorrect Timing**
   - Verify `configTICK_RATE_HZ` is set to 1000 (1ms tick)
   - Check `configCPU_CLOCK_HZ` matches actual system clock
   - Ensure `pdMS_TO_TICKS()` macro is used correctly

### Debug Tips

- Enable FreeRTOS assertions for error detection
- Use FreeRTOS trace facility for task analysis
- Monitor task states using RTOS-aware debugger features
- Check return values from FreeRTOS API calls

## FreeRTOS Concepts Demonstrated

### Task Management

- **Task Creation**: Using `xTaskCreate()` API
- **Task Execution**: Infinite loop task pattern
- **Task Parameters**: Understanding function signature

### Timing Services

- **Blocking Delays**: `vTaskDelay()` usage
- **Tick Conversion**: `pdMS_TO_TICKS()` macro
- **Non-blocking Design**: Yielding CPU during delays

### Scheduler Operation

- **Preemptive Scheduling**: Ready tasks run immediately
- **Idle Task**: Automatic low-power operation
- **Context Switching**: Transparent task switching

## Extension Ideas

1. **Add Second Task**: Create blue LED task with different timing
2. **Task Priorities**: Experiment with different priority levels
3. **Task Communication**: Use queues to pass data between tasks
4. **Error Handling**: Add proper error checking for API calls
5. **Task Deletion**: Implement dynamic task creation/deletion

## Files Description

- `main.c`: Complete FreeRTOS implementation with single task
- `tm4c.h`: TM4C123 register definitions and constants
- `lab3.uvprojx`: Keil µVision project configuration
- `FreeRTOSConfig.h`: FreeRTOS kernel configuration settings
- `RTE/`: Runtime environment files (auto-generated)

## Performance Analysis

### Memory Usage

- **Task Stack**: 256 words (1024 bytes) per task
- **Heap Usage**: ~1KB for task control blocks and kernel data
- **Code Size**: ~15KB additional for FreeRTOS kernel
- **RAM Usage**: ~2KB total for single task system

### Timing Performance

- **Task Switch Time**: <10µs typical
- **Interrupt Latency**: <5µs for highest priority
- **Tick Resolution**: 1ms (configurable)
- **CPU Overhead**: <5% for scheduler in simple systems

---

**Note**: This lab introduces the fundamental concepts of real-time operating systems. FreeRTOS provides a solid foundation for more complex embedded applications requiring multiple concurrent tasks with deterministic timing requirements.
