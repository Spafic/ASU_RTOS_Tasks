# Lab 07: Semaphores and Queues in FreeRTOS

## Overview

This lab demonstrates the implementation of semaphores and queues in FreeRTOS for task synchronization and data communication. You will learn how to use counting semaphores for signaling between tasks and queues for transferring data between tasks.

## Objectives

- Understand the concept of semaphores and queues in real-time operating systems
- Implement counting semaphores in FreeRTOS
- Use queues for inter-task communication
- Synchronize tasks and transfer data safely
- Configure and use UART for output

## Hardware Requirements

- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and serial communication
- Push buttons on the development board (SW1 connected to PF0)

## Software Requirements

- Keil µVision IDE
- PuTTY or similar terminal emulator for serial communication

## Project Structure

- `main.c`: Main application code with semaphore and queue implementations
- `portF.c/h`: GPIO Port F driver for button input
- `UART0.c/h`: UART driver for serial communication
- `tm4c123gh6pm.h`: Hardware definitions for the TM4C123 microcontroller

## Implementation Details

### Initialization

The system initializes two FreeRTOS objects:
1. **Queue**: An 8-element queue to transmit data between tasks
2. **Counting Semaphore**: A semaphore with maximum count of 5, used to synchronize task execution

```c
xQueue = xQueueCreate(8, sizeof(uint8_t));
xCountingSemaphore = xSemaphoreCreateCounting(5, 0);
```

### Tasks

Two tasks are created and run concurrently:

1. **BTN_CHK_TASK**: 
   - Monitors button SW1 (PF0)
   - When button is pressed:
     - Increments a counter
     - Sends the counter value to the queue
     - Gives a semaphore to signal the UART task
     - Includes debounce delay (100ms)

2. **UART_TASK**:
   - Waits for semaphore from the button task
   - When semaphore is received:
     - Reads a value from the queue
     - Transmits the value to UART
     - Sends carriage return and newline characters

### Hardware Configuration

- **PortF**: Configured with PF0 as input with pull-up resistor for button
- **UART0**: Configured with 115200 baud rate for serial communication

## Getting Started

1. Open the `lab7.uvprojx` file in Keil µVision
2. Build the project (F7)
3. Flash the program to your TM4C123 board
4. Connect to the board via serial terminal (115200 baud rate)
5. Press SW1 (PF0) button to see incremented counter values in the terminal

## Expected Behavior

- Each press of SW1 increments a counter and displays its value on the terminal
- Counter values are transmitted via UART with proper carriage return and line feed
- The system demonstrates proper synchronization between button input and UART output

## Terminal Configuration

For serial output (115200 baud rate):

1. **Install a terminal program**: PuTTY, TeraTerm, or the built-in terminal in your IDE
2. **Configure connection**:
   - Port: Check Device Manager for your board's COM port
   - Baud rate: 115200
   - Data bits: 8
   - Stop bits: 1
   - Parity: None
   - Flow control: None

## Troubleshooting

- **No button response**: Check PortF initialization and pull-up resistor configuration
- **No serial output**: Verify UART initialization, baud rate settings, and COM port connection
- **Queue overflow**: The queue can hold up to 8 elements; rapid button presses could exceed this limit
- **Inconsistent output**: Check debounce timing; adjust if needed

## Code Analysis

This implementation demonstrates key FreeRTOS concepts:

1. **Task Synchronization**: Using counting semaphores to coordinate actions between tasks
2. **Data Transfer**: Using queues to safely transfer data between tasks
3. **Task Scheduling**: Tasks running concurrently with appropriate priorities and delays
4. **Hardware Abstraction**: Separating hardware interface code (UART, GPIO) from application logic

## Extending the Lab

- Add priority inheritance to handle priority inversion
- Implement binary semaphores for resource protection
- Add multiple button inputs with different semaphores
- Extend the UART functionality to handle input commands

## References

- [FreeRTOS Semaphore Documentation](https://www.freertos.org/a00113.html)
- [FreeRTOS Queue Documentation](https://www.freertos.org/a00018.html)
- [TM4C123 Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [Serial Communication Basics](https://www.analog.com/en/analog-dialogue/articles/uart-a-hardware-communication-protocol.html)
