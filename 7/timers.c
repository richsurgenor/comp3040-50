/*
timers.c
Manages all program timers
*/

#include "stdint.h"
#include "STM32L1xx.h"
#include "timers.h"

/*
Generate a PWM waveform with timer TIM10
 Period should be 1 ms (frequency 1 KHz)
 First, generate a waveform with one duty cycle value
 Then, verify that you can generate waveforms with each
of the 11 specified duty cycles, from 0% to 100%, as
selected by keypad keys 0 – A.
 Measure and record the 11 duty cycle values
 Plot measured duty cycle vs. selection key #
*/

// Change output pin when CNT = CCRy

//Set frequency in Hz for timer
void set_frequency(uint32_t frequency)
{
    TIM10->ARR = ( (TIMX_CLOCK_SPEED / frequency ) / TIM10->PSC) - 1;
}

void enable_timer_GPIO()
{
  GPIOA->MODER &= ~0x3000; // clear mode of PA6
  GPIOA->MODER |= 0x2000; // alternate function mode on PA6

  GPIOA->AFR[0] &= ~0x0F000000; //clear AFRL6
  GPIOA->AFR[0] |= 0x03000000; //PA6 = AF2
}
//cr, ccmr, capture compare register number

void init_timers(void)
{
  RCC->CR |= RCC_CR_HSION; // Turn on 16MHz HSI oscillator
  while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI ready
  RCC->CFGR |= RCC_CFGR_SW_HSI; // Select HSI as system clock

  RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; // enable clock in the RCC
  //TIM10->DIER |= TIM_DIER_UIE; // enable trigger controller
  NVIC_EnableIRQ(TIM10_IRQn); // enable TIM10 interrupts
  NVIC_SetPriority(TIM10_IRQn, 0);

  TIM10->ARR = TIM10_ARR;
  TIM10->PSC = TIM10_PSC;

  TIM10->CCMR1 &= ~0x70; //clear Output Compater 1 Mode bits
  TIM10->CCMR1 |= 0x60; // set to PWM mode 1 (active to "inactive")
  
  TIM10->CCER &= ~0x03; //clear CC1 Polarity (b1) and CC1 Enable (b0) bits
  TIM10->CCER |= 0x01; //set CC1 Enable bit
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
//ARR = (16000000 / f / 32) - 1
//ARR = ((1/f) / (32) * (16000000)) - 1
//ARR = 49999
//ARR is 16 bit register...
//(2^16) - 1 = 65535

//Button 0 = start/stop toggle for the stopwatch
//Button 1 = clear time value of stopwatch (can only activate when timer is stopped)
