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

void systick(void) 
{
	if (useSpeedControl) {
		updateSpeed();
		speedControl();
	}
	updateServo2();
	updateServo3();
}

void button0(void)
{
	delay_ms(200);
	
	SERVO_ON;
	setServo1Pwm(servo1CenterPwm); 
	delay_ms(200);

	MTR_ON;
	
	enableSpeedControl();
	setSpeedX(0.0);
	
	/*
	while(1) {
		int32_t temp = lineIndex;
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
	SERVO_OFF;
	setSpeedX(0);
	//setPwm(0);
}

int main(void)
{
	loadSettings();
	
	setup();
	while(1) {
		//printf("volt %5d|IS %d|AO1 %4d|Out1 %4d|AO2 %4d|Out2 %4d|enc %6d\n\r", read_Voltage, read_Current, read_AO1, read_Out1, read_AO2, read_Out2, getEncCount());
		LED1_ON;
		delay_ms(500);
		LED1_OFF;
		delay_ms(500);
	}
}
