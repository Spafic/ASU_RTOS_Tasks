#ifndef UART0_h_

#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"

void UART0_Init(void);
void UART0_Settings(unsigned int BaudRate);
void UART0_Interrupt(unsigned int flag);
void Write_Char (unsigned char character);
void Write_String (unsigned char *p_char);

#endif