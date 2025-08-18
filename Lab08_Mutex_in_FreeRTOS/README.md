# Lab 08: Mutex in FreeRTOS

## Overview

This lab introduces mutexes (mutual exclusion objects) in FreeRTOS for protecting shared resources and preventing priority inversion. You will learn how to use mutexes to ensure thread-safe access to shared resources and understand the differences between mutexes and binary semaphores.

## Objectives

- Understand the concept of mutexes in real-time operating systems
- Implement mutex-based resource protection in FreeRTOS
- Learn about priority inheritance and priority inversion
- Compare mutexes with binary semaphores
- Use UART for output and debugging

## Hardware Requirements

- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and serial communication
- Optional: External LEDs for visual feedback

## Software Requirements

- Keil µVision IDE
- PuTTY or similar terminal emulator for serial communication

## Project Structure

- `main.c`: Main application code with mutex implementations
- `portf.c/h`: GPIO Port F driver for buttons and LEDs
- `UART0.c/h`: UART driver for serial communication
- `tm4c123gh6pm.h`: TM4C123 hardware definitions

## Getting Started

1. Open the `lab8.uvprojx` file in Keil µVision
2. Build the project using F7 or the Build button
3. Flash the program to your TM4C123 board
4. Connect to the board via PuTTY to observe the output

> [!NOTE] > **Serial Terminal Required**: This lab uses UART communication with a baud rate of 115200. Configure PuTTY or another terminal emulator to view the output.

## PuTTY Configuration

To properly view the output from this lab:

1. **Install PuTTY**: Download from [https://www.putty.org/](https://www.putty.org/)
2. **Identify COM Port**:
   - Open Device Manager in Windows
   - Look under "Ports (COM & LPT)"
   - Find "Stellaris Virtual Serial Port" or similar TM4C123 entry
   - Note the COM port number (e.g., COM3)
3. **Configure PuTTY**:
   - Connection Type: Serial
   - Serial Line: Your COM port
   - Speed: 115200
   - Data bits: 8, Stop bits: 1, Parity: None
   - Flow control: None
4. **Connect**: Click "Open" to start the serial connection

## Implementation Details

This lab demonstrates:

1. Creating mutexes using `xSemaphoreCreateMutex()`
2. Protecting shared resources with `xSemaphoreTake()` and `xSemaphoreGive()`
3. Understanding priority inheritance mechanisms
4. Comparing mutex behavior with binary semaphores
5. Demonstrating priority inversion scenarios and their solutions

## Expected Behavior

- Tasks access shared resources safely using mutexes
- Priority inheritance prevents priority inversion
- UART output shows mutex operations and task synchronization
- Visual feedback through LEDs (if implemented)

## Key Concepts

### Mutex vs Binary Semaphore

- **Mutex**: Ownership-based, supports priority inheritance, used for mutual exclusion
- **Binary Semaphore**: Count-based, no ownership concept, used for signaling

### Priority Inheritance

- Automatically elevates the priority of a task holding a mutex when a higher-priority task waits for it
- Prevents unbounded priority inversion scenarios

## Troubleshooting

- **No Serial Output**:

  - Verify COM port selection and baud rate (115200)
  - Check UART connections and initialization in code
  - Ensure PuTTY is configured correctly

- **Mutex Issues**:

  - Verify mutex creation before use
  - Ensure proper take/give pairing
  - Check for deadlock scenarios

- **Priority Inversion**:
  - Use mutexes instead of binary semaphores for resource protection
  - Verify priority inheritance is working correctly

## Extending the Lab

Try these extensions to deepen your understanding:

1. Implement multiple mutexes protecting different resources
2. Create scenarios demonstrating priority inversion and its resolution
3. Compare performance between mutex and binary semaphore approaches
4. Add recursive mutex functionality for nested critical sections

## References

- [FreeRTOS Mutex Documentation](https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html)
- [Priority Inheritance in FreeRTOS](https://www.freertos.org/RTOS-priority-inheritance.html)
- [TM4C123 Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [Real-Time Systems Priority Inversion](https://en.wikipedia.org/wiki/Priority_inversion)
