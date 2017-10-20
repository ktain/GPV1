#if !defined(SETUP_H)
#define SETUP_H

#define TICKS_PER_MS (168000)
#define TICKS_PER_US (168)
#define BAUDRATE (921600)	// Serial data rate

/* LED MACRO Control */
#define LED1_ON		GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define LED2_ON		GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define LED3_ON		GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define LED4_ON		GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define LED1_OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define LED2_OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define LED3_OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define LED4_OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define ALL_LED_ON	LED1_ON; LED2_ON; LED3_ON; LED4_ON
#define ALL_LED_OFF	LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF

/* ADC Pinout */
#define read_Voltage	((int32_t)(readADC(10)*3.22))
#define read_Current	readADC(12)
#define read_AO1	readADC(14)
#define read_Out1	readADC(15)
#define	read_AO2	readADC(8)
#define read_Out2	readADC(9)

/* Motor MACRO control */
#define MTR_ON		GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define MTR_OFF		GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define SERVO_ON	GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define SERVO_OFF	GPIO_ResetBits(GPIOB, GPIO_Pin_3)
#define MTR_PWM		(TIM2->CCR3)
#define SERVO1_PWM	(TIM4->CCR4)
#define SERVO2_PWM	(TIM4->CCR3)
#define SERVO3_PWM	(TIM4->CCR2)

/* Camera MACRO control*/
#define CLK_HI		GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define CLK_LO		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define SI_HI		GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define SI_LO		GPIO_ResetBits(GPIOC, GPIO_Pin_8)

void setup(void);
void systick_setup(void);
void led_setup(void);
void button_setup(void);
void usart_setup(void);
void adc_setup(void);
void motor_setup(void);
void encoder_setup(void);
void camera_setup(void);
void interrupt_setup(void);
uint32_t readADC(uint32_t channel);

extern volatile int32_t voltage;

#endif
