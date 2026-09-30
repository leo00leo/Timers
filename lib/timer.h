#ifndef TIMER_H
#define TIMER_H
#include "stm32f103xb.h"

void delay_init();
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void timer_init();
uint32_t timer_millis();
#endif