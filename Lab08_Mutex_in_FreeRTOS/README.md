```markdown
# Lab 08 — Mutexes and Priority Inheritance (FreeRTOS)

## Overview

This folder contains lab material that demonstrates use of mutexes in FreeRTOS and how mutexes (with priority inheritance) prevent priority inversion and deadlocks on the TM4C123GH6PM board.

There are two example tasks/implementations in this lab:
- `Task1/` — baseline examples and notes (has its own README)
- `Task2/` — complete priority-inheritance demo with button-driven deadlock scenario (documented in `Task2/README.md` and `Task2/priority_inheritance_demo.md`)

## Objectives

- Learn how to create and use mutexes in FreeRTOS (`xSemaphoreCreateMutex`, `xSemaphoreTake`, `xSemaphoreGive`).
- Observe priority inheritance when a high-priority task blocks on a mutex held by a low-priority task.
- Practice debugging concurrent issues using the debugger (watch variables) and UART output.

## Hardware / Software

- Hardware: TM4C123GH6PM (Tiva C LaunchPad)
- IDE: Keil µVision 5
- Serial terminal: PuTTY, Tera Term, or similar (115200, 8N1)

## Quick start (Keil)

1. Open `lab8.uvprojx` in the task folder you want to run (`Task1/` or `Task2/`).
2. Build the project (F7).
3. Download/flash to the TM4C123 board.
4. Open a serial terminal at 115200 baud to view UART output.

Notes: Each task folder contains its own `lab8.uvprojx`. Use the project matching the example you want to run.

## Where to look in this folder

- `Task1/` — introductory examples and `README.md` (already present).
- `Task2/` — priority inheritance demo:
  - `main.c` — creates tasks, mutexes, semaphores and a UART mutex.
  - `portf.c` / `portf.h` — button & LED initialization and ISR.
  - `UART0.c` / `UART0.h` — UART helper used for debug output.
  - `priority_inheritance_demo.md` — detailed debugging & simulation guide.
  - `lab8.uvprojx` — Keil project file for Task2.

## Useful pointers

- The demo uses two mutexes `Mutex1` and `Mutex2`, a binary semaphore for the button, and a `UARTMutex` to serialize console output.
- LEDs on Port F are used as visual indicators:
  - PF1 (Red): HighPriorityTask running
  - PF2 (Blue): Deadlock resolved (brief flash)
- To enable/verify priority inheritance, ensure your `FreeRTOSConfig.h` has priority inheritance enabled (see `priority_inheritance_demo.md`).

## Troubleshooting

- If you see no UART output, confirm COM port in Device Manager and PuTTY settings (115200, 8N1, no flow control).
- If the program appears stuck, use the debugger and watch `LowPriorityTaskHandle->uxPriority`, `HighPriorityTaskHandle->eCurrentState`, and the mutex holder fields to inspect priority inheritance and ownership.

---

See the `Task2/README.md` for a concise Task2-specific guide and `Task2/priority_inheritance_demo.md` for an extended debugging walkthrough.

## References

- FreeRTOS mutex docs: https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html
- Priority inheritance: https://www.freertos.org/RTOS-priority-inheritance.html
```
