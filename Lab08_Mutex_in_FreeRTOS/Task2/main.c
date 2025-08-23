#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include "UART0.h"
#include "portf.h"
#include "queue.h"


SemaphoreHandle_t xBinarySemaphore;
SemaphoreHandle_t Mutex1;
SemaphoreHandle_t Mutex2;
SemaphoreHandle_t UARTMutex;

void InitTask(void);
void LowPriorityTask(void *pvParameters);
void HighPriorityTask(void *pvParameters);
TaskHandle_t LowPriorityTaskHandle;
TaskHandle_t HighPriorityTaskHandle;

/* UART protected write */
void Write_String_Protected(unsigned char *str);

int main (void) {
	InitTask();
	xTaskCreate(LowPriorityTask, "LowPriTask", configMINIMAL_STACK_SIZE, NULL, 1, &LowPriorityTaskHandle);
	xTaskCreate(HighPriorityTask, "HighPriTask", configMINIMAL_STACK_SIZE, NULL, 3, &HighPriorityTaskHandle);
	
	vTaskStartScheduler();

	while(1);

}

/* Simple UART write wrapper that serializes access to the UART peripheral */
void Write_String_Protected(unsigned char *str) {
	if (UARTMutex != NULL) {
		if (xSemaphoreTake(UARTMutex, portMAX_DELAY) == pdTRUE) {
			Write_String(str);
			xSemaphoreGive(UARTMutex);
		}
	} else {
		/* Fallback if mutex not created */
		Write_String(str);
	}
}

void LowPriorityTask(void *pvParameters) {
	Write_String_Protected((unsigned char *)"Low Priority Task Started\n");
	
	while(1) {
		// Signal when starting a new cycle
	Write_String_Protected((unsigned char *)"Low Priority Task: Starting new cycle\n");
		
		// First, acquire Mutex1
	Write_String_Protected((unsigned char *)"Low Priority Task: Attempting to take Mutex1\n");
		if (xSemaphoreTake(Mutex1, portMAX_DELAY) == pdTRUE) {
			Write_String_Protected((unsigned char *)"Low Priority Task: Acquired Mutex1\n");
			
			// Delay to simulate work and allow debugging
			vTaskDelay(pdMS_TO_TICKS(500));
			
			// Now try to acquire Mutex2
			Write_String_Protected((unsigned char *)"Low Priority Task: Attempting to take Mutex2\n");
			if (xSemaphoreTake(Mutex2, portMAX_DELAY) == pdTRUE) {
				// Critical section with both mutexes
				Write_String_Protected((unsigned char *)"Low Priority Task: Acquired both mutexes\n");
				vTaskDelay(pdMS_TO_TICKS(500));
				
				// Release mutexes in reverse order
				xSemaphoreGive(Mutex2);
				Write_String_Protected((unsigned char *)"Low Priority Task: Released Mutex2\n");
			}
			
			// Release Mutex1
			xSemaphoreGive(Mutex1);
			Write_String_Protected((unsigned char *)"Low Priority Task: Released Mutex1\n");
		}
		
		// Wait before next cycle
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}

void HighPriorityTask(void *pvParameters) {
	Write_String_Protected((unsigned char *)"High Priority Task Started\n");
	
	// Let low priority task start first
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	while(1) {
		// Wait for button press (xBinarySemaphore)
		if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
			Write_String_Protected((unsigned char *)"High Priority Task: Button pressed, starting deadlock sequence\n");
			
			// First, acquire Mutex2
			Write_String_Protected((unsigned char *)"High Priority Task: Attempting to take Mutex2\n");
			if (xSemaphoreTake(Mutex2, portMAX_DELAY) == pdTRUE) {
				Write_String_Protected((unsigned char *)"High Priority Task: Acquired Mutex2\n");
				
				// Turn on LED to indicate high priority task is running
				GPIO_PORTF_DATA_R |= 0x02;  // Turn on RED LED (PF1)
				
				// Small delay to ensure Low Priority Task has Mutex1
				vTaskDelay(pdMS_TO_TICKS(200));
				
				// Now try to acquire Mutex1 - this will block if Low Priority Task has it
				Write_String_Protected((unsigned char *)"High Priority Task: Attempting to take Mutex1\n");
				if (xSemaphoreTake(Mutex1, portMAX_DELAY) == pdTRUE) {
					// Critical section with both mutexes
					Write_String_Protected((unsigned char *)"High Priority Task: Acquired both mutexes\n");
					
					// Turn on Blue LED to indicate deadlock is resolved
					GPIO_PORTF_DATA_R |= 0x04;  // Turn on BLUE LED (PF2)
					vTaskDelay(pdMS_TO_TICKS(500));
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off BLUE LED
					
					// Release Mutex1
					xSemaphoreGive(Mutex1);
					Write_String_Protected((unsigned char *)"High Priority Task: Released Mutex1\n");
				}
				
				// Release Mutex2
				xSemaphoreGive(Mutex2);
				Write_String_Protected((unsigned char *)"High Priority Task: Released Mutex2\n");
				GPIO_PORTF_DATA_R &= ~0x02; // Turn off RED LED
			}
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
	Mutex1 = xSemaphoreCreateMutex();
	Mutex2 = xSemaphoreCreateMutex();
	UARTMutex = xSemaphoreCreateMutex();

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
