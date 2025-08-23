#include "UART0.h"

#define ClkDiv 16

extern uint32_t SystemCoreClock;

void UART0_Init(void) {
	
	SYSCTL_RCGCUART_R |= (1 << 0);
	SYSCTL_RCGCGPIO_R |= (1 << 0);
	GPIO_PORTA_LOCK_R = 0x4C4F434B;
	GPIO_PORTA_CR_R |= (1 << 0) | (1 << 1);
	GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
	GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);
}


void UART0_Settings (unsigned int BaudRate) {
	
	UART0_CTL_R &= ~(1<<0);
	UART0_CC_R = 0;
	
	int baudDivisor = SystemCoreClock / (ClkDiv * BaudRate);
	float fractionalPart = (float) (SystemCoreClock % (ClkDiv * BaudRate)) / (ClkDiv * BaudRate);

	UART0_IBRD_R = baudDivisor;
	UART0_FBRD_R = (int) ((fractionalPart * 64) + 0.5);
	UART0_LCRH_R = 0x00000060;
	UART0_CTL_R |= (1 << 0);
	
}

void UART0_Interrupt (unsigned int flag) {
	if (flag) {
		UART0_IM_R |= (1 << 4);
	}
	else{
		UART0_IM_R &= ~(1 << 4);
	}
	NVIC_EN0_R |= (1 << 5);
	
}

void Write_Char (unsigned char character) {
	while ((UART0_FR_R >> 5) & 1) {};
	UART0_DR_R = character;
		
}

void Write_String (unsigned char *p_char) {
	while (*p_char) {
		if (*p_char == '\n') {
			Write_Char('\r');
		}
		Write_Char(*p_char++);
	}
}