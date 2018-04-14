//SPI2 - Master, send data every 50ms
//SPI2 Master
//TIM2 50ms interrupt
//PB7 LED
#include <math.h>
#include <stdio.h>
#include "stm32f10x.h" // Device header
#include "stdbool.h"
#include "header.h"
volatile int edge=500;
void tL()
{	WconL =0;
	WconR = 0;
	while(WconR + WconL <26){
		powerLeft(-1800);
		powerRight(-1800);
	}
	powerLeft(0);
	powerRight(0);
	
	dir = (dir+1)%4;

		
};
void tR()
{
	WconL = 0;
	WconR = 0;
	while(WconR + WconL <26){
	powerLeft(1800);
	powerRight(1800);
	}
	powerLeft(0);
	powerRight(0);
	
	dir = dir-1;
	if(dir < 0){
	dir = dir + 4;} 
};
int main(void) {

variousInit();	
SPI2_init();
//TIM2_init();
counter_init();
wheelInit();
USARTInitNoInterrupt();
//WIFIInit();
//USARTInitInterrupt();
DelayMs(200);
		tL();
	DelayMs(200);
		tR();	
	while(1)
{
	

	//*******************************************
/*
	while(ballCoorX>edge);
	int cX=ballCoorX;
	int cY=ballCoorY;
	int pX=0;
	int pY=0;
	
	while(1)
	{
		cX=ballCoorX;
	  cY=ballCoorY;
		float distance = sqrt( pow(cX-pX,2) + pow(cY-pY,2) );
		if( distance<20 )
			break;
		DelayMs(500);
		pX=cX;
		pY=cY;
		
	}
	
	procedure();

	powerLeft(-3900);
	powerRight(3300);
	DelayMs(130);
	stop();
	DelayMs(80);
	turnRight();
	DelayMs(1000);
	*/
	//************************************************************
}
}

/*
//Serial Port Interrupt Handler
void USART2_IRQHandler() {
	
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		
		
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		char data = USART_ReceiveData(USART2);
		if( data==':')
		{
				progress=0;
				return;
		}
		else if( data=='\n')
		{
			progress=-1;

			if(vector[0]=='C' && vector[1]=='Y' && vector[2]=='W')
			{
				blinkLight();
				parseCarCoor();
				action=1;
				
			}
			else if(vector[0]=='B' && vector[1]=='O' && vector[2]=='E')
			{
					parseBallCoor();
					action=1;
				
			}

			return;
		}
		else if(progress!=-1)
		{
			vector[progress]=data;
			progress++;
		}
		
		
	}	
}
*/

void TIM4_IRQHandler(void) {
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
			WconL++;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
 TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

void TIM2_IRQHandler(void) {
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		WconR++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}



























/*
void TIM2_IRQHandler() {
	
//Flash lights

if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7)) {
GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
} else {
GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
}

	
//Feedback controller in 
	/*
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
	ssHigh();
 SPI_I2S_SendData(SPI2, 'S');
	while((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET));
	data = SPI_I2S_ReceiveData(SPI2);
	SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_FLAG_RXNE);

	ssLow();
	 SPI_I2S_SendData(SPI2, 'S');
	while((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET));
	data = SPI_I2S_ReceiveData(SPI2);
	
SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_FLAG_RXNE);
	
	for(position=1;position<7;position++)
	{
		if(detect(position)==0)
		{
			error=position-4;
			break;
		}
	}
	
	powerRight(nominalRight+error*factor);
	powerLeft(nominalLeft-error*factor);
	
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
*/
