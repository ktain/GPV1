#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include <stdbool.h>

#include "setup.h"
#include "delay.h"
#include "camera.h"

volatile int32_t scanBuf[128];
volatile float linePos = 0;
volatile int32_t integrationTime_us = 0;
volatile int32_t startTime_us = 0;
volatile bool isIntegrating = 0;

int32_t nearCamPwm;
int32_t farCamPwm;
int32_t nearCamMinPwm;
int32_t nearCamMaxPwm;
int32_t farCamMinPwm;
int32_t farCamMaxPwm;

/*
 * readCameraStart() - Start the camera read routine
 */
void readCameraStart(void)
{	
	// Flush garbage values
	SI_HI;
	CLK_HI;
	SI_LO;
	CLK_LO;
	for (int32_t i = 0; i < 17; i++) {
		CLK_HI;
		CLK_LO;
	}
	
	// Begin integrating
}

/*
 * readCameraStop() - Wait and stop the camera read routine
 */
void readCameraStop(void)
{
	for (int32_t i = 0; i < 111; i++) {
		CLK_HI;
		CLK_LO;
	}
	
	// Wait for remaining integration time
	// elapse_us(integrationTime_us, startTime_us);
	
	// Read values into buffer
	SI_HI;
	CLK_HI;
	delay_us(10);
	scanBuf[0] = read_AO1;	// AO normal
	SI_LO;
	CLK_LO;
	for (int32_t i = 1; i < 128; i++) {
		CLK_HI;
		scanBuf[i] = read_AO1;
		CLK_LO;
	}
	
}

/*
 * readCamera() - Start, wait, and stop the camera read routine using systick
 */
void readCamera(void)
{
	int32_t elapsedTime_us = micros() - startTime_us;
	if (!isIntegrating) {
		startTime_us = micros();
		isIntegrating = 1;
		readCameraStart();
	}
	else if (elapsedTime_us >= integrationTime_us) {
		readCameraStop();
		detectLinePos();
		isIntegrating = 0;
	}
}

/*
 * detectLinePos() - Detect and update line position 
 */
void detectLinePos(void) {
	// Spatial moving average filter
	moving_avg_filter(scanBuf, 128);
	
	// Detect line position
	int32_t maxIndex = 0;
	int32_t maxVal = 0;
	for (int32_t i = 0; i < 128; i++) {
		if (scanBuf[i] > maxVal) {
			maxVal = scanBuf[i];
			maxIndex = i;
		}
	}
	
	linePos = maxIndex;
}

// Set nearCam tilt servo on-time in microseconds
void setNearCamPwm(int32_t pwm)
{
	if (pwm > nearCamMaxPwm)
		pwm = nearCamMaxPwm;
	else if (pwm < nearCamMinPwm)
		pwm = nearCamMinPwm;
	
	SERVO1_PWM = pwm;
}

// Set farCam tilt servo on-time in microseconds 
void setFarCamPwm(int32_t pwm)
{
	if (pwm > farCamMaxPwm)
		pwm = farCamMaxPwm;
	else if (pwm < farCamMinPwm)
		pwm = farCamMinPwm;
	
	SERVO2_PWM = pwm;
}

// spatial moving average filter
void moving_avg_filter (volatile int32_t *arr, int32_t size) {
	int32_t i;
	// perform spatial moving average filter
	for (i = 1; i < size-1; i++) {
		arr[i] = (arr[i-1] + arr[i] + arr[i+1])/3;
	}
	arr[0] = arr[1];
	arr[size-1] = arr[size-2];
}

