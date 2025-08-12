#include <stdint.h>
#include "tm4c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UART0.h"
#include "portF.h"
#include "queue.h"

// Define the queue handle and other global variables
QueueHandle_t xQueue;
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
    xQueue = xQueueCreate(5, sizeof(char));

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
            UART0_SendString("SW2 pressed. Counter sent to queue. Reset to 0.\r\n");
            if (xQueueSend(xQueue, &counter, pdMS_TO_TICKS(100)) == pdPASS)
            {
                counter = '0'; // Reset counter after sending
            }
            vTaskDelay(pdMS_TO_TICKS(150)); // Debounce
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void UART_TASK(void *pvParameters)
{
    for (;;)
    {
        if (xQueueReceive(xQueue, &receivedCounter, pdMS_TO_TICKS(100)) == pdPASS)
        {
            UART0_SendString("UART Task: Received value from queue: ");
            UART0_SendChar(receivedCounter);
            UART0_SendString("\r\n");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}