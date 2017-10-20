/**
 * Filename: delay.c
 *
 * Getter methods to keep track of system time delays
 */

/* Peripheral includes */
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* User includes */
#include "delay.h"
#include "setup.h"

/*
 * delay_ms() - Busy wait in milliseconds
 */
void delay_ms(uint32_t duration_ms)
{
	uint32_t start_time_ms = millis();
	while(millis() - start_time_ms < duration_ms) {
		;	// busy wait
	}
}

/*
 * delay_us() - Busy wait in microseconds
 *
 * Uses the Data Watchpoint and Trace Unit (DWT) to measure time in us
 */
void delay_us(uint32_t duration_us)
{
	volatile uint32_t cycles = duration_us*TICKS_PER_US;
	volatile uint32_t start = 0;
	
	// Set the Debug Exception and Monitor Control Register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= 1 ; // Enable the counter
	DWT->CYCCNT = 0; // Reset the counter

	while(DWT->CYCCNT < cycles) {
		;	// busy wait
	}

}

/*
 * elapse_ms() - Busy wait in ms
 *
 * Delay for a remaining amount of time in ms
 */
void elapse_ms(uint32_t duration_ms, uint32_t start_time_ms)
{
	delay_ms(duration_ms - (millis() - start_time_ms));
}

/*
 * elapse_us() - Busy wait in us
 *
 * Delay for a remaining amount of time in us
 */
void elapse_us(uint32_t duration_us, uint32_t start_time_us)
{
	int32_t curt_us = micros();
	if (duration_us > (curt_us - start_time_us))
		delay_us(duration_us - (micros() - start_time_us));
}

