#if !defined(CAMERA_H)
#define CAMERA_H

void readCameraStart(void);
void readCameraStop(void);
void readCamera(void);
void updateExposureTime(void);
void detectLinePos(volatile int32_t *arr, int32_t size, int32_t min_width, int32_t max_width);
void movingAvgFilter(volatile int32_t *arr, int32_t size);
int32_t getNearestPeak(volatile int32_t *arr, int32_t size, int32_t threshold, float old_pos, int32_t min_width, int32_t max_width);
void setNearCamOnTime(int32_t onTime_us);
void setFarCamOnTime(int32_t onTime_us);


extern volatile int32_t scanBuf[128];
extern volatile float linePos;
extern volatile int32_t exposureTime_us;

extern int32_t minExposureTime_us;
extern int32_t maxExposureTime_us;
extern int32_t min_line_width;
extern int32_t max_line_width;

extern int32_t nearCamOnTime;
extern int32_t farCamOnTime;
extern int32_t nearCamMinOnTime;
extern int32_t nearCamMaxOnTime;
extern int32_t farCamMinOnTime;
extern int32_t farCamMaxOnTime;

#endif
