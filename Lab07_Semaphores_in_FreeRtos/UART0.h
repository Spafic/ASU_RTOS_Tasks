#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"

void UART0_Init(void);
void UART0_Settings(unsigned int BaudRate);
void UART0_Interrupt(unsigned int flag);
void UART0_SendChar (char c);
void UART0_SendString (const char *str);