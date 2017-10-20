/**
 * Filename: setup.c
 * 
 * Peripheral setup routines
 */

/* Peripheral includes */
#include "stm32f4xx.h"

/* User includes */
#include "setup.h"
#include "delay.h"
#include "camera.h"

/*
 * setup() - calls all other peripheral setup routines
 */
void setup() 
{
	systick_setup();
	led_setup();
	button_setup();
	usart_setup();
	adc_setup();
	motor_setup();
	encoder_setup();
	camera_setup();
}


/*
 * systick_setup() - kernel periodic interrupt
 * 
 * The SysTick timer is a 24-bit countdown timer.
 * SysTick_Handler is invoked when the timer reaches 0.
 * The RELOAD value should not exceed 0xFFFFFF (16,777,215). 
 */
static void systick_setup(void) 
{
	/* Set an interrupt every TICKS_PER_MS number of ticks */
	SysTick_Config(TICKS_PER_MS);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// 4 bits preempt, 0 bits subpriority
	NVIC_SetPriority(SysTick_IRQn, 0);		// lower == higher priority
}


/*
 * led_setup() - Setup the LEDs
 */
static void led_setup(void) 
{
	/* Declare a GPIO struct to work with*/
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 
	 * Connect the clock to the GPIO ports to be used.
	 * On the STM32F4XX, the GPIO ports are connected to the AHB1 bus.
	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*
	 * GPIO_Pin - specify the pin to modify (specific, all)
	 * GPIO_Mode - pin-specific mode (digital out, digital in, analog, alternate)
	 * GPIO_OType - hardware configuration (push-pull, open-drain)
	 * GPIO_Speed - GPIO clock speed (2, 25, 50, 100MHz)
	 * GPIO_PuPd - internal configuration (pull-up, pull-down, neither)
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/* 
 * button_setup() - Setup user pushbuttons 
 */
static void button_setup(void) 
{
	/* Declare EXTI, GPIO, and NVIC 32-bit structs to work with */
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Connect the clock to the GPIO ports used by buttons */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	/* Connect the clock to SYSCFG used by EXTI line*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Configure GPIO settings for button0 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	// Update registers
	
	/* Map EXTI line to button0 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
	
	/* Configure EXTI line for button 0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// Update registers
	
	/* Configure NVIC channel for button0 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;	// Lowest priority = 0x0F
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		// Highest priority = 0x00
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* Configure GPIO settings for button1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	// Update registers
	
	/* Map the EXTI line to button1 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);

	/* Configure EXTI line for button 1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// Update registers
	
	/* Configure NVIC channel for button1 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;	// Lowest priority = 0x0F
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		// Highest priority = 0x00
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 
/* 
 * usart_setup() - Setup serial port communication
 */
static void usart_setup(void) 
{
	/* Declare GPIO and USART 32-bit structs to work with */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	
	/* Connect the clock to the USART GPIO port used */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* Connect the Clock to USART1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Configure TX pin as alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	
	/* Configure RX pin as alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	/* Configure GPIO pins for USART */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	// Update registers
	
	/* Configure USART */
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);	// Update registers
	
	/* Configure USART clock */
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_High;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
	USART_ClockInit(USART1, &USART_ClockInitStructure);	// Update registers

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

/* 
 * adc_setup() - Setup ADCs
 */
static void adc_setup(void)
{
	/* Declare ADC and GPIO structs to work with */
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Connect the clock to the ADC and GPIO ports used */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
	
	/* Configure GPIOB for ADC */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure GPIOC for ADC */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Configure ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	/* Configure ADC */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

/* 
 * readADC() - reads and returns ADC values from the master ADC (ADC1)
 * http://www.micromouseonline.com/2009/05/26/simple-adc-use-on-the-stm32/
 */
uint32_t readADC(uint32_t channel)
{
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

/* 
 * motor_setup() - setup DC motor servos
 */
static void motor_setup(void)
{
	/* Declare structs to work with */
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* Servo Enable Setup */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Servo PWM Setup */
	/* Connect the clock to the timer used */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* Configure GPIOB */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/* Connect TIM4 pins to AF */  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

	
	//TIM clock = APB clock * 2 = 84MHz
	//Tim clock = (period+1) * (prescaler+1) * PWM_frequency
	//PWM frequency = 84000000 / (19999+1) / (83+1) = 50Hz

	/* Configure TIM Base */
	TIM_TimeBaseStructure.TIM_Period = (20000) - 1;	// scale PWM duty cycle from 0-19999 ms
	TIM_TimeBaseStructure.TIM_Prescaler = (84) - 1;	// 84=50Hz servo updates
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
	
	
	/* Motor Enable Setup */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/* Motor PWM Setup */
	/* Connect the clock to the timer used */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Configure GPIOA */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	/* Connect TIM9 pins to AF */  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	
	//TIM clock = APB clock * 2 = 84MHz
	//Tim clock = (period+1) * (prescaler+1) * PWM_frequency
	//PWM frequency = 84000000 / (999+1) / (3+1) = 21000Hz = 21KHz

	/* Configure TIM Base */
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 3;	// 2=28kHz, 3=21kHZ, 4=16.8kHz 20=4khZ, 41=2kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

/* 
 * encoder_setup() - Setup quadrature encoder
 */
static void encoder_setup(void)
{
	/* Declare GPIO and TIM struct to work with */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Connect the clock to the timers used */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	/* Configure GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	
	TIM_SetAutoreload (TIM5, 0xffffffff);	// 32-bit counter on TIM2

	/* Configure quadrature decoding */
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	
	/* Enable TIM counter */
	TIM_Cmd(TIM5, ENABLE);
}

/* 
 * camera_setup() - Setup line scan camera and integration time
 */
static void camera_setup(void) 
{
	/* Declare a GPIO struct to work with*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 
	 * Connect the clock to the GPIO ports to be used.
	 * On the STM32F4XX, the GPIO ports are connected to the AHB1 bus.
	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*
	 * GPIO_Pin - specify the pin to modify (specific, all)
	 * GPIO_Mode - pin-specific mode (digital out, digital in, analog, alternate)
	 * GPIO_OType - hardware configuration (push-pull, open-drain)
	 * GPIO_Speed - GPIO clock speed (2, 25, 50, 100MHz)
	 * GPIO_PuPd - internal configuration (pull-up, pull-down, neither)
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	/* Declare TIM, and NVIC struct to work with */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;

	/* Connect the APB1 clock to the timers used */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// Enabling global interrupt
	NVIC_Init(&NVIC_InitStructure);					// Initializing NVIC structure

	//PWM frequency = 84000000 / (period + 1) / (prescaler + 1)
	
	/* Configure TIM Base */
	TIM_TimeBaseStructure.TIM_Period = (integrationInterval_us) - 1;	// stay between (20Hz-1kHz) [50000:1000] 
	TIM_TimeBaseStructure.TIM_Prescaler = (84) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM3, ENABLE);				// Enable timer
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	// Enable timer interrupts
}
