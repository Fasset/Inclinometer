#include "common.h"
#include <math.h>

#define PI 3.14159f

volatile struct _accel {
	uint8_t msb;
	uint8_t lsb;
	int16_t z_out;
	int16_t x_out;
	int16_t y_out;
	float z_g_value;
	float y_g_value;
	float x_g_value;
	float EMA_z_g_value;
	float EMA_y_g_value;
	float EMA_x_g_value;
	float angle;
} accel;

char dataBufor[128];


void TIM3_init(void) //TIM3 - frequency set delay  
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
	TIM3->ARR  = 25000;
	TIM3->PSC = 390;
	TIM3->DIER   |= TIM_DIER_UIE; //interrupt enable 
	TIM3->CR1    |= TIM_CR1_CEN; //counter enable
	NVIC_SetPriority(TIM3_IRQn, 0); 
	NVIC_EnableIRQ(TIM3_IRQn); // NVIC enable 
}

void TIM3_IRQHandler (void)  //TIM3 interrupt handling 
{
 if (TIM3->SR & TIM_SR_UIF) 
 {
	accel.lsb = i2c_read(ADXL_WRITE, ADXL_READ, OUT_X_L);
	accel.msb = i2c_read(ADXL_WRITE, ADXL_READ, OUT_X_H);
	accel.x_out = ((accel.msb << 8) | accel.lsb);

	accel.lsb = i2c_read(ADXL_WRITE, ADXL_READ, OUT_Y_L);
	accel.msb = i2c_read(ADXL_WRITE, ADXL_READ, OUT_Y_H);
	accel.y_out = ((accel.msb << 8) | accel.lsb);

	accel.lsb = i2c_read(ADXL_WRITE, ADXL_READ, OUT_Z_L);
	accel.msb = i2c_read(ADXL_WRITE, ADXL_READ, OUT_Z_H);
	accel.z_out = ((accel.msb << 8) | accel.lsb);

	accel.x_g_value = (float)accel.x_out * SCALE_2G;
	accel.y_g_value = (float)accel.y_out * SCALE_2G;
	accel.z_g_value = (float)accel.z_out * SCALE_2G;

	accel.EMA_x_g_value = (accel.x_g_value - accel.EMA_x_g_value) / EMA_SMOOTH + accel.EMA_x_g_value ;
	accel.EMA_y_g_value = (accel.y_g_value - accel.EMA_y_g_value) / EMA_SMOOTH + accel.EMA_y_g_value ;
	accel.EMA_z_g_value = (accel.z_g_value - accel.EMA_z_g_value) / EMA_SMOOTH + accel.EMA_z_g_value ;

	if(accel.EMA_x_g_value >= 1) { //sinus {-1, 1}
		accel.EMA_x_g_value = 1;
	}
	if(accel.EMA_y_g_value >= 1) {
		accel.EMA_y_g_value = 1;
	}
	if(accel.EMA_z_g_value >= 1) {
		accel.EMA_z_g_value = 1;
	}
	if(accel.EMA_x_g_value <= -1) {
		accel.EMA_x_g_value = -1;
	}
	if(accel.EMA_y_g_value <= -1) {
		accel.EMA_y_g_value = -1;
	}
	if(accel.EMA_z_g_value <= -1) {
		accel.EMA_z_g_value = -1;
	}


	sprintf(dataBufor, "%f\r", ((atan(accel.EMA_x_g_value / accel.EMA_z_g_value) / PI) * 180.0)) ;
   	transmitBuf(dataBufor);

	GPIOA->ODR ^= (1<<5);
 }
	TIM3->SR = 0;  
}




