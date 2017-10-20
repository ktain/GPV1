/**
 * Filename: usart.c
 *
 * Provides functions for printing to the serial port.
 * Overrides libc fputc() and fgetc() to redirect output
 * to the serial port.
 */

/* Peripheral includes */
#include "stm32f4xx.h"

/* libc includes */
#include <stdio.h>

/* Local FILE struct for custom fputc() and fgetc() */
struct __FILE 
{
	int32_t x;
};

/* Local stdout and stdin FILE structs */
FILE __stdout;
FILE __stdin;

/*
 * _sys_exit() - library exit function
 *
 * Must not return
 */
void _sys_exit(int32_t x)
{
	x = x;
}

/*
 * fputc() - sends a char to the serial port
 *
 * Busy wait until the serial TX port is empty
 * and transmit the char over the USART peripheral
 *
 * Return the char sent
 */
int32_t fputc(int32_t c, FILE *f) 
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
		;	// busy wait
	}
	USART_SendData(USART1, (uint8_t)c);
	return c;
}

/* 
 * fgetc() - reads a char from the serial port
 * 
 * Busy wait until the serial RX port is empty
 * and receive the char over the USART peripheral
 *
 * Return the char read
 */
int32_t fgetc(FILE *f) 
{
	uint16_t tmp;
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {
		;	// busy wait
	}
	tmp = USART_ReceiveData(USART1);
	return tmp;
}
