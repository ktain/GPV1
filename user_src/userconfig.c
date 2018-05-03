#include "stm32f4xx.h"
#include <stdbool.h>
#include <string.h>

#include "userconfig.h"
#include "setup.h"
#include "motorcontrol.h"
#include "camera.h"
#include "display.h"
#include "menu.h"

/*
 * loadSettings() - Load settings before setting up peripherals
 */
void loadSettings(void) {
	
	// Motor settings
	counts_per_mm = 68.9;
	kpX = 0.3;
	kdX = 0.5;
	targetAccX = 5.0;
	targetDecX = 5.0;
	motorMaxPwm = 900;
	
	minExposureTime_us = 1000;	// min exposure time in us
	maxExposureTime_us = 20000;	// max exposure time in us (max 65536)
	targetPixelVal = 300;		// target for max pixel value (max 4095)
	servo_period_us = 3000;		// servo period in us (max 65536)
	min_line_width = 3;
	max_line_width = 40;
	centerRange = 20;		// +-20

	// NearCam (single cam) settings
	nearCamOnTime = 1800;			// Lower == see further
	nearCamMinOnTime = 1600;	// How far
	nearCamMaxOnTime = 2100;	// How close
	
	// FarCam settings
	farCamOnTime = 1420;		// Lower == move right
	farCamMinOnTime = 1420;	//920
	farCamMaxOnTime = 1420;	//1920
	
	// Steering settings
	steeringCenterOnTime = 1480;	// Lower == move left more
	steeringMinOnTime = 1180;
	steeringMaxOnTime = 1780;
	steering_Kp = 5.0;
	steering_Kd = 0.8;
	
	// Display settings
	displayDelay_ms = 200;
	displayBrightness = 10;	//0-15
	
	// Menu settings
	numOptions = countOptions();
	ticksPerOption = 1000;
	
}


int32_t countOptions(void) {
	uint32_t count = 0;
	while(strcmp(options[count], "") != 0) {
		count++;
	}
	return count;
}
