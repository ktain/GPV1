#if !defined(CAMERA_H)
#define CAMERA_H

void readCameraStart(void);
void readCameraStop(void);
void readCamera(void);

void scanLine(void);

extern volatile int32_t scanBuf[128];
extern volatile float lineIndex;
extern volatile int32_t integrationTime_us;
extern volatile int32_t integrationInterval_us;

#endif
