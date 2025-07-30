#include <stdint.h>
#include "tm4c.h"
#include "FreeRTOS.h"
#include "task.h"

#define RED_LED 0x02

void PortF_Init(void);
void ToggleRedLED(void);
void Task1(void *pvParameters);

int main(void)
{
    PortF_Init();

    // Create a task
    xTaskCreate(Task1, "Red LED Task",configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Infinite loop (should never reach here)
    while (1) {}
}

void Task1(void *pvParameters)
{
    for( ; ; )
    {
        ToggleRedLED();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000 ms
    }
}

void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x20;
		while((SYSCTL_PRGPIO_R & 0x20) == 0);
    GPIO_PORTF_DIR_R = 0x0E;
		GPIO_PORTF_DEN_R = 0x0E;
    GPIO_PORTF_DATA_R = 0x00;
}

void ToggleRedLED(void)
{
		GPIO_PORTF_DATA_R ^= RED_LED;
}