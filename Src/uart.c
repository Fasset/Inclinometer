
#include "common.h"

volatile uint8_t usart_write, usart_read;
volatile char tab[UART_BUFF];

void uart_init(void) 
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable clock for USART 
	RCC->AHB1ENR |= (1<<0);
	GPIOA->MODER |= GPIO_MODER_MODER2_1; //alternate function PA2 
	GPIOA->MODER |= GPIO_MODER_MODER3_1; //alternate function PA3
	GPIOA->AFR[0] |= (1<<8)|(1<<9)|(1<<10) | (1<<12) | (1<<13) | (1<<14); //PA2, PA3 A1
	USART2->BRR = (uint32_t)(BAUDRATE); 
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE ; //RX, TX, RX interrupt enable
	USART2->CR1 |= USART_CR1_UE;  //UART enable
	NVIC_SetPriority(USART2_IRQn, 2); 
	NVIC_EnableIRQ(USART2_IRQn);
	
}

void USART2_IRQHandler(void)
{
	if(USART2->SR & USART_SR_RXNE) 
	{ 
		usart_write++; 
		usart_write = usart_write & UART_MASK; 
		tab[usart_write] =  USART2->DR; 	
	}
}

char get_char(void) 
{
	if(usart_write == usart_read) 
	{
		return(NO_CHAR);
	}
	usart_read++; 
	usart_read = usart_read & UART_MASK; 
	return(tab[usart_read]); 
}

void transmitByte(char data)
{
	while( !(USART2->SR & USART_SR_TXE) );
	USART2->DR=data;
} 

void transmitBuf(char *dat)
{
	while(*dat)
	{
		transmitByte(*dat++); 
	}
}