#if !defined(DELAY_H)
#define DELAY_H

void delay_ms(uint32_t duration_ms);
void delay_us(uint32_t duration_us);
void elapse_ms(uint32_t duration_ms, uint32_t start_time_ms);
void elapse_us(uint32_t duration_us, uint32_t start_time_us);

#endif
