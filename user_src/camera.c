#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include <stdlib.h>
#include <stdbool.h>

#include "setup.h"
#include "delay.h"
#include "camera.h"
#include "display.h"

volatile int32_t scanBuf[128];
volatile float linePos = 0;
volatile int32_t exposureTime_us = 0;
volatile int32_t startTime_us = 0;
volatile bool isIntegrating = 0;
volatile int32_t centerRange;

int32_t minExposureTime_us;
int32_t maxExposureTime_us;
int32_t min_line_width;
int32_t max_line_width;
int32_t targetPixelVal;

int32_t nearCamOnTime;
int32_t farCamOnTime;
int32_t nearCamMinOnTime;
int32_t nearCamMaxOnTime;
int32_t farCamMinOnTime;
int32_t farCamMaxOnTime;

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
 * readCamera() - Stop, restart, and save the camera read routine
 */
void readCamera(void)
{
	readCameraStop();
	readCameraStart();
	detectLinePos(scanBuf, 128, min_line_width, max_line_width);
	updateExposureTime();
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
	if (maxVal > targetPixelVal)
		exposureTime_us -= 100;
	else 
		exposureTime_us += 100;
	
	if (exposureTime_us > maxExposureTime_us)
		exposureTime_us = maxExposureTime_us;
	else if (exposureTime_us < minExposureTime_us)
		exposureTime_us = minExposureTime_us;
	
	EXPOSURE_TIME = exposureTime_us;
}

/*
 * detectLinePos() - Detect and update line position given an array of pixel values
 */
void detectLinePos(volatile int32_t *arr, int32_t size, int32_t min_width, int32_t max_width) {
	static int32_t old_pos = 63;
	int32_t threshold;
	
	// Spatial moving average filter
	movingAvgFilter(arr, size);	// ~18us no optimizations
	movingAvgFilter(arr, size);
	
	// Calculate threshold
	int32_t maxVal = arr[0];
	int32_t minVal = arr[0];
	for (int32_t i = 0; i < size; i++) {
		if (arr[i] > maxVal) {
			maxVal = arr[i];
		}
		if (arr[i] < minVal) {
			minVal = arr[i];
		}
	}
	threshold = 0.9*(maxVal - minVal) + minVal;
	
	linePos = getNearestPeak(arr, 128, threshold, old_pos, min_width, max_width);
	old_pos = linePos;
	
	/*
	// Detect line position
	int32_t maxIndex = 0;
	int32_t maxVal = 0;
	for (int32_t i = 0; i < size; i++) {
		if (arr[i] > maxVal) {
			maxVal = arr[i];
			maxIndex = i;
		}
	}
	
	linePos = maxIndex;
	*/
}

// Set nearCam tilt servo on-time in microseconds
void setNearCamOnTime(int32_t onTime_us)
{
	if (onTime_us > nearCamMaxOnTime)
		onTime_us = nearCamMaxOnTime;
	else if (onTime_us < nearCamMinOnTime)
		onTime_us = nearCamMinOnTime;
	
	nearCamOnTime = onTime_us;
	NEAR_CAM_PWM = onTime_us;
}

// Set farCam tilt servo on-time in microseconds 
void setFarCamOnTime(int32_t onTime_us)
{
	if (onTime_us > farCamMaxOnTime)
		onTime_us = farCamMaxOnTime;
	else if (onTime_us < farCamMinOnTime)
		onTime_us = farCamMinOnTime;
	
	farCamOnTime = onTime_us;
	FAR_CAM_PWM = onTime_us;
}

// Spatial moving average filter
void movingAvgFilter(volatile int32_t *arr, int32_t size) {
	int32_t i;
	
	for (i = 1; i < size-1; i++) {
		arr[i] = (arr[i-1] + arr[i] + arr[i+1])/3;
	}
	arr[0] = arr[1];
	arr[size-1] = arr[size-2];
}

// Get nearest peak
int32_t getNearestPeak(volatile int32_t *arr, int32_t size, int32_t threshold, float old_pos, int32_t min_width, int32_t max_width) {
	float nearest_pos = old_pos;
	int32_t lhs = 0;
	int32_t rhs = 0;
	int32_t min_dist_from_old_pos = 127;
	int32_t peak_count = 0;
	
	// find LHS
	for (int32_t i = 0; i < 128; i++) {
		if (arr[i] >= threshold && i > rhs) {
			lhs = i;
			// find RHS
			for (int32_t j = lhs; j < 128; j++) {
				if (arr[j] < threshold || j >= 127) {
					rhs = j;
					peak_count++;
					break;
				}
			}
			if ((rhs-lhs >= min_width) && (rhs-lhs <= max_width) && 
			    (abs(lhs + (rhs-lhs)/2 - (int32_t)old_pos) < abs(min_dist_from_old_pos))) {
				nearest_pos = lhs + (rhs-lhs)/2;
				min_dist_from_old_pos = abs(lhs + (rhs-lhs)/2 - (int32_t)old_pos);
			}
		}
	}
	return nearest_pos;
}

