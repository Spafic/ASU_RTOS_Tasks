# Lab 07: Semaphores in FreeRTOS

## Overview

This lab introduces the use of semaphores in FreeRTOS for task synchronization and resource management. You will learn how to use binary and counting semaphores to coordinate tasks and protect shared resources.

## Objectives

- Understand the concept of semaphores in real-time operating systems
- Implement binary and counting semaphores in FreeRTOS
- Synchronize tasks and manage shared resources
- Use UART for output and debugging

## Hardware Requirements

- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and serial communication

## Software Requirements

- Keil µVision IDE
- PuTTY or similar terminal emulator for serial communication

## Project Structure

- `main.c`: Main application code with semaphore implementations
- `portF.c/h`: GPIO Port F driver
- `UART0.c/h`: UART driver
- `tm4c123gh6pm.h`: Hardware definitions

## Getting Started

1. Open the `lab7.uvprojx` file in Keil µVision
2. Build the project (F7)
3. Flash the program to your TM4C123 board
4. Connect to the board via PuTTY to observe output

> [!NOTE]
> **Serial Terminal Required**: This lab uses UART communication with a baud rate of 115200. Configure PuTTY or another terminal emulator to view the output.

## PuTTY Configuration

To view UART output:

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

- Create binary and counting semaphores using `xSemaphoreCreateBinary()` and `xSemaphoreCreateCounting()`
- Use `xSemaphoreTake()` and `xSemaphoreGive()` for synchronization
- Demonstrate task coordination and resource protection
- Output results and debug info via UART

## Expected Behavior

- Tasks synchronize using semaphores
- Shared resources are accessed safely
- UART output shows semaphore operations and task status

## Troubleshooting

- **No Serial Output**: Check COM port and baud rate (115200), verify UART initialization
- **Semaphore Issues**: Ensure semaphores are created before use, check for deadlocks

## Extending the Lab

- Implement priority inheritance
- Use semaphores for event signaling
- Experiment with multiple semaphores and tasks

## References

- [FreeRTOS Semaphore Documentation](https://www.freertos.org/a00113.html)
- [TM4C123 Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [Serial Communication Basics](https://www.analog.com/en/analog-dialogue/articles/uart-a-hardware-communication-protocol.html)
