#if !defined(__STM32F4xx_IT_H)
#define __STM32F4xx_IT_H

#if defined(__cplusplus)
 extern "C" {
#endif 

#include "stm32f4xx.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI9_5_IRQHandler(void);
void TIM3_IRQHandler(void);

extern volatile uint32_t g_millis;

uint32_t millis(void);
uint32_t micros(void);

#if defined(__cplusplus)
}
#endif

#endif
