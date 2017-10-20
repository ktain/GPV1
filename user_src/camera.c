#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "setup.h"
#include "delay.h"
#include "camera.h"

volatile int32_t scanBuf[128];
volatile float lineIndex = 0;
volatile int32_t integrationTime = 0;
volatile int32_t integrationInterval = 0;
volatile int32_t startTime = 0;

/*
 * readCameraStart() - Start the camera read routine
 */
void readCameraStart(void)
{
	startTime = micros();
	scanLine();
	
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
	elapse_us(integrationTime, startTime);
	
	// Read values into buffer
	SI_HI;
	CLK_HI;
	delay_us(10);
	scanBuf[0] = read_Out1;
	SI_LO;
	CLK_LO;
	for (int32_t i = 1; i < 128; i++) {
		CLK_HI;
		scanBuf[i] = read_Out1;
		CLK_LO;
	}
	
}

/*
 * readCamera() - Start, wait, and stop the camera read routine
 */
void readCamera(void)
{
	readCameraStart();
	readCameraStop();
}

void scanLine(void) {
	// Scan for line
	int32_t maxIndex = 0;
	int32_t maxVal = 0;
	for (int32_t i = 0; i < 128; i++) {
		if (scanBuf[i] > maxVal) {
			maxVal = scanBuf[i];
			maxIndex = i;
		}
	}
	
	lineIndex = maxIndex;
}
