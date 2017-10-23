#include "stm32f4xx.h"

#include "setup.h"
#include "userconfig.h"
#include "motorcontrol.h"
#include "encoder.h"
#include "camera.h"

float steering_Kp;
float steering_Kd;

volatile float steeringPwm;
volatile int32_t steeringCenterPwm;
volatile int32_t steeringMinPwm;
volatile int32_t steeringMaxPwm;
volatile float steeringError;
volatile float prevSteeringError;

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
volatile float motorMaxPwm = 0;

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


void updateSteeringAngle(void)
{
	// steeringError from -1.0 to 1.0
	steeringError = (linePos - 63.5)/63.5;
	
	// steering error converted to pwm
	if (steeringError < 0)
		steeringError = steeringError * (steeringCenterPwm - steeringMinPwm);
	else 
		steeringError = steeringError * (steeringMaxPwm - steeringCenterPwm);
	
	steeringPwm = steeringCenterPwm + steeringError;
	setSteeringPwm(steeringPwm);
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
	if (pwm > motorMaxPwm)
		pwm = motorMaxPwm;
	else if (pwm < 0)
		pwm = 0;

	MTR_PWM = pwm;
}

// Set steering servo on-time in microseconds
void setSteeringPwm(int32_t pwm)
{
	if (pwm > steeringMaxPwm)
		pwm = steeringMaxPwm;
	else if (pwm < steeringMinPwm)
		pwm = steeringMinPwm;
	
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
