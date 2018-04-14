//grid based movements
volatile unsigned char data='\0';
volatile int position=0;
volatile int nominalLeft=2100;
volatile int nominalRight=2100;
volatile float factor=24;
volatile char vector[25]="";
volatile char reply[2]="";
volatile char message1[]="AT\r\n";		
volatile char getIP[]="AT+CWJAP=\"EIE3105\",\"31053106\"\r\n";
volatile char message2[]="AT+CIPSTART=\"UDP\",\"0\",0,3105,2\r\n";
volatile int expected=-1;
volatile int car_flag=0;
volatile int ball_flag=0;
volatile int K1=20;
volatile int K2=16;
volatile int K3=20;
volatile int ref=-1;
#define mX 200
#define mY 300
#define PI 3.1415926

//************************************************************************
volatile int ballX=0;
volatile int ballY=0;
volatile int carAX=0;
volatile int carAY=0;
volatile int carBX=0;
volatile int carBY=0;

volatile int WconL = 0;
volatile int WconR = 0;


int suppX =0;
volatile int roflag = 0;

volatile int dir = 2;//present direction 
volatile int redir = 0;//reqiued direction


int hexTodec(char);

//************************************************************************


//************************************************************************
//Ball Game
volatile int progress=-1;
volatile int action=0;
volatile int carCoorX=-1;
volatile int carCoorY=-1;
volatile int ballCoorX=1200;
volatile int ballCoorY=-1;
volatile int turningTimeLeft=600;
volatile int turningTimeRight=700 ;
volatile int initial_threshold=10;

//************************************************************************

// TIM3 CH1 PWM  **  PA6      ** D12
#define TIM3_CH1_PWM_RCC_GPIO  RCC_APB2Periph_GPIOA
#define TIM3_CH1_PWM_GPIO      GPIOA
#define TIM3_CH1_PWM_PIN       GPIO_Pin_6
// TIM3 CH2 PWM  **  PA7      ** D11
#define TIM3_CH2_PWM_RCC_GPIO  RCC_APB2Periph_GPIOA
#define TIM3_CH2_PWM_GPIO      GPIOA
#define TIM3_CH2_PWM_PIN       GPIO_Pin_7
//TIM4 CH1 IC1   **  PB6
#define TIM4_CH1_IT1_RCC_GPIO  RCC_APB2Periph_GPIOB
#define TIM4_CH1_IT1_GPIO      GPIOB
#define TIM4_CH1_IT1_PIN       GPIO_Pin_6
void DelayMs(uint32_t ms);

void DelayMs(uint32_t ms);
void turnOn(void);
void turnOff(void);
static __IO uint32_t msTicks;
int convert(char);
void powerLeft(int);
void powerRight(int);
void DelayMs(uint32_t ms)
{
	// Reload us value
	msTicks = ms;
	// Wait until usTick reach zero
	while (msTicks);
}

// SysTick_Handler function will be called every 1 ms
void SysTick_Handler()
{
	if (msTicks != 0)
	{
		msTicks--;
	}
}


void setBackwardDirectionRight(void);
void setBackwardDirectionLeft(void);
void setForwardDirectionRight(void);
void setForwardDirectionLeft(void);
int validate(void);

int validate()
{
			int sum=0;
	for(int i=0;i<15;i++)
	{
		sum+=vector[i];
	}
	
		if(sum%256==0)
		{
			turnOn();
			return 1;
		}
		else
		{
			return 0;
		}

}

void ssHigh()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
}

void ssLow()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_12, Bit_RESET);
}

void variousInit()
{	
	//GPIO Init PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO Init PC15
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO Init PB7
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	
	// Configure the SysTick timer to overflow every 1 ms
	SysTick_Config(SystemCoreClock / 1000);
}

void powerRight(int cycle)
{
	if(cycle<0)
	{
		setBackwardDirectionRight();
		TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = -1*cycle; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(cycle>=0)
	{
	setForwardDirectionRight();
		TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = cycle; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
}

void powerLeft(int cycle)
{
	if(cycle<0)
	{
		setBackwardDirectionLeft();
		TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = -1*cycle; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(cycle>=0)
	{
		setForwardDirectionLeft();
		TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = cycle; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
}

void send( volatile char arr[])
{
	int count=0;
	for(;arr[count]!='\0';count++)
	{
		
	}
	for(int i=0;i<count;i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, arr[i]); 
	}
}

void SPI2_init(void) {
RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//SPI Configuration
	
SPI_InitTypeDef SPI_InitStructure;
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
SPI_Init(SPI2, &SPI_InitStructure);
// Enable SPI2

//*****************************************

//*****************************************

SPI_Cmd(SPI2, ENABLE);
}
//*****************************************
//*****************************************
/*
void TIM2_init() {
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
TIM_TimeBaseInitTypeDef timerInitStructure;
timerInitStructure.TIM_Prescaler = 18000; //1/(72Mhz/18000)=0.25ms
timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
timerInitStructure.TIM_Period = 200; //0.25ms*200 = 50ms
timerInitStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1;
timerInitStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIM2, &timerInitStructure);
TIM_Cmd(TIM2, ENABLE);
//Enable update event and interrupt for Timer2
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
NVIC_EnableIRQ(TIM2_IRQn);
}
*/
//*****************************************
//*****************************************
void stop()
{
	powerLeft(0);
	powerRight(0);
}


void wheelInit()
{
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure I/O for Tim3 Ch1 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH1_PWM_GPIO, &GPIO_InitStructure);
	
	// Configure I/O for Tim3 Ch2 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH2_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH2_PWM_GPIO, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 144-1;  //1/(72Mhz/1440)=0.2ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 5000-1;  
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);
	
	TIM_OCInitTypeDef outputChannelInit;
	//Enable Tim3 Ch1 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//Enable Tim3 Ch2 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
}

void USARTInitInterrupt()
{
	
	//USART2 TX RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART2 ST-LINK USB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStructure; 
	
	USART_InitStructure.USART_BaudRate = 115200;
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	// Enable the USART2 TX Interrupt 
	USART_ITConfig(USART2, USART_IT_TC, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// Enable the USART2 RX Interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void USARTInitNoInterrupt()
{
	//USART2 TX RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART2 ST-LINK USB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStructure; 
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}
void counter_init()
{

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
  	TIM_TimeBaseInitTypeDef timerInitStructure;
  	timerInitStructure.TIM_Prescaler = 0;
  	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	timerInitStructure.TIM_Period = 1;
  	timerInitStructure.TIM_ClockDivision = 0;
  	timerInitStructure.TIM_RepetitionCounter = 0;
  	TIM_TimeBaseInit(TIM4, &timerInitStructure);
  	TIM_Cmd(TIM4, ENABLE);
		
		timerInitStructure.TIM_Prescaler = 0;
  	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	timerInitStructure.TIM_Period = 1;
  	timerInitStructure.TIM_ClockDivision = 0;
  	timerInitStructure.TIM_RepetitionCounter = 0;
  	TIM_TimeBaseInit(TIM2, &timerInitStructure);
  	TIM_Cmd(TIM2, ENABLE);
	
		TIM_TIxExternalClockConfig(TIM4, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
		TIM_TIxExternalClockConfig(TIM2, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0);
	
		//Enable update event for Timer2
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		NVIC_EnableIRQ(TIM4_IRQn);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		NVIC_EnableIRQ(TIM2_IRQn);
		
	
}

void blinkLight()
{
if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7)) {
GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
} else {
GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
}
}

int detect(int pos)
{
	if( (data & (1<<pos) )==0)
		return 0;
	else 
		return 1;
}

int convert(char c) {
	switch(c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
	}
}

void parseCarCoor()
{
	carCoorX= convert(vector[6])*256 + convert(vector[7])*16 + convert(vector[8]);
	carCoorY= convert(vector[9])*256 + convert(vector[10])*16 + convert(vector[11]);
	car_flag=(car_flag+1)%5;
}

void parseBallCoor()
{
	ballCoorX= convert(vector[6])*256 + convert(vector[7])*16 + convert(vector[8]);
	ballCoorY= convert(vector[9])*256 + convert(vector[10])*16 + convert(vector[11]) + 5;
	ball_flag=(ball_flag+1)%5;
}

void setForwardDirectionRight()
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
}

void setForwardDirectionLeft()
{	
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
}

void setBackwardDirectionRight()
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_RESET);
}

void setBackwardDirectionLeft()
{	
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);

}


void turnOn()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
}

void turnOff()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
}

void turnLeft()
{
	powerLeft(-2950);
	powerRight(2950);
	DelayMs(turningTimeLeft);
	stop();
}

void turnRight()
{
	powerLeft(2950);
	powerRight(-2950);
	DelayMs(turningTimeRight);
	stop();
}
//***************************************************************************************************
//***************************************************************************************************

float r2d(float radian)
{
	float degree= 180*radian/( PI );
	return degree;
}

float setRef(int carX, int carY, int ballX, int ballY, char option)
{
	if( option == 'b')
	{
		float distance = sqrt( pow(carCoorX-mX,2) + pow(carCoorY-mY,2) );
	float angle=acos( (mY-carCoorY)/distance );
	return angle;
	}
	else if ( option == 'f')
	{
		ballX = ballX - 100;
		float distance = sqrt( pow(carX-ballX,2) + pow(carY-ballY,2) );
	float angle=acos( (ballX-carX)/distance );
	return angle;
	}
	else
	return 0;
}

float calDistance(int carX, int carY, int ballX, int ballY)
{
	ballX = ballX - 100;
	float distance = sqrt( pow(carX-ballX,2) + pow(carY-ballY,2) );
	return distance;
}

int calDistanceS(int carX, int carY, int ballX, int ballY)
{
	int distanceS =pow(carX-ballX,2) + pow(carY-ballY,2);
	return distanceS;
}


float calError( float reference, char option)
{
	if(option == 'b')
	{
		float distance = sqrt( pow(carCoorX-mX,2) + pow(carCoorY-mY,2) );
	float angle= acos( (mY-carCoorY)/distance );
	float error = distance * sin(angle-reference);
	return error;
	}
	else if(option == 'f')
	{
		int ballX = ballCoorX - 100;
		int ballY = ballCoorY;
		float distance = sqrt( pow(ballX-carCoorX,2) + pow(ballY-carCoorY,2) );
	float angle=acos( (ballX-carCoorX)/distance );
	float error = distance * sin(reference-angle);
	return error;
	}
	else
		return 0;
}
//*******************************************************************************************************
//***************************************************************************************************

void backToBottom(){
	
	float angle = setRef(carCoorX,carCoorY,ballCoorX,ballCoorY,'b');
	
	if(mY>carCoorY)
	{
		while(carCoorX>160)
	{
		int error = ((int) (calError(angle,'b')));
		int left=-2600+15*error;
		int right=-2600-15*error;
		if(-3000<left<-2000)
		{
			powerLeft(left);
		}
		else if(left<=-3000)
		{
			powerLeft(-3000);
		}
		else 
		{
			powerLeft(-2000);
		}
		
		if(-3000<right<-2000)
		{
			powerRight(right);
		}
		else if(right<=-3000)
		{
			powerRight(-3000);
		}
		else
		{
			powerRight(-2000);
		}
		
	}
	}
	else
	{
		while(carCoorX>160)
		{
			int error = ((int) (calError(angle,'b')));
			int left=-2600-15*error;
		int right=-2600+15*error;	
		if(-3000<left<-2000)
		{
					powerLeft(left);
		}
		else if(left<=-3000)
		{
			powerLeft(-3000);
		}
		else
		{
			powerLeft(-2000);
		}
	
		if(-3000<right<-2000)
		{
			powerRight(right);
		}
		else if(right<=-3000)
		{
			powerRight(-3000);
		}
		else
		{
			powerRight(-2000);
		}
		
		}
	}
	stop();
	return;
}


void bottomline_straight(){
	DelayMs(200);
	
	float angle = setRef(carCoorX,carCoorY,ballCoorX,ballCoorY,'f');
	if( ballCoorY>carCoorY)
	{
		while( calDistance(carCoorX,carCoorY,ballCoorX,ballCoorY) > 50)
	{
		int error =calError(angle, 'f');
		powerLeft(3000-18*error);
		powerRight(3000+18*error);
	}
	DelayMs(250);
	}
	else if( ballCoorY<carCoorY)
	{
		while( calDistance(carCoorX,carCoorY,ballCoorX,ballCoorY) > 50)
	{
		int error =calError(angle, 'f');
		powerLeft(3000+18*error);
		powerRight(3000-18*error);
	}
	DelayMs(250);
	}
	else 
	{
	}
	
	
/*	
	if(carCoorY>ballCoorY){
		speed=2400;
					expected=carCoorX;
		while(carCoorY>(ballCoorY+10)){
			
			powerLeft(2200-K1*(carCoorX-expected));
			powerRight(2800+K1*(carCoorX-expected));
		}
	}
	else{
		speed=2400;
					expected=carCoorX;
		while(carCoorY<(ballCoorY-25)){
			
			powerLeft(2200+16*(carCoorX-expected));
			powerRight(2800-16*(carCoorX-expected));
		}
	}
	*/
	stop();
	DelayMs(300);
}

void straight(){

					while( calDistance(carCoorX,carCoorY,ballCoorX,ballCoorY) > 50){
		powerLeft(3000-18*(carCoorY-ballCoorY));
		powerRight(3000+18*(carCoorY-ballCoorY));
		//DelayMs(500);
		//tune();
	}
	stop();
	//DelayMs(200);
	return;
}

void beat(){
	while((carCoorX-ballCoorX<0) && carCoorX<500)
	{
	if(ballCoorX-carCoorX>=120) break;
	powerLeft(3500-15*(carCoorY-ballCoorY));
	powerRight(380+15*(carCoorY-ballCoorY));
	}
	
	stop();
	//DelayMs(300);
	return;
}

int abs(int a)
{
	if(a<0)
		return -1*a;
	else
		return a;
}

void tune(char option)
{
	int cError=carCoorY-ballCoorY;
	int pError=0;
	if(option=='r')
	{
		while(1)
	{
		powerLeft(-3500);
		powerRight(3500);
		DelayMs(100);
		stop();
		DelayMs(50);
		pError=cError;
		cError=carCoorY-ballCoorY;
		if(pError*cError>0)
			break;
	}
	}
	
	else if(option=='l')
	{
		while((carCoorY-ballCoorY)<20)
	{
		powerLeft(3500);
		powerRight(-3500);
		DelayMs(100);
		stop();
		DelayMs(50);
		pError=cError;
		cError=carCoorY-ballCoorY;
		if(pError*cError>0)
			break;
	}
	
	}
	else
		return;
}

void anotherTune(char option)
{
	int turnTime=80;
	int turnSpeed=3500;
	
	if(option=='r')
	{
		while((carCoorX-ref)>5)
	{
		powerLeft(turnSpeed);
		powerRight(-1*turnSpeed);
		DelayMs(160);
		stop();
		DelayMs(80);
	}
	while((carCoorX-ref)<-5)
	{
		powerLeft(-1*turnSpeed);
		powerRight(turnSpeed);
		DelayMs(160);
		stop();
		DelayMs(80);
	}
}
	
	else if(option =='l' )
	{
		while((carCoorX-ref)>5)
	{
		powerLeft(-1*turnSpeed);
		powerRight(turnSpeed);
		DelayMs(160);
		stop();
		DelayMs(80);
	}
	while((carCoorX-ref)<-5)
	{
		
		powerLeft(turnSpeed);
		powerRight(-1*turnSpeed);
		DelayMs(160);
		stop();
		DelayMs(80);
	}
	}
	
}

void WIFIInit()
{
	DelayMs(2000);
	DelayMs(2000);
	DelayMs(2000);
	send(message2);
	DelayMs(2000);
	return;
}

void procedure(){
	
	ref=carCoorX-15;
	
	if((carCoorY-ballCoorY)<-initial_threshold){
		anotherTune('r');
		bottomline_straight();
		tune('r');
	}

	else if((carCoorY-ballCoorY)>initial_threshold){
		anotherTune('l');
		bottomline_straight();
		tune('l');
	}
	else
	{
		straight();
		if(carCoorY>ballCoorY)
		{
			tune('r');
		}
		else
		{
			tune('l');
		}
	}

	DelayMs(200);
	//straight();
	beat();
	backToBottom();
	return;
}

