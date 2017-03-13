#include "stm32f4xx.h"

//USART
#define CLOCK_FREQ 50000000UL
#define BAUD 115200UL
#define BAUDRATE (CLOCK_FREQ/BAUD)
#define NO_CHAR (-1)
#define UART_BUFF 16UL //rozmiar odbieranego bufora
#define UART_MASK (UART_BUFF-1)

//I2C
#define I2C_ADRESS_WRITE (210)

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define CTRL_REG6 0x25
#define WHO_AM_I 0x0F
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D 
#define FIFO_CTRL_REG (0x2E)





//FUNKCJE DEKLARACJE

void uart_init(void) ;
void transmitBuf(char *dat);
void transmitByte(char data);
/*
void transmitByte(char data);
void TIM3_init(void); 
void i2c_init(void);
uint8_t i2c_read (uint8_t adr, uint8_t adr_read, uint8_t adr_reg);
void i2c_write(uint8_t ICadres, uint8_t adres, uint8_t data);
void transmitBuf(char *dat);
extern volatile uint8_t flag; */
