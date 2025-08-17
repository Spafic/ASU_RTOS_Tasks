#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UART0.h"
#include "portF.h"
#include "semphr.h"

// Define semaphore handles and global variables
SemaphoreHandle_t xSemaphoreBtn1;
SemaphoreHandle_t xSemaphoreBtn2;
unsigned char counter = '0'; // Counter for button presses
unsigned char receivedCounter = '0';

void UART_TASK(void *pvParameters);
void BTN1_CHK_TASK(void *pvParameters);
void BTN2_CHK_TASK(void *pvParameters);

int main(void)
{
    PortF_Init(); // Call initialization of part of the hardware
    UART0_Init();
    UART0_Settings(115200);

    UART0_SendString("System initialized.\r\n");

    // Create semaphores
    xSemaphoreBtn1 = xSemaphoreCreateBinary();
    xSemaphoreBtn2 = xSemaphoreCreateBinary();

    xTaskCreate(BTN1_CHK_TASK, "Button1 Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(BTN2_CHK_TASK, "Button2 Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(UART_TASK, "UART Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler(); // Start the scheduler

    for(;;);
    return 0;
}

void BTN1_CHK_TASK(void *pvParameters)
{
    for (;;)
    {
        if (!(GPIO_PORTF_DATA_R & (1 << 4))) // SW1 pressed
        {
            counter++;
            UART0_SendString("SW1 pressed. Counter incremented.\r\n");
            xSemaphoreGive(xSemaphoreBtn1); // Signal to UART task
            vTaskDelay(pdMS_TO_TICKS(150)); // Debounce
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void BTN2_CHK_TASK(void *pvParameters)
{
    for (;;)
    {
        if (!(GPIO_PORTF_DATA_R & (1 << 0))) // SW2 pressed
        {
            UART0_SendString("SW2 pressed. Counter sent. Reset to 0.\r\n");
            receivedCounter = counter; // Save current counter
            counter = '0';             // Reset
            xSemaphoreGive(xSemaphoreBtn2); // Signal UART task
            vTaskDelay(pdMS_TO_TICKS(150)); // Debounce
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void UART_TASK(void *pvParameters)
{
    for (;;)
    {
        // Wait for either semaphore
        if (xSemaphoreTake(xSemaphoreBtn1, pdMS_TO_TICKS(10)) == pdPASS)
        {
            UART0_SendString("UART Task: SW1 updated counter = ");
            UART0_SendChar(counter);
            UART0_SendString("\r\n");
        }

        if (xSemaphoreTake(xSemaphoreBtn2, pdMS_TO_TICKS(10)) == pdPASS)
        {
            UART0_SendString("UART Task: SW2 final value = ");
            UART0_SendChar(receivedCounter);
            UART0_SendString("\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
