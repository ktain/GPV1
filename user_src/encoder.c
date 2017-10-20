#include "stm32f4xx.h"

/*
 * getEncCount() - Returns encoder count
 */
int32_t getEncCount(void)
{
	return TIM5->CNT;
}

/*
 * setEncCount() - Sets encoder count
 */
void setEncCount(int32_t count)
{
	TIM5->CNT = count;
}

/*
 * resetEncCount() - Resets encoder count
 */
void resetEncCount(void)
{
	TIM5->CNT = 0;
}
