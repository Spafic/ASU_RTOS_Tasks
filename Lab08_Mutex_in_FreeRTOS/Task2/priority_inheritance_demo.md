# Priority Inheritance Demo: Debugging and Simulation Guide

## Overview

This guide provides step-by-step instructions for simulating and debugging the priority inheritance mechanism in FreeRTOS using the TM4C123GH6PM microcontroller. The demo creates a classic deadlock scenario between two tasks of different priorities and demonstrates how priority inheritance resolves it.

## Setup

### Hardware Requirements
- TM4C123GH6PM board (Tiva C Series)
- USB Cable for programming and serial communication
- Serial Terminal (115200 baud rate)

### Software Requirements
- Keil μVision 5
- FreeRTOS configuration with priority inheritance enabled

## Conceptual Understanding

### Deadlock Scenario
In our demo:
1. **LowPriorityTask (Priority 1)** acquires Mutex1, then attempts to acquire Mutex2
2. **HighPriorityTask (Priority 3)** acquires Mutex2, then attempts to acquire Mutex1
3. This creates a classic deadlock: each task holds a resource the other needs

### Priority Inheritance Solution
Without priority inheritance, the system would remain deadlocked. With priority inheritance:
1. When HighPriorityTask waits for Mutex1 (held by LowPriorityTask)
2. LowPriorityTask temporarily inherits Priority 3
3. LowPriorityTask completes its execution without being preempted
4. LowPriorityTask releases Mutex1
5. HighPriorityTask acquires Mutex1 and continues
6. Deadlock is resolved

## Debugging Steps

### Step 1: Set Up Breakpoints

Set breakpoints at these key locations:

1. **In LowPriorityTask:**
   ```c
   // Line where LowPriorityTask attempts to take Mutex2
   Write_String((unsigned char *)"Low Priority Task: Attempting to take Mutex2\n");
   ```

2. **In HighPriorityTask:**
   ```c
   // Line where HighPriorityTask attempts to take Mutex1
   Write_String((unsigned char *)"High Priority Task: Attempting to take Mutex1\n");
   ```

3. **Additional breakpoint in LowPriorityTask:**
   ```c
   // After acquiring Mutex1
   Write_String((unsigned char *)"Low Priority Task: Acquired Mutex1\n");
   ```

### Step 2: Set Up Watch Variables

Add these variables to your Watch window:

1. `LowPriorityTaskHandle->uxPriority` - To observe priority changes
2. `Mutex1->pxMutexHolder` - To see which task holds Mutex1
3. `Mutex2->pxMutexHolder` - To see which task holds Mutex2
4. `HighPriorityTaskHandle->eCurrentState` - To see the state of the high priority task
5. `LowPriorityTaskHandle->eCurrentState` - To see the state of the low priority task

### Step 3: Enable Priority Inheritance in FreeRTOS

Ensure this line is added to your FreeRTOSConfig.h file:
```c
#define configUSE_MUTEXES_PRIORITY_INHERITANCE 1
```

### Step 4: Run the Simulation

1. **Build and download** the code to the board
2. **Start debugging** in Keil μVision
3. **Run to first breakpoint** - LowPriorityTask acquires Mutex1
4. **Check watch variables**:
   - `LowPriorityTaskHandle->uxPriority` should be 1
   - `Mutex1->pxMutexHolder` should point to LowPriorityTask

5. **Press the button** (or simulate it in debugging) to trigger HighPriorityTask
6. **Run to next breakpoint** - HighPriorityTask acquires Mutex2
7. **Check watch variables**:
   - `Mutex2->pxMutexHolder` should point to HighPriorityTask

8. **Continue to next breakpoint** - HighPriorityTask attempts to take Mutex1
9. **Observe task state**:
   - HighPriorityTask should enter Blocked state

10. **Step or continue** to where LowPriorityTask attempts to take Mutex2
11. **Observe priority inheritance**:
    - `LowPriorityTaskHandle->uxPriority` should now be 3 (inherited from HighPriorityTask)
    - This is the key moment where priority inheritance prevents deadlock

12. **Continue execution** to observe the resolution:
    - LowPriorityTask will continue execution (with priority 3)
    - LowPriorityTask will release Mutex1
    - HighPriorityTask will acquire Mutex1 and complete its execution
    - The Blue LED will flash, indicating deadlock resolution

## Expected Output

### Terminal Output Sequence

You should see output similar to this in your UART terminal, though there may be some overlapping/corrupted lines due to both tasks trying to write to UART simultaneously:

```
High Priority Task Started
Low Priority Task Started
Low Priority Task: Starting new cycle
Low Priority Task: Attempting to take Mutex1
Low Priority Task: Acquired Mutex1
Low Priority Task: Attempting to take Mutex2
Low Priority Task: Acquired both mutexes
Low Priority Task: Released Mutex2
Low Priority Task: Released Mutex1

High Priority Task: Button pressed, starting deadlock sequence
High Priority Task: Attempting to take Mutex2
High Priority Task: Acquired Mutex2
High Priority Task: Attempting to take Mutex1

[Some lines may appear corrupted here due to task switching and UART contention]

Low Priority Task: Starting new cycle
Low Priority Task: Attempting to take Mutex1
Low Priority Task: Acquired Mutex1
Low Priority Task: Attempting to take Mutex2

[Priority inheritance happens here - may not be visible in output]

Low Priority Task: Released Mutex1

High Priority Task: Acquired both mutexes
High Priority Task: Released Mutex1
High Priority Task: Released Mutex2
```

> [!NOTE]
> You may see corrupted lines like Low Priority High PrioriTask: Releasty Task: Aced Mutex2. This is normal and happens when both tasks attempt to write to UART simultaneously. The actual task execution is still correct - it's just the debug output that gets mixed up. This is a common issue in multitasking systems when the UART driver isn't protected by a mutex.

### Visual Indicators

- **Red LED (PF1)** turns on when HighPriorityTask is running
- **Blue LED (PF2)** flashes briefly when deadlock is resolved

## Debugging Observations

### UART Output Issues

You may notice corrupted or interleaved output in the terminal, such as:
```
Low Priority High PrioriTask: Releasty Task: Aced Mutex2
quired Mutex2
```

This occurs because:
1. Multiple tasks are writing to UART without synchronization
2. Task switching happens during UART writes
3. The UART peripheral itself isn't protected by a mutex

**This does not affect the actual execution of the tasks or the priority inheritance mechanism.** It only affects the debug output. For cleaner output, you could:

1. Use a separate mutex just for UART operations
2. Focus on the LED indicators and debugger variables instead of UART output
3. Add small delays between characters in the Write_String function

### Without Priority Inheritance
If you were to disable priority inheritance (`configUSE_MUTEXES_PRIORITY_INHERITANCE 0`), you would observe:
- Both tasks remain blocked indefinitely
- HighPriorityTask waiting for Mutex1
- LowPriorityTask waiting for Mutex2
- System deadlock with no resolution

### With Priority Inheritance
With priority inheritance enabled:
1. LowPriorityTask's priority temporarily increases to 3
2. LowPriorityTask continues execution without being preempted
3. LowPriorityTask releases Mutex1, allowing HighPriorityTask to proceed
4. LowPriorityTask's priority returns to 1 after HighPriorityTask no longer waits for Mutex1
5. Both tasks complete their execution

## What to Look For in the Debugger

### Verifying Priority Inheritance

Since the UART output might be unreliable, the most definitive way to verify priority inheritance is through the debugger:

1. **Set a breakpoint** in the LowPriorityTask right after this line:
   ```c
   Write_String((unsigned char *)"Low Priority Task: Attempting to take Mutex2\n");
   ```

2. **When this breakpoint hits** during the deadlock scenario:
   - Immediately check `LowPriorityTaskHandle->uxPriority` in the Watch window
   - If it shows a value of 3 (instead of the original 1), priority inheritance is working
   - You can also check `HighPriorityTaskHandle->eCurrentState` to confirm it's blocked

3. **Additional verification**:
   - In FreeRTOS, the mutex structure has a field that points to the task inheriting priority
   - Check `Mutex1->pxMutexHolder` to see which task holds Mutex1
   - If `xSemaphoreGetMutexHolder(Mutex1)` equals LowPriorityTaskHandle, priority inheritance is occurring

4. **Timing verification**:
   - Without priority inheritance: The system would remain deadlocked
   - With priority inheritance: The deadlock resolves within a few seconds

### Priority Changes
The most important observation is the priority change of LowPriorityTask:
- Initially: `LowPriorityTaskHandle->uxPriority` = 1
- During deadlock resolution: `LowPriorityTaskHandle->uxPriority` = 3
- After resolution: `LowPriorityTaskHandle->uxPriority` = 1

### Task States
Watch how task states change:
- `eBlocked`: Task is waiting for a resource
- `eReady`: Task is ready to run but not running
- `eRunning`: Task is currently running

### Mutex Ownership
Observe how mutex ownership changes during execution:
- Initially: Mutex1 owned by LowPriorityTask, Mutex2 unowned
- After button press: Mutex1 owned by LowPriorityTask, Mutex2 owned by HighPriorityTask
- After resolution: Both mutexes released

## Conclusion

This demonstration shows how FreeRTOS's priority inheritance mechanism prevents deadlocks in real-time systems. By temporarily boosting the priority of a lower-priority task when a higher-priority task is waiting for a resource it holds, the system ensures that critical sections complete quickly and resources are released promptly.

## Code Improvements (Optional)

If you want cleaner UART output, consider implementing these improvements:

### 1. Add a UART Mutex

```c
// At the global scope, add:
SemaphoreHandle_t UARTMutex;

// In InitTask(), add:
UARTMutex = xSemaphoreCreateMutex();

// Create a new function for synchronized UART writing:
void Write_String_Protected(unsigned char *str) {
    if (xSemaphoreTake(UARTMutex, portMAX_DELAY) == pdTRUE) {
        Write_String(str);
        xSemaphoreGive(UARTMutex);
    }
}

// Use Write_String_Protected() instead of Write_String() in all tasks
```

### 2. Add Task Identification to Messages

```c
// Create helper functions to identify which task is printing:
void LowPriorityTask_Print(const char *msg) {
    char buffer[100];
    sprintf(buffer, "[LP Task] %s (Priority: %lu)\n", 
            msg, uxTaskPriorityGet(NULL));
    Write_String_Protected((unsigned char *)buffer);
}

void HighPriorityTask_Print(const char *msg) {
    char buffer[100];
    sprintf(buffer, "[HP Task] %s (Priority: %lu)\n", 
            msg, uxTaskPriorityGet(NULL));
    Write_String_Protected((unsigned char *)buffer);
}
```

These improvements will make the output easier to follow and will explicitly show the priority changes, but they're not necessary for the core demonstration of priority inheritance.
