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

int32_t menuEncCount = 0;
int32_t option = 0;
bool busy = 0;

char* options[] = {
	"0", // 0
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",	// 7
	"8",
	"9",
};

// Option refresh tracker
void refreshOptions(void) {
	bool needToRefresh = 0;
	if (getEncCount() - menuEncCount > 2000) {
		option++;
		if (option > NUMOPTIONS - 1) {
			option = 0;
		}
		menuEncCount = getEncCount();
		needToRefresh = 1;
	}
	else if (getEncCount() - menuEncCount < -2000) {
		option--;
		if (option < 0) {
			option = NUMOPTIONS - 1;
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
	delay_ms(500);
	
	switch(option) {
		case 0: 
			SERVO_ON;
			setNearCamOnTime(nearCamOnTime); 
			delay_ms(500);
			NEAR_CAM_PWM = 0;
			MTR_ON;
			enableSpeedControl();
			setSpeedX(1.0);
		
			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:
			
			break;
		case 6:
		
			break;
		case 7: 
		
			break;
		case 8:	
			
			break;
		case 9:
			
			break;
		case 10:
			break;
		
		default:
			break;
	}
	
	display(options[option]);
	busy = 0;
}
