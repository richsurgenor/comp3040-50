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

  GPIOA->MODER &= ~0xC000; // clear mode of PA7
  GPIOA->MODER |= 0x8000; // alternate function mode on PA7

  GPIOA->AFR[0] &= ~0xFF000000; //clear AFRL6 and AFRL7
  GPIOA->AFR[0] |= 0x33000000; //PA6 = AF3, PA7 = AF3

  /*lab9*/
  GPIOA->PUPDR &= ~(0x0000C000); // Clear PA7 mode bits
	GPIOA->PUPDR |= (0x00004000);  // Set PA7 to pull-up resistor
}
//cr, ccmr, capture compare register number

void init_timers(void)
{
  RCC->CR |= RCC_CR_HSION; // Turn on 16MHz HSI oscillator
  while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI ready
  RCC->CFGR |= RCC_CFGR_SW_HSI; // Select HSI as system clock

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // enable clock in the RCC
	
  RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; // enable clock in the RCC
  RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; // enable clock in the RCC
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN; // enable clock in the RCC
	
  //TIM10->DIER |= TIM_DIER_UIE; // enable trigger controller
	NVIC_EnableIRQ(TIM9_IRQn); // enable TIM9 interrupts
  NVIC_SetPriority(TIM9_IRQn, 0);
	
  NVIC_EnableIRQ(TIM10_IRQn); // enable TIM10 interrupts
  NVIC_SetPriority(TIM10_IRQn, 2);
	
	NVIC_EnableIRQ(TIM11_IRQn); // enable TIM11 interrupts
  NVIC_SetPriority(TIM11_IRQn, 1);
	
	NVIC_EnableIRQ(TIM6_IRQn);
	NVIC_SetPriority(TIM6_IRQn, 3);

  TIM10->ARR = TIM10_ARR;
  TIM10->PSC = TIM10_PSC;

  TIM10->CCMR1 &= ~0x70; //clear Output Compater 1 Mode bits
  TIM10->CCMR1 |= 0x60; // set to PWM mode 1 (active to "inactive")

  TIM10->CCER &= ~0x03; //clear CC1 Polarity (b1) and CC1 Enable (b0) bits
  TIM10->CCER |= 0x01; //set CC1 Enable bit

  //TIM11 initialization
  TIM11->ARR = TIM11_ARR;
  TIM11->PSC = TIM11_PSC;

  TIM11->DIER |= TIM_DIER_CC1IE; // enable trigger controller

  TIM11->CCMR1 &= ~0x3;
  TIM11->CCMR1 |= 0x1;

  TIM11->CCER &= ~0x03; //clear CC1 Polarity (b1) and CC1 Enable (b0) bits
  TIM11->CCER |= 0x01; //set CC1 Enable bit
	
	//stopwatch
	TIM6->ARR = TIM6_ARR;
	TIM6->PSC = TIM6_ARR;
	TIM6->DIER |= TIM_DIER_UIE;
	
	//TIM11->CCMR1 |= 0xD0;
	
	//TIM9 initialization
  TIM9->ARR = TIM9_ARR;
  TIM9->PSC = TIM9_PSC;

  TIM9->DIER |= TIM_DIER_UIE; // enable trigger controller

}

void toggle_timers(int timer)
{
  switch(timer) {
		case 6:
			TIM6->CR1 ^= 0x01;
			break;
		case 9:
      TIM9->CR1 ^= 0x01; // bit0 = enable/disable yeet
      break;
    case 10:
      TIM10->CR1 ^= 0x01; // bit0 = enable/disable yeet
      break;
    case 11:
      TIM11->CR1 ^= 0x01; // bit0 = enable/disable yeet
      break;
  }
}

void clear_timers(int timer)
{
  switch(timer) {
		case 9:
      TIM9->CNT = 0;
      break;
    case 10:
      TIM10->CNT = 0;
      break;
    case 11:
      TIM11->CNT = 0;
      break;
  }

  // clear timers or counters?
}

double get_tachometer_frequency()
{
	if (TIM11->CCR1) {
		return ((double) TIMX_CLOCK_SPEED / TIM11->CCR1) / (1+TIM11->PSC);
	}
	return 0;
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
