#include "stm32f4xx.h"
#include <stdbool.h>

#include "setup.h"
#include "motorcontrol.h"
#include "camera.h"
#include "display.h"

/*
 * loadSettings() - Load settings before setting up peripherals
 */
void loadSettings(void) {
	
	counts_per_mm = 68.9;
	kpX = 0.3;
	kdX = 0.5;
	targetAccX = 5.0;
	targetDecX = 5.0;
	motorMaxPwm = 500;
	
	exposureTime_us = 20000;	// exposure time in us < interval (max 65536)
	servo_period_us = 3000;		// servo period in us (max 65536)
	
	// NearCam tilt settings
	nearCamOnTime = 2000;		// Lower == see further
	nearCamMinOnTime = 1500;
	nearCamMaxOnTime = 2250;
	
	// FarCam tilt settings
	farCamOnTime = 1420;		// Lower == move right
	farCamMinOnTime = 1420;	//920
	farCamMaxOnTime = 1420;	//1920
	
	// Steering settings
	steeringCenterOnTime = 1460;	// Lower == move left more
	steeringMinOnTime = 1140;
	steeringMaxOnTime = 1760;
	steering_Kp = 1.0;
	steering_Kd = 0.0;
	
	// Display settings
	displayDelay_ms = 200;
	displayBrightness = 10;	//0-15
}

