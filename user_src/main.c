#include "stm32f4xx_it.h"
#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "userconfig.h"
#include "setup.h"
#include "delay.h"
#include "encoder.h"
#include "motorcontrol.h"
#include "camera.h"
#include "display.h"
#include "menu.h"

void systick(void) 
{
	int32_t startTime_us = micros();
	
	if (useSpeedControl) {
		updateSpeed();
		speedControl();
	}

	updateSteeringAngle();
	updateDisplay();

	
	// Process UART commands
	static uint16_t tmp;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
		tmp = USART_ReceiveData(USART1);
		// Kill switch
		if (tmp == 's') {
			STEERING_PWM = 0;
			setSpeedX(0);
			disableSpeedControl();
		}
	}
	
	static int32_t maxTime_us = 0;
	
	// Refresh display
	if (!busy) {
		refreshOptions();
	}
	
	
	// display 
	static int i = 0;
	if (i < 200)
		i++;
	else {
		//displayInt(TIM3->CNT);
		//displayInt(maxTime_us);
		//displayVoltage();
		//displayInt(motorPwm);
		//displayInt(EXPOSURE_TIME/1000);
		//displayInt((int)steeringError);
		i = 0;
	}
	
	
	int32_t elapsedTime_us = micros() - startTime_us;
	maxTime_us = (elapsedTime_us > maxTime_us)? elapsedTime_us:maxTime_us;
	
}

void button0(void)
{
	delay_ms(500);
	
	enter();
	
	/*
	while(1) {
		int32_t temp = linePos;
		printf("|");
		for (int32_t i = 0; i < 128; i++) {
			if (i == temp)
				printf(".");
			else
				printf(" ");
		}
		printf("|");
		printf("\r\n");
	}
	*/

	/*
	int32_t scanBufShot[128];
	while(1) {
		int32_t i;
		for (i = 0; i < 128; i++)
			scanBufShot[i] = scanBuf[i];
		
		for (int32_t i = 0; i < 128; i++) {
			printf("%d %d\n", i, scanBufShot[i]);
		}
		delay_ms(20);
	}
	*/

	/*
	printf("\r\n\n");
	printf("\r\n\n");
	int32_t scanBufShot[128];
	int32_t i;
	for (i = 0; i < 128; i++)
		scanBufShot[i] = scanBuf[i];
	for (i = 0; i < 128; i++) {
		for (int32_t j = 0; j < scanBufShot[i]/10; j++) {
			printf("0");
		}
		printf("\r\n");
	}
	for(i = 0; i < 20; i++)
		printf("%-9d|", i*100);
	printf("%-9d|", i*100);
	printf("\r\n");
	*/
	
	/*
	printf("\r\n\n");
	for (int32_t i = 0; i < 128; i++) {
		printf("%2d ", scanBuf[i]);
	}
	*/
	
	/*
	MTR_ON;
	SERVO_ON;
	enableSpeedControl();

	//setSpeedX(6.0);
	//setPwm(100);
	int32_t centerPwm = 1460;
	setServo1Pwm(centerPwm);
	setServo2Pwm(centerPwm);
	setServo3Pwm(centerPwm);
	*/
}

void button1(void)
{
	//SERVO_OFF;
	STEERING_PWM = 0;
	setSpeedX(0);
	disableSpeedControl();
}

int main(void)
{
	loadSettings();
	setup();
	
	displayVoltage();
	
	while(1) {
		
		/*
		// Send camera values
		int32_t scanBufShot[128];
		int32_t i;
		for (i = 0; i < 128; i++)
			scanBufShot[i] = scanBuf[i];
		
		for (int32_t i = 0; i < 128; i++) {
			printf("%d %d\n", i, scanBufShot[i]);
		}
		delay_ms(100);
		*/
		
		//printf("%f\n\r", linePos);
		
		printf("volt %5d|IS %d|AO1 %4d|Out1 %4d|AO2 %4d|Out2 %4d|enc %6d|linePos %.2f\n\r", read_Voltage, read_Current, read_AO1, read_Out1, read_AO2, read_Out2, getEncCount(), linePos);
	}
}
