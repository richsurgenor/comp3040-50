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

#define TIM10_PSC (uint32_t) 31
#define TIM10_ARR (uint32_t) 49999 // 50000 - 1

void init_timers(void);
void toggle_timers(void);
void clear_timers(void);

#endif
