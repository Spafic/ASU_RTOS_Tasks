#include <stdint.h>
#include "tm4c.h"
#include "FreeRTOS.h"
#include "task.h"

#define RED_LED 0x02
#define BLUE_LED 0x04
#define GREEN_LED 0x08

void PortF_Init(void);

void vTask1 (void *pvParameters);
void vTask2 (void *pvParameters);
void vTask3 (void *pvParameters);

int main (void){
		PortF_Init();
	
		xTaskCreate (vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate (vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate (vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	
		vTaskStartScheduler ();
	
		for(;;);
}

void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x20;
		while((SYSCTL_PRGPIO_R & 0x20) == 0);
    GPIO_PORTF_DIR_R = 0x0E;
		GPIO_PORTF_DEN_R = 0x0E;
    GPIO_PORTF_DATA_R = 0x00;
}

void vTask1 (void *pvParameters)
{
		TickType_t xLastWakeTime = xTaskGetTickCount();
		for(;;){
				GPIO_PORTF_DATA_R ^= RED_LED;
				vTaskDelayUntil(&xLastWakeTime, 1000);
		}
}

void vTask2 (void *pvParameters)
{
		TickType_t xLastWakeTime = xTaskGetTickCount();
		for(;;){
				GPIO_PORTF_DATA_R ^= BLUE_LED;
				vTaskDelayUntil(&xLastWakeTime, 2000);
		}
}

void vTask3 (void *pvParameters)
{
		TickType_t xLastWakeTime = xTaskGetTickCount();
		for(;;){
				GPIO_PORTF_DATA_R ^= GREEN_LED;
				vTaskDelayUntil(&xLastWakeTime, 3000);
		}
}
