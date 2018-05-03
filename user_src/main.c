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
	
	if (isRunning) {
		// go faster on straights
		if (linePos > 64-centerRange/2 && linePos < 64+centerRange/2) {
			setSpeedX(straightSpeed);
		}
		else {
			setSpeedX(turnSpeed);
		}
	}
		
	if (useSpeedControl) {
		updateSpeed();
		speedControl();
	}

	updateSteeringAngle();
	
	// Process UART commands
	static uint16_t tmp;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
		tmp = USART_ReceiveData(USART1);
		// Kill switch
		if (tmp == 's') {
			isRunning = 0;
			STEERING_PWM = 0;
			setSpeedX(0);
			disableSpeedControl();
		}
	}
	
	// Refresh display
	if (!busy && !isRunning) {
		refreshOptions();
		updateDisplay();
	}
	
	int32_t elapsedTime_us = micros() - startTime_us;
}

void button0(void)
{
	enter();
}

void button1(void)
{
	isRunning = 0;
	setSpeedX(0);
	delay_ms(1000);
	STEERING_PWM = 0;
	SERVO_OFF;
	disableSpeedControl();

}

int main(void)
{
	loadSettings();
	setup();
	
	displayVoltage();
	
	while(1) {
		delay_ms(100);
		
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
		
		//printf("numOptions %d\n\r", numOptions);
		
		// Max AO1 value
		int32_t maxVal = 0;
		for (int i = 0; i < 128; i++) {
			if (scanBuf[i] > maxVal)
				maxVal = scanBuf[i];
		}
		
		printf("volt %5d|IS %d|AO1 %4d|Out1 %4d|AO2 %4d|Out2 %4d|enc %6d|linePos %.2f|exposure %5dus|maxVal %4d|motorPwm %3.1f|steeringOnTime %4.1f\n\r", 
						read_Voltage, read_Current, read_AO1, read_Out1, read_AO2, read_Out2, getEncCount(), linePos, exposureTime_us, maxVal, motorPwm, steeringOnTime);
	}
}
