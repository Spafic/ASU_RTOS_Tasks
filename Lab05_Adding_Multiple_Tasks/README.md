# Lab 05: Adding Multiple Tasks

## Overview

This lab builds upon previous labs by focusing on managing multiple tasks in a FreeRTOS environment. You will learn how to create, prioritize, and schedule multiple tasks to work together effectively in a real-time operating system.

## Objectives

- Create and manage multiple tasks in a FreeRTOS environment
- Understand task priorities and their effect on scheduling
- Implement task synchronization techniques
- Monitor task performance and resource usage
- Practice effective multi-tasking design patterns

## Hardware Requirements

- TM4C123GH6PM LaunchPad Evaluation Kit
- USB cable for programming and debugging
- Optional: Additional LEDs or peripherals for visual feedback

## Software Requirements

- Keil µVision IDE
- FreeRTOS kernel (included in the project)

## Project Structure

- `main.c`: Main application code with task definitions and initialization
- `tm4c123gh6pm.h`: TM4C123 hardware definitions
- `RTE/`: Run-Time Environment folder containing FreeRTOS configuration

## Getting Started

1. Open the `lab5.uvprojx` file in Keil µVision
2. Review the main.c file to understand the task structure
3. Build the project using F7 or the Build button
4. Flash the program to your TM4C123 board
5. Observe the behavior of multiple tasks running concurrently

## Implementation Details

This lab demonstrates:

1. Creating multiple tasks with different priorities using `xTaskCreate()`
2. Controlling task execution through priorities
3. Managing shared resources between tasks
4. Implementing periodic tasks with different execution frequencies
5. Using task utilities such as `vTaskDelay()` and `vTaskDelayUntil()`

## Expected Behavior

- Multiple tasks execute according to their assigned priorities
- Higher priority tasks preempt lower priority tasks when needed
- Tasks demonstrate cooperation by yielding control when appropriate
- The application shows predictable real-time behavior

## Synchronization Methods

This lab explores various ways tasks can be synchronized:

1. **Priority-based Preemption**: Higher priority tasks interrupt lower priority tasks
2. **Time Slicing**: Tasks of equal priority share CPU time
3. **Explicit Yielding**: Tasks voluntarily give up processor time
4. **Timed Delays**: Tasks suspend themselves for specific time periods

## Task Management Best Practices

- Assign priorities based on task importance and deadlines
- Keep critical sections short to avoid blocking other tasks
- Use appropriate delay mechanisms for periodic tasks
- Be mindful of stack usage for each task
- Consider using task notifications for simple task-to-task signaling

## Troubleshooting

- **Tasks Not Running**: Verify priority assignments and ensure the scheduler is started
- **Stack Overflow**: Increase stack size for tasks that use more stack space
- **Timing Issues**: Check delay calculations and timer tick configuration
- **System Hangs**: Look for potential deadlocks or priority inversion situations

## Extending the Lab

Try these extensions to deepen your understanding:

1. Add more tasks with different behaviors and priorities
2. Implement a task that monitors and reports on other tasks' performance
3. Experiment with different scheduling policies by modifying FreeRTOSConfig.h
4. Add resource sharing between tasks with proper protection mechanisms

## References

- [FreeRTOS Task Management](https://www.freertos.org/a00015.html)
- [TM4C123 Datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
- [Task Priorities and Scheduling in FreeRTOS](https://www.freertos.org/RTOS-task-priority.html)
