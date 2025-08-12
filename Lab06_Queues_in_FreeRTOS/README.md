# Lab 06: Queues in FreeRTOS

## Overview

This lab focuses on inter-task communication using FreeRTOS queues. You will learn how to create, send to, and receive from queues to facilitate communication between different tasks in a real-time operating system.

## Objectives

- Understand the concept of queues in real-time operating systems
- Implement queue-based communication between tasks
- Configure and use UART for displaying output from the microcontroller
- Work with GPIO inputs to trigger inter-task communication

## Hardware Requirements

- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and serial communication
- Optional: External push buttons or sensors for input

## Software Requirements

- Keil µVision IDE
- PuTTY or similar terminal emulator for serial communication

## Project Structure

- `main.c`: Main application code with task definitions and queue implementations
- `portF.c/h`: GPIO Port F driver for onboard buttons and LEDs
- `UART0.c/h`: UART driver for serial communication
- `tm4c.h`: TM4C123 hardware definitions

## Getting Started

1. Open the `lab6.uvprojx` file in Keil µVision
2. Build the project using F7 or the Build button
3. Flash the program to your TM4C123 board
4. Connect to the board via PuTTY to observe the output

> [!NOTE] 
> **Serial Terminal Required**: This lab uses UART communication with a baud rate of 115200. Configure PuTTY or another terminal emulator to view the output.

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

1. Creating queues using `xQueueCreate()`
2. Sending data to queues with `xQueueSend()` or `xQueueSendToBack()`
3. Receiving data from queues with `xQueueReceive()`
4. Configuring tasks with appropriate priorities for queue communication
5. Using UART for debugging and demonstrating queue functionality

## Expected Behavior

- The application creates multiple tasks that communicate through queues
- Input from buttons (GPIO) generates data that is sent to queues
- Receiver tasks process the queued data and display results via UART
- The terminal shows the flow of data between tasks

## Troubleshooting

- **No Serial Output**:
  - Verify COM port selection and baud rate (115200)
  - Check UART connections and initialization in code
  - Ensure PuTTY is configured correctly
- **Queue Issues**:
  - Check that queue handles are valid and not NULL
  - Verify that queue send and receive operations have appropriate timeout values
  - Ensure queue size is adequate for your application

## Extending the Lab

Try these extensions to deepen your understanding:

1. Implement multiple queues for different types of data
2. Add queue peek functionality without removing items
3. Experiment with different queue sizes and timeout values
4. Implement priority queuing by combining queues with task priorities

## References

- [FreeRTOS Queue Documentation](https://www.freertos.org/Embedded-RTOS-Queues.html)
- [TM4C123 Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [Serial Communication Basics](https://www.analog.com/en/analog-dialogue/articles/uart-a-hardware-communication-protocol.html)
