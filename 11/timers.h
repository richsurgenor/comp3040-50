/*
timers.h
Manages all timers throughout the program
*/

#include "stdint.h"
#include "STM32L1xx.h"

#ifndef TIMERS_H
#define TIMERS_H

//timx clock from rcc -> trigger controller -> psc prescaler -> counter -> autoreload register -> update event

//Tout = (0x2000 × 0x100) ÷ 0x200000 = 1 second
#define TIMX_CLOCK_SPEED (uint32_t) 16000000  //16MHz

#define TIM6_PSC (uint32_t) 31
#define TIM6_ARR (uint32_t) 49999 // 50000 - 1

// Tperiod = (ARR + 1) x (PSC + 1) / (16MHz)
// Tperiod = (4999 + 1) x (31 + 1) / (16MHz) = 10e-3
#define TIM9_PSC (uint32_t) 31//31
#define TIM9_ARR (uint32_t) 4999//4999 // for period of 10ms

#define TIM10_PSC (uint32_t) 31
#define TIM10_ARR (uint32_t) 49999 // 50000 - 1

#define TIM11_PSC (uint32_t) 5
#define TIM11_ARR (uint32_t) 0xFFFF // 50000 - 1

void set_frequency(uint32_t frequency);
void enable_timer_GPIO(void);
void init_timers(void);
void toggle_timers(int timer);
void clear_timers(int timer);

/*LAB9*/
double get_tachometer_frequency(void);

#endif
