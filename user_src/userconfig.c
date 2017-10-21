#include "stm32f4xx.h"
#include <stdbool.h>

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
	targetAccX = 10.0;
	targetDecX = 10.0;
	
	integrationInterval_us = 5000;	// integration interrupt interval
	integrationTime_us = 4500;	// integration time in us < interval
	
	// Camera tilt settings
	servo1CenterPwm = 2100;		// Lower == see further 1650
	servo1MinPwm = 1500;
	servo1MaxPwm = 2100;
	
	// Camera pan settings
	servo2CenterPwm = 1420;		// Lower == move right
	servo2MinPwm = 1420;	//920
	servo2MaxPwm = 1420;	//1920
	pan_Kp = 0.0;
	pan_Kd = 0.0;
	
	// Steering settings
	servo3CenterPwm = 1460;		// Lower == move left
	servo3MinPwm = 1140;
	servo3MaxPwm = 1760;
	steering_Kp = 10.0;
	steering_Kd = 0.0;
	
	// Display settings
	displayDelay_ms = 200;
	displayBrightness = 4;	//0-15
}

