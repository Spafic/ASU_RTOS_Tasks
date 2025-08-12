#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"

// LED masks
#define RED_LED    0x02  // PF1
#define BLUE_LED   0x04  // PF2
#define GREEN_LED  0x08  // PF3

// Function prototypes
void PortF_Init(void);
void Task_Red(void *pvParameters);
void Task_Green(void *pvParameters);
void vApplicationIdleHook(void);

//--------------------------------------------------
// MAIN
//--------------------------------------------------
int main(void)
{
    PortF_Init();

    // Create tasks
    xTaskCreate(Task_Red, "Red LED Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(Task_Green, "Green LED Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1) {}
}

//--------------------------------------------------
// Port F initialization
//--------------------------------------------------
void PortF_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;                  // Enable Port F clock
    while ((SYSCTL_PRGPIO_R & 0x20) == 0) {}    // Wait until ready

    GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED | GREEN_LED); // PF1, PF2, PF3 output
    GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED | GREEN_LED); // Digital enable
    GPIO_PORTF_DATA_R &= ~(RED_LED | BLUE_LED | GREEN_LED); // All LEDs off
}

//--------------------------------------------------
// Task: Toggle Red LED every 1s
//--------------------------------------------------
void Task_Red(void *pvParameters)
{
    for (;;)
    {
        GPIO_PORTF_DATA_R ^= RED_LED;
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second
    }
}

//--------------------------------------------------
// Task: Toggle Green LED every 2s
//--------------------------------------------------
void Task_Green(void *pvParameters)
{
    for (;;)
    {
        GPIO_PORTF_DATA_R ^= GREEN_LED;
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds
    }
}

//--------------------------------------------------
// Idle Hook: Toggles Blue LED
//--------------------------------------------------
void vApplicationIdleHook(void)
{
    static TickType_t lastTick = 0;
    TickType_t now = xTaskGetTickCount();

    // Only toggle every 100ms
    if ((now - lastTick) >= pdMS_TO_TICKS(100))
    {
        GPIO_PORTF_DATA_R ^= BLUE_LED;
        lastTick = now;
    }
}
