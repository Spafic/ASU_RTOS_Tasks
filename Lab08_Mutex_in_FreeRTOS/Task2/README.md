# Task2 — Priority Inheritance Demo (concise)

This README summarizes the purpose and how to run/debug the `Task2` demo that demonstrates FreeRTOS priority inheritance using two mutexes and a button-triggered scenario.

## Purpose

Create a classic deadlock where:
- LowPriorityTask (priority 1) acquires `Mutex1` then tries to acquire `Mutex2`.
- HighPriorityTask (priority 3) acquires `Mutex2` then tries to acquire `Mutex1` after a button press.

With priority inheritance enabled, when the HighPriorityTask blocks on `Mutex1`, the LowPriorityTask temporarily inherits the higher priority so it can finish its critical section and release the mutexes, resolving the deadlock.

## Important files

- `main.c` — tasks, mutexes and synchronization:
  - Semaphores created: `xBinarySemaphore` (button), `Mutex1`, `Mutex2`, `UARTMutex`.
  - Tasks created: `LowPriorityTask` (prio 1) and `HighPriorityTask` (prio 3).
  - `Write_String_Protected()` wraps UART output using `UARTMutex` for clean logging.
- `portf.c` / `portf.h` — Port F initialization, LED control and GPIO ISR (`GPIOF_Handler`) that gives `xBinarySemaphore` from ISR.
- `UART0.c/h` — simple UART driver used by `Write_String`.
- `priority_inheritance_demo.md` — extended step-by-step debugging guide (watch variables, breakpoints, expected output).

## LEDs and Button behavior

- PF0 and PF4: buttons (both generate the same semaphore to trigger HighPriorityTask)
- PF1 (RED): HighPriorityTask running indicator (set when it holds `Mutex2`)
- PF2 (BLUE): Flashes briefly when the deadlock is resolved (HighPriorityTask has acquired both mutexes)

## How to run

1. Open `Task2/lab8.uvprojx` in Keil µVision.
2. Build and flash to the board.
3. Open serial terminal at 115200 to observe debug messages.
4. Allow the program to start; LowPriorityTask runs periodically.
5. Press a button (PF0 or PF4) to signal HighPriorityTask and start the deadlock sequence.

## Debugging tips (concise)

- Use the debugger to watch these expressions/variables:
  - `LowPriorityTaskHandle->uxPriority` — should temporarily become 3 during inheritance.
  - `Mutex1->pxMutexHolder` and `Mutex2->pxMutexHolder` — who owns each mutex.
  - `HighPriorityTaskHandle->eCurrentState` — should be `eBlocked` while waiting.
- Place breakpoints at log points in `main.c` where tasks attempt to take mutexes (strings printed by `Write_String_Protected`).
- If UART output is garbled, prefer debugger watch variables and LED indicators — UART prints can interleave if not serialized.

## FreeRTOS configuration

- Ensure priority inheritance is enabled in `FreeRTOSConfig.h` (see `priority_inheritance_demo.md`):

```c
#define configUSE_MUTEXES_PRIORITY_INHERITANCE 1
```

## Expected behaviour (summary)

1. LowPriorityTask starts and periodically acquires `Mutex1` and then `Mutex2`.
2. Button press causes HighPriorityTask to acquire `Mutex2` and then attempt to take `Mutex1` (blocks).
3. LowPriorityTask inherits the higher priority, completes and releases `Mutex1`.
4. HighPriorityTask acquires `Mutex1`, flashes Blue LED, and releases both mutexes.

## Optional improvements

- Use `UARTMutex` (already present) to avoid interleaved prints or extend logging with task identifiers.
- Add assertions on mutex take/give results to detect unexpected failures.

---

For a full debugging walkthrough with breakpoints and watch variable suggestions, see `priority_inheritance_demo.md` in this folder.

