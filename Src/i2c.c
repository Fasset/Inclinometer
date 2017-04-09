#include "common.h"



void i2c_init() { //funkcja inicjalizujaca i2c
	RCC->AHB1ENR |= (1<<1);
	RCC->AHB1ENR |= (1<<2);
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //I2C1 clock enable 
	//PB6 - SCL, PB7 SDA
	GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1; 
	GPIOB->AFR[0] |= (1<<26) | (1<<30); 
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; 
	I2C1->CR2 = 8;  //8Mhz 
	I2C1->CCR = 40; //Tpclk=125ns, T=40*125ns=5000ns 
	I2C1->TRISE |= 0x0002;
	I2C1->CR1 = 1; //I2C enable
}

void i2c_write(uint8_t ICadres, uint8_t adres, uint8_t data) { 
	uint32_t temp = temp; //zmienna tymczasowa
	I2C1->CR1 |= I2C_CR1_ACK; //obsluga ACK
	I2C1->CR1 |= I2C_CR1_START;  //start 
	while (!(I2C1->SR1 & I2C_SR1_SB)); //oczekiwanie na potwierdzenie odbioru 
	I2C1->DR = ICadres; // wpisanie adresu I2c urzadzenia slave
	while (!(I2C1->SR1 & I2C_SR1_ADDR)); //potwierdzenie odbioru adresu
	temp = I2C1->SR2; //zerowanie flagi poprzez odczyt
	I2C1->DR = adres; //wyslanie adresu rejestru docelowego urzadzenia
	while (!(I2C1->SR1 & I2C_SR1_BTF)); //oczekiwanie na potwierdzenie wyslania 1 bajtu 
	I2C1->DR = data; //wyslanie danych
	while (!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 |= I2C_CR1_STOP; //stop
}


uint8_t i2c_read (uint8_t adr, uint8_t adr_read, uint8_t adr_reg) { //funkcja odczytu 
	int status2, dane;
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START; // START
	while (!(I2C1->SR1 & I2C_SR1_SB)); //oczekiwanie na potwierdzenie odbioru 
	I2C1->DR = adr; // slave address 		
	while (!(I2C1->SR1 & I2C_SR1_ADDR)); // wait for ADDRESS sent (ADDR=1)
	status2 = I2C1->SR2; // read status to clear flag
	while(!( I2C1->SR1 & I2C_SR1_TXE ));		
	I2C1->DR = adr_reg; // Address in chip -> DR & write
	while( !( I2C1->SR1 & I2C_SR1_BTF));
	while (!(I2C1->SR1 & I2C_SR1_TXE)); // wait for DR empty (TxE

	I2C1->CR1 |= I2C_CR1_START;	
	while( !( I2C1->SR1 & I2C_SR1_SB));
	//transmitByte('6'); 
	I2C1->DR = adr_read; //adres
	while( !( I2C1->SR1 & I2C_SR1_ADDR ));	
	status2 = I2C1->SR2;	
	I2C1->CR1 &= ~I2C_CR1_ACK;//clear ACK
	I2C1->SR1 &= ~I2C_SR1_ADDR;//clear ADDR
	I2C1->CR1 |= I2C_CR1_STOP;	
	while( !( I2C1->SR1 & I2C_SR1_RXNE ));
	dane = I2C1->DR;	
	return dane; 
} 


