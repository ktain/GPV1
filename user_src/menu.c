#include <stdbool.h>

#include "stm32f4xx.h"
#include "menu.h"
#include "delay.h"
#include "encoder.h"
#include "display.h"
#include "userconfig.h"
#include "camera.h"
#include "motorcontrol.h"
#include "setup.h"

int32_t numOptions;
int32_t menuEncCount = 0;
int32_t ticksPerOption;
int32_t option = 0;
bool busy = 0;
bool isRunning = 0;

char* options[] = {
	"Run 1.0",	// 0
	"Run 2.0",
	"Run 3.0",
	"Run 3.5",
	"Run 4.0",
	"Run 4.5",	// 5
	"Run 5.0",
	"Run 5.5",
	"See closer",
	"See further",
	"",		// end of options
};

// Option refresh tracker
void refreshOptions(void) {
	bool needToRefresh = 0;
	if (getEncCount() - menuEncCount > ticksPerOption) {
		option++;
		if (option > numOptions - 1) {
			option = 0;
		}
		menuEncCount = getEncCount();
		needToRefresh = 1;
	}
	else if (getEncCount() - menuEncCount < -ticksPerOption) {
		option--;
		if (option < 0) {
			option = numOptions - 1;
		}
		menuEncCount = getEncCount();
		needToRefresh = 1;
	}
	
	if (needToRefresh) {
		display(options[option]);
	}

}

void enter(void) {
	busy = 1;
	
	switch(option) {
		case 0: // Run 1.0
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime); 
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 2.0;
			turnSpeed = 1.0;
			enableSpeedControl();
			break;
		case 1:	// Run 2.0
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime); 
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 3.0;
			turnSpeed = 2.0;
			enableSpeedControl();
			break;
		case 2:	// Run 3.0
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 4.0;
			turnSpeed = 3.0;
			enableSpeedControl();
			break;
		case 3:	// Run 3.5
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 4.5;
			turnSpeed = 3.5;
			enableSpeedControl();
			break;
		case 4: // Run 4.0
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 5.0;
			turnSpeed = 4.0;
			enableSpeedControl();
			break;
		case 5:	// Run 4.5
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 5.5;
			turnSpeed = 4.5;
			enableSpeedControl();
			break;
		case 6:	// Run 5.0
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 6.0;
			turnSpeed = 5.0;
			enableSpeedControl();
			break;
		case 7: // Run 5.5
			delay_ms(500);
			isRunning = 1;
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			straightSpeed = 6.5;
			turnSpeed = 5.5;
			enableSpeedControl();
			break;
		case 8:	// Adjust camera tilt closer
			nearCamOnTime += 50;
			setNearCamOnTime(nearCamOnTime);
			displayInt(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			break;
		case 9: // Adjust camera tilt further
			nearCamOnTime -= 50;
			setNearCamOnTime(nearCamOnTime);
			displayInt(nearCamOnTime);
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			break;
		
		default:
			break;
	}
	
	display(options[option]);
	busy = 0;
}
