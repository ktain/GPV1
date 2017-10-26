#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include <stdlib.h>

#include "setup.h"
#include "delay.h"
#include "camera.h"
#include "display.h"

volatile int32_t scanBuf[128];
volatile float linePos = 0;
volatile int32_t exposureTime_us = 0;
volatile int32_t startTime_us = 0;
volatile _Bool isIntegrating = 0;

int32_t nearCamPwm;
int32_t farCamPwm;
int32_t nearCamMinPwm;
int32_t nearCamMaxPwm;
int32_t farCamMinPwm;
int32_t farCamMaxPwm;

/*
 * readCameraStart() - Start the camera read routine
 *	Timing is important 
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
	
	// Now integration begins
}

/*
 * readCameraStop() - Stop the camera read routine
 */
void readCameraStop(void)
{
	int32_t i;
	for (i = 0; i < 111; i++) {
		CLK_HI;
		CLK_LO;
	}
	
	// Read values into buffer
	SI_HI;
	CLK_HI;
	delay_us(10); 		// Necessary delay for first pixel
	scanBuf[0] = read_AO1;	// AO normal, Out processed
	SI_LO;
	CLK_LO;
	for (i = 1; i < 128; i++) {
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
	else if (elapsedTime_us >= exposureTime_us) {
		readCameraStop();
		detectLinePos();
		isIntegrating = 0;
	}
}

// Automatic exposure time adjustment
void updateExposureTime(void)
{	
	// Adaptive exposure time
	int32_t maxVal = 0;
	for (int i = 0; i < 128; i++) {
		if (scanBuf[i] > maxVal)
			maxVal = scanBuf[i];
	}
	if (maxVal > 3000)
		exposureTime_us -= 100;
	else 
		exposureTime_us += 100;
	
	if (exposureTime_us > 20000)
		exposureTime_us = 20000;
	else if (exposureTime_us < 1000)
		exposureTime_us = 1000;
}

/*
 * detectLinePos() - Detect and update line position 
 */
void detectLinePos(void) {
	static int32_t old_pos = 63;
	int32_t threshold;
	
	// Spatial moving average filter
	//movingAvgFilter(scanBuf, 128);
	
	// Calculate threshold
	int32_t maxVal = scanBuf[0];
	int32_t minVal = scanBuf[0];
	for (int32_t i = 0; i < 128; i++) {
		if (scanBuf[i] > maxVal) {
			maxVal = scanBuf[i];
		}
		if (scanBuf[i] < minVal) {
			minVal = scanBuf[i];
		}
	}
	threshold = 0.3*(maxVal - minVal) + minVal;
	
	linePos = getNearestPeak(scanBuf, 128, threshold, old_pos);
	old_pos = linePos;
	
	/*
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
	*/
}

// Set nearCam tilt servo on-time in microseconds
void setNearCamPwm(int32_t pwm)
{
	if (pwm > nearCamMaxPwm)
		pwm = nearCamMaxPwm;
	else if (pwm < nearCamMinPwm)
		pwm = nearCamMinPwm;
	
	NEAR_CAM_PWM = pwm;
}

// Set farCam tilt servo on-time in microseconds 
void setFarCamPwm(int32_t pwm)
{
	if (pwm > farCamMaxPwm)
		pwm = farCamMaxPwm;
	else if (pwm < farCamMinPwm)
		pwm = farCamMinPwm;
	
	FAR_CAM_PWM = pwm;
}

// Spatial moving average filter
void movingAvgFilter(volatile int32_t *arr, int32_t size) {
	int32_t i;
	
	// perform spatial moving average filter
	for (i = 1; i < size-1; i++) {
		arr[i] = (arr[i-1] + arr[i] + arr[i+1])/3;
	}
	arr[0] = arr[1];
	arr[size-1] = arr[size-2];
}

// Get nearest peak
int32_t getNearestPeak(volatile int32_t *arr, int32_t size, int32_t threshold, int32_t old_pos) {
	static int32_t line_pos = 63;
	int32_t min_width = 5;
	int32_t max_width = 40;
	int32_t lhs = 0;
	int32_t rhs = 0;
	int32_t min_dist_from_old_pos = 127;
	int32_t peak_count = 0;
	
	for (int32_t i = 0; i < 128; i++) {
		if (arr[i] >= threshold && i > rhs) {
			lhs = i;
			for (int32_t j = lhs; j < 128; j++) {
				if (arr[j] < threshold || j >= 127) {
					rhs = j;
					peak_count++;
					break;
				}
			}
		}
		if ((rhs-lhs >= min_width) && (rhs-lhs <= max_width) && 
		    (abs(lhs + (rhs-lhs)/2 - old_pos) < abs(min_dist_from_old_pos))) {
			line_pos = lhs + (rhs-lhs)/2;
			min_dist_from_old_pos = abs(lhs + (rhs-lhs)/2 - old_pos);
		}
	}
	return line_pos;
}

