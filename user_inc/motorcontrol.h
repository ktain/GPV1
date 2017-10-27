#if !defined(MOTORCONTROL_H)
#define MOTORCONTROL_H

extern volatile float steeringOnTime;
extern volatile int32_t steeringCenterOnTime;
extern volatile int32_t steeringMinOnTime;
extern volatile int32_t steeringMaxOnTime;

extern float steering_Kp;
extern float steering_Kd;

extern volatile float steeringPwm;
extern volatile float steeringError;
extern volatile float prevSteeringError;

extern float counts_per_mm;

extern volatile _Bool useSpeedControl;

extern volatile float prevEncCount;
extern volatile float curEncCount;
extern volatile float prevSpeed;
extern volatile float curSpeed;
extern volatile float prevSpeedX;
extern volatile float curSpeedX;
extern volatile float curAccX;

extern volatile float targetAccX;
extern volatile float targetDecX;
extern volatile float targetSpeedX;

extern volatile float encFeedbackX;
extern volatile float prevErrorX;
extern volatile float errorX;
extern volatile float posPwmX;
extern volatile float motorPwm;
extern volatile float motorMaxPwm;

extern volatile float kpX;
extern volatile float kdX;

void updateSteeringAngle(void);
void updateSpeed(void);
void speedControl(void);
void setPwm(int32_t pwm);
void setSpeedX(float m_per_s);
void resetMotorParameters(void);
void enableSpeedControl(void);
void disableSpeedControl(void);

void setSteeringOnTime(int32_t onTime_us);
float mm_to_counts(float mm);
float counts_to_mm(float counts);

#endif
