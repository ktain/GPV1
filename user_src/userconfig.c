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
	targetAccX = 5.0;
	targetDecX = 5.0;
	motorMaxPwm = 500;
	
	integrationTime_us = 20000;	// integration time in us < interval
	
	// Camera1 tilt settings
	camera1Pwm = 2250;		// Lower == see further 1650
	camera1MinPwm = 1500;
	camera1MaxPwm = 2250;
	
	// Camera2 tilt settings
	camera2Pwm = 1420;		// Lower == move right
	camera2MinPwm = 1420;	//920
	camera2MaxPwm = 1420;	//1920
	
	// Steering settings
	steeringCenterPwm = 1460;	// Lower == move left more
	steeringMinPwm = 1140;
	steeringMaxPwm = 1760;
	steering_Kp = 1.0;
	steering_Kd = 0.0;
	
	// Display settings
	displayDelay_ms = 200;
	displayBrightness = 10;	//0-15
}

