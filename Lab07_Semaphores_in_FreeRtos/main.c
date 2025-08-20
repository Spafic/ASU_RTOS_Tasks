#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UART0.h"
#include "portF.h"
#include "semphr.h"

QueueHandle_t xQueue;
SemaphoreHandle_t xCountingSemaphore;

void UART_TASK(void *pvParameters);
void BTN_CHK_TASK(void *pvParameters);
void InitTask(void);

int main(void)
{
    InitTask();
	
		//Create Tasks
		xTaskCreate(BTN_CHK_TASK, "Button Task", configMINIMAL_STACK_SIZE, NULL ,1, NULL);
		xTaskCreate(UART_TASK, "UART Task", configMINIMAL_STACK_SIZE, NULL ,1, NULL);
		
		//Start schedular
		vTaskStartScheduler();
	
    for(;;);
    return 0;
}

void InitTask(void){
	// Initialize PortF and Uart
	
	PortF_Init();
	UART0_Init();
	
	// Select Baud Rate
	UART0_Settings(115200); 
	
	xQueue = xQueueCreate(8, sizeof(uint8_t));
	xCountingSemaphore = xSemaphoreCreateCounting(5, 0);
	
}

void BTN_CHK_TASK(void *pvParameters)
{
		static uint8_t IncrementCounter = '0';
    for (;;)
    {
        if ((GPIO_PORTF_DATA_R & (1 << 0)) == 0) // SW1 pressed
        {
            IncrementCounter++;
						xQueueSend(xQueue, &IncrementCounter , portMAX_DELAY);
						xSemaphoreGive(xCountingSemaphore);
						vTaskDelay(pdMS_TO_TICKS(100)); //Delay for 100 ms
        }
        vTaskDelay(pdMS_TO_TICKS(50)); //Debounce delay fro 50 ms
    }
}

void UART_TASK (void *pvParameters) {
    uint8_t receivedValue;
    for(;;) {
        if(xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE) {
            if(xQueueReceive(xQueue, &receivedValue, 0) == pdPASS) {
                while ((UART0_FR_R & (1<<5)) != 0);
                UART0_DR_R = receivedValue;

                while ((UART0_FR_R & (1<<5)) != 0);
                UART0_DR_R = '\r';

                while ((UART0_FR_R & (1<<5)) != 0);
                UART0_DR_R = '\n';
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}