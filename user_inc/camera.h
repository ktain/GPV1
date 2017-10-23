#if !defined(CAMERA_H)
#define CAMERA_H

void readCameraStart(void);
void readCameraStop(void);
void readCamera(void);

void scanLine(void);

extern volatile int32_t scanBuf[128];
extern volatile float lineIndex;
extern volatile int32_t integrationTime_us;

extern int32_t camera1Pwm;
extern int32_t camera2Pwm;
extern int32_t camera1MinPwm;
extern int32_t camera1MaxPwm;
extern int32_t camera2MinPwm;
extern int32_t camera2MaxPwm;

#endif
