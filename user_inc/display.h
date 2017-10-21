#ifndef DISPLAY_H
#define DISPLAY_H

static const char fontTable[127*5];

extern int32_t displayDelay_ms;
extern int32_t displayDelayCounter;
extern int32_t displayBrightness;
extern char displayStr[36];

void writeControlRegister(char c);
void display(char* str);
void clearDisplay(void);
void displayFloat(float f);
void displayInt(int32_t i);
void setDisplayBrightness(int8_t brightness);
void displayVoltage(void);
void displayError(void);
void updateDisplay(void);

#endif
