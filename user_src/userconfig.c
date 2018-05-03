#include "stm32f4xx.h"
#include <stdbool.h>

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
	motorMaxPwm = 999;
	
	minExposureTime_us = 1000;	// min exposure time in us
	maxExposureTime_us = 20000;	// max exposure time in us (max 65536)
	targetPixelVal = 300;		// target for max pixel value (max 4095)
	servo_period_us = 3000;		// servo period in us (max 65536)
	min_line_width = 3;
	max_line_width = 40;

	
	// NearCam (single cam) settings
	nearCamOnTime = 1900;		// Lower == see further
	nearCamMinOnTime = 1500;
	nearCamMaxOnTime = 2250;
	
	// FarCam settings
	farCamOnTime = 1420;		// Lower == move right
	farCamMinOnTime = 1420;	//920
	farCamMaxOnTime = 1420;	//1920
	
	// Steering settings
	steeringCenterOnTime = 1460;	// Lower == move left more
	steeringMinOnTime = 1140;
	steeringMaxOnTime = 1780;
	steering_Kp = 5.0;
	steering_Kd = 0.8;
	
	// Display settings
	displayDelay_ms = 200;
	displayBrightness = 10;	//0-15
	
	// Menu settings
	ticksPerOption = 1000;
	
}

