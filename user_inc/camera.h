#if !defined(CAMERA_H)
#define CAMERA_H

void readCameraStart(void);
void readCameraStop(void);
void readCamera(void);
void updateExposureTime(void);
void detectLinePos(void);
void movingAvgFilter(volatile int32_t *arr, int32_t size);

extern volatile int32_t scanBuf[128];
extern volatile float linePos;
extern volatile int32_t exposureTime_us;

extern int32_t nearCamPwm;
extern int32_t farCamPwm;
extern int32_t nearCamMinPwm;
extern int32_t nearCamMaxPwm;
extern int32_t farCamMinPwm;
extern int32_t farCamMaxPwm;

#endif
