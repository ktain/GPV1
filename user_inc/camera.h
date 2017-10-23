#if !defined(CAMERA_H)
#define CAMERA_H

void readCameraStart(void);
void readCameraStop(void);
void readCamera(void);

void scanLine(void);

extern volatile int32_t scanBuf[128];
extern volatile float lineIndex;
extern volatile int32_t integrationTime_us;

extern int32_t nearCamPwm;
extern int32_t farCamPwm;
extern int32_t nearCamMinPwm;
extern int32_t nearCamMaxPwm;
extern int32_t farCamMinPwm;
extern int32_t farCamMaxPwm;

#endif
