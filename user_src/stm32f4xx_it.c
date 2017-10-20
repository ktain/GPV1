/**
 * Filename: stm324xx_it.c
 *
 * Interrupt handlers and system time getter methods
 */

/* Peripheral includes */
#include "stm32f4xx_it.h"

/* User includes */
#include "main.h"
#include "delay.h"
#include "setup.h"
#include "camera.h"
#include "motorcontrol.h"

/* Static variables */
volatile static uint32_t g_millis;	// 49.7 days until overflow

/*
 * millis() - returns system time in milliseconds
 */
inline uint32_t millis(void) 
{
	return g_millis;
}

/*
 * micros() - returns system time in microseconds
 *
 * Return the sum of scaled current time and the remaining time
 * needed before the next systick reload cycle
 */
inline uint32_t micros(void) 
{
	return g_millis*1000 + (SystemCoreClock/1000 - SysTick->VAL)/TICKS_PER_US;
}

/*
 * Systick_Handler() - SysTick interrupt handler
 *
 * Runs periodically whenever the systick timer value reaches 0
 * Updates system time variable and calls the user systick() function
 */
void SysTick_Handler(void)
{
	__disable_irq();	// Ensure that systick is atomic
	
	g_millis++;
	systick();
	
	__enable_irq();
}

/*
 * EXTI_9_5_IRQHandler() - Button interrupt handler
 */
void EXTI9_5_IRQHandler(void) 
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
		button0();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET) {
		button1();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

/*
 * TIM3_IRQHandler() - Line scan camera interrupt handler
 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		GPIO_ToggleBits(GPIOA, GPIO_Pin_7);
		readCamera();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
