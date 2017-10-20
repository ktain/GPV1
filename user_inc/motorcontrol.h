#if !defined(MOTORCONTROL_H)
#define MOTORCONTROL_H

extern int32_t servo1CenterPwm;
extern int32_t servo2CenterPwm;
extern int32_t servo3CenterPwm;
extern int32_t servo1MinPwm;
extern int32_t servo1MaxPwm;
extern int32_t servo2MinPwm;
extern int32_t servo2MaxPwm;
extern int32_t servo3MinPwm;
extern int32_t servo3MaxPwm;

extern float pan_Kp;
extern float pan_Kd;
extern float steering_Kp;
extern float steering_Kd;

extern volatile float servo2Pwm;
extern volatile float servo2Error;
extern volatile float prevServo2Error;
extern volatile float servo3Pwm;
extern volatile float servo3Error;
extern volatile float prevServo3Error;

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

extern volatile float kpX;
extern volatile float kdX;

void updateServo2(void);
void updateServo3(void);
void updateSpeed(void);
void speedControl(void);
void setPwm(int32_t pwm);
void setSpeedX(float m_per_s);
void resetMotorParameters(void);
void enableSpeedControl(void);
void disableSpeedControl(void);

void setServo1Pwm(int32_t pwm);
void setServo2Pwm(int32_t pwm);
void setServo3Pwm(int32_t pwm);

float mm_to_counts(float mm);
float counts_to_mm(float counts);

float getSensorError(void);

#endif
