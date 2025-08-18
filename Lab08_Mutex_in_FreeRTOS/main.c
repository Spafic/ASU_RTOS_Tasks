#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include "UART0.h"
#include "portf.h"
#include "queue.h"


SemaphoreHandle_t xBinarySemaphore;
SemaphoreHandle_t Mutex;

void InitTask(void);
void CounterTask(void *pvParameters);
void LedToggleTask(void *pvParameters);
void CounterTask (void *pvParameters);
void LedToggleTask(void *pvParameters);

int main (void) {
	InitTask();
	xTaskCreate(CounterTask, "CounterTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(LedToggleTask, "LedTogglerTask", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	
	vTaskStartScheduler();

	while(1);

}

void CounterTask(void *pvParameters) {
	if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE) {
		Write_String((unsigned char *)"this is the counter task\n");
		xSemaphoreGive(Mutex);
	}
	while(1){
		if (xSemaphoreTake(Mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
			// Print all numbers 0-10 as a complete sequence
			for (int i = 0; i <= 10; i++) {
				if (i < 10) {
					Write_Char('0' + i);  // Convert number to ASCII character
				} else {
					Write_String((unsigned char *)"10");  // Handle two-digit number
				}
				Write_String((unsigned char *)"\n");
			}
			xSemaphoreGive(Mutex);
		}
		vTaskDelay(pdMS_TO_TICKS(1000));  // Wait 1 second before next complete sequence
	}
}

void LedToggleTask(void *pvParameters) {
	if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE) {
		Write_String((unsigned char *)"This is the LedToggler task\n");
		xSemaphoreGive(Mutex);
	}
	while(1) {
		if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
			/* Perform a single blink on each button press:
			   - Turn LED on
			   - Print message under mutex (with short timeout)
			   - Delay 500 ms
			   - Turn LED off
			*/
			GPIO_PORTF_DATA_R |= 0x04;  // Turn LED on (PF2)

			if (xSemaphoreTake(Mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
				Write_String((unsigned char *)"This is the led toggler task\n");
				xSemaphoreGive(Mutex);
			}

			vTaskDelay(pdMS_TO_TICKS(500));

			GPIO_PORTF_DATA_R &= ~0x04; // Turn LED off (PF2)
		}
	}
}

void InitTask(void) {
	 PortF_Init();

	 UART0_Init();
	 UART0_Settings (115200);

	 /* Create synchronization primitives before enabling the GPIO interrupt.
		 This prevents the ISR from running and accessing a NULL semaphore if
		 a button press occurs during or immediately after startup. */
	 xBinarySemaphore = xSemaphoreCreateBinary();
	 Mutex = xSemaphoreCreateMutex();

	 PortF_Interrupt_Settings();
	 PortF_Interrupt_Enable();
}

void GPIOF_Handler (void) {
	uint32_t mis = GPIO_PORTF_MIS_R;
	/* Clear the interrupt flags first */
	GPIO_PORTF_ICR_R = mis & 0x11;

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	/* For each fired pin give the semaphore. Using the same semaphore for both
	   pins is fine for this lab — both buttons should unblock the same task. */
	if (mis & (1 << 0)) {
		xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
	}
	if (mis & (1 << 4)) {
		xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
	}

	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
