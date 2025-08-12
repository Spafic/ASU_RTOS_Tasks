#include "UART0.h"

#define ClkDiv 16

extern uint32_t SystemCoreClock;

void UART0_Init(void){
	//Enable UART0 and GPIOA clock
	SYSCTL_RCGCUART_R |= (1 << 0); //UART0
	SYSCTL_RCGCGPIO_R |= (1 << 0); //Port A
	
	// Wait for peropherals to be ready
	while((SYSCTL_RCGCUART_R & (1 << 0)) ==0 );
	while((SYSCTL_RCGCGPIO_R & (1 << 0)) ==0 );
	
	//Unlock and configure PA0, PA1 for UART
	GPIO_PORTA_LOCK_R = 0x4C4F434B;
	GPIO_PORTA_CR_R |= (1 << 0) | (1 << 1);
	GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);
	GPIO_PORTA_PCTL_R |= (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011 ;
	GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);
}

void UART0_Settings(unsigned int BaudRate){
	UART0_CC_R &= ~(1 << 0); //Disable UART for configrations
	UART0_CC_R = 0;  //Use System clock
	int intPart = SystemCoreClock / (ClkDiv * BaudRate);
	int fracPart = ((SystemCoreClock / (ClkDiv * BaudRate)) * 64 + (ClkDiv * BaudRate) /2) / (ClkDiv * BaudRate);
	
	UART0_IBRD_R =intPart;
	UART0_FBRD_R = fracPart;
	
	UART0_LCRH_R = 0x70;  //8 bits, no piriority, 1 stopbit, FIFO enabled
	UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); //Enable: UART, TX, RX
}

void UART0_Interrupt(unsigned int flag){
	if(flag){
		UART0_IM_R |= (1 << 4); //Enable RX interrupts
	} else {
		UART0_IM_R &= ~ (1 << 4);
	}
	NVIC_EN0_R |= (1 <<5);
}

void UART0_SendChar (char c){
	while((UART0_FR_R & (1 << 5)) != 0); // Wait for TX ready
	UART0_DR_R = c;
}

void UART0_SendString (const char *str){
	while(*str){
		UART0_SendChar(*str++); //Loop on string and send char by char
	}
}