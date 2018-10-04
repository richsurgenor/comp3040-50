/*
timers.c
Manages all program timers
*/

#include "stdint.h"
#include "STM32L1xx.h"
#include "timers.h"

void init_timers(void)
{
  RCC->CR |= RCC_CR_HSION; // Turn on 16MHz HSI oscillator
  while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI ready
  RCC->CFGR |= RCC_CFGR_SW_HSI; // Select HSI as system clock

  RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; // enable clock in the RCC
  TIM10->DIER |= TIM_DIER_UIE; // enable trigger controller
  NVIC_EnableIRQ(TIM10_IRQn); // enable TIM10 interrupts
  NVIC_SetPriority(TIM10_IRQn, 0);

  TIM10->ARR = TIM10_ARR;
  TIM10->PSC = TIM10_PSC;
}

void toggle_timers(void)
{
  TIM10->CR1 ^= 0x01; // bit0 = enable/disable yeet
}

void clear_timers(void)
{
  TIM10->CNT = 0;
  // clear timers or counters?
}

//Tperiod = (ARR + 1) x (PSC + 1) / (Fclock)
//Tperiod = (ARR + 1) x (PSC + 1) / (16MHz)

//IF PSC = 31
//ARR = ((0.1 second) / (32) * (16MHz)) - 1
//ARR = 49999
//ARR is 16 bit register...
//(2^16) - 1 = 65535

//Button 0 = start/stop toggle for the stopwatch
//Button 1 = clear time value of stopwatch (can only activate when timer is stopped)
