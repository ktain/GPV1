#include "stm32f4xx.h"

#include "setup.h"
#include "userconfig.h"
#include "motorcontrol.h"
#include "encoder.h"
#include "camera.h"

int32_t servo1CenterPwm;
int32_t servo2CenterPwm;
int32_t servo3CenterPwm;
int32_t servo1MinPwm;
int32_t servo1MaxPwm;
int32_t servo2MinPwm;
int32_t servo2MaxPwm;
int32_t servo3MinPwm;
int32_t servo3MaxPwm;

float pan_Kp;
float pan_Kd;
float steering_Kp;
float steering_Kd;

volatile float servo2Pwm;
volatile float servo2Error;
volatile float prevServo2Error;
volatile float servo3Pwm;
volatile float servo3Error;
volatile float prevServo3Error;

float counts_per_mm;

volatile float kpX;
volatile float kdX;

volatile _Bool useSpeedControl = 0;
volatile _Bool useSensorFeedback = 0;

volatile float prevEncCount = 0;
volatile float curEncCount = 0;
volatile float curSpeed = 0;	// actual, counts/ms
volatile float curSpeedX = 0;	// ideal

volatile float targetAccX;
volatile float targetDecX;
volatile float targetSpeedX = 0;

volatile float encFeedbackX = 0;
volatile float prevErrorX = 0;
volatile float errorX = 0;
volatile float posPwmX = 0;
volatile float motorPwm = 0;

void updateSpeed(void) 
{
	curEncCount = getEncCount();
	curSpeed = curEncCount - prevEncCount;
	prevEncCount = curEncCount;
}

void speedControl(void)
{
	if (curSpeedX < targetSpeedX) {
		curSpeedX += mm_to_counts(targetAccX)/1000;
		if (curSpeedX > targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	else if (curSpeedX > targetSpeedX) {
		curSpeedX -= mm_to_counts(targetDecX)/1000;
		if (curSpeedX < targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	
	// calculate translational error
	encFeedbackX = curSpeed;
	errorX += curSpeedX - encFeedbackX;	// ideal - actual
	posPwmX = kpX*errorX + kdX*(errorX-prevErrorX);

	// update derivative input
	prevErrorX = errorX;
	
	motorPwm = posPwmX;
	
	setPwm(motorPwm);
}

void updateServo2(void)
{
	static int32_t pwmIndex = 0;
	static int32_t numOccupied = 1;
	static float servo2PwmArray[SERVO2_PWM_ARRAY_SIZE] = {0};
	
	servo2Error = 64 - lineIndex;
	servo2PwmArray[pwmIndex] = servo2CenterPwm + pan_Kp*servo2Error + pan_Kd*(servo2Error - prevServo2Error);
	prevServo2Error = servo2Error;
	
	// Average pwm array
	servo2Pwm = 0;
	for (int32_t i = 0; i < numOccupied; i++) {
		servo2Pwm += servo2PwmArray[i];
	}
	servo2Pwm /= numOccupied;
	
	setServo2Pwm(servo2Pwm);
	
	if (numOccupied < SERVO2_PWM_ARRAY_SIZE)
		numOccupied++;
	
	pwmIndex++;
	if (pwmIndex == SERVO2_PWM_ARRAY_SIZE)
		pwmIndex = 0;
}

void updateServo3(void)
{
	static int32_t pwmIndex = 0;
	static int32_t numOccupied = 1;
	static float servo3PwmArray[SERVO3_PWM_ARRAY_SIZE] = {0};
	float prevServo3Pwm = servo3Pwm;
	
	servo3Error = (servo2CenterPwm - servo2Pwm) + (lineIndex - 64);
	servo3PwmArray[pwmIndex] = servo3CenterPwm + steering_Kp*servo3Error - steering_Kd*(servo3Error - prevServo3Error);
	prevServo3Error = servo3Error;
	
	// Average pwm array
	servo3Pwm = 0;
	for (int32_t i = 0; i < numOccupied; i++) {
		servo3Pwm += servo3PwmArray[i];
	}
	servo3Pwm /= numOccupied;
	
	setServo3Pwm(servo3Pwm);
	
	if (numOccupied < SERVO3_PWM_ARRAY_SIZE)
		numOccupied++;
	
	pwmIndex++;
	if (pwmIndex == SERVO3_PWM_ARRAY_SIZE)
		pwmIndex = 0;
}

void enableSpeedControl(void) {
	resetMotorParameters();
	useSpeedControl = 1;
}

void disableSpeedControl(void) {
	useSpeedControl = 0;
	resetMotorParameters();
}

void resetMotorParameters(void) {
	encFeedbackX = 0;
	prevErrorX = 0;
	errorX = 0;
	posPwmX = 0;
	
	prevEncCount = 0;
	curEncCount = 0;
	curSpeed = 0;
	curSpeedX = 0;
	
	targetSpeedX = 0;
	
	setPwm(0);
	resetEncCount();
}

// Set motor pwm
void setPwm(int32_t pwm)
{
	if (pwm > 999)
		pwm = 999;
	else if (pwm < 0)
		pwm = 0;

	MTR_PWM = pwm;
}

// Set servo1 on-time in microseconds
void setServo1Pwm(int32_t pwm)
{
	if (pwm > servo1MaxPwm)
		pwm = servo1MaxPwm;
	else if (pwm < servo1MinPwm)
		pwm = servo1MinPwm;
	
	SERVO1_PWM = pwm;
}

// Set servo2 on-time in microseconds 
void setServo2Pwm(int32_t pwm)
{
	if (pwm > servo2MaxPwm)
		pwm = servo2MaxPwm;
	else if (pwm < servo2MinPwm)
		pwm = servo2MinPwm;
	
	SERVO2_PWM = pwm;
}

// Set servo3 on-time in microseconds
void setServo3Pwm(int32_t pwm)
{
	if (pwm > servo3MaxPwm)
		pwm = servo3MaxPwm;
	else if (pwm < servo3MinPwm)
		pwm = servo3MinPwm;
	
	SERVO3_PWM = pwm;
}

// Set translational speed
void setSpeedX(float m_per_s)
{
	targetSpeedX = mm_to_counts(m_per_s);
}

// convert mm to counts
inline float mm_to_counts(float mm)
{
	return mm*counts_per_mm;
}

// convert counts to mm
float counts_to_mm(float counts)
{
	return counts/counts_per_mm;
}
