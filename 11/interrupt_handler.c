#include "interrupt_handler.h"
#include "keypad.h"
#include "common.h"
#include "timers.h"
#include "controller.h"

#include "stdint.h"
#include "string.h"


uint8_t display_keypad_count = 0;
double frequency;
double period;


uint16_t low_frequency = 50;
uint16_t high_frequency = 500;

#define LOW_FREQUENCY (uint32_t) 50
#define HIGH_FREQUENCY (uint32_t) 500
#define SPACING (uint32_t) HIGH_FREQUENCY / LOW_FREQUENCY

//TIM9 interrupts every 10ms->store period data in array->need 2s worth of data->  2s/0.01s=200

#define size (uint32_t) 200 //2/( ((TIM9_ARR + 1) * (TIM9_PSC + 1)) / (TIMX_CLOCK_SPEED) );

uint16_t data[size];

uint16_t old_CCR1;
int datacol = 0;
//int domain[20000];
//double t = ((TIM9_ARR + 1) * (TIM9_PSC + 1)) / (TIMX_CLOCK_SPEED);

/* Process of interrupt signal on our board:
1. Interrupt signal comes in on GPIO pin
2. Edge detection
3. Software interrupt event register => IMR
4. Interrupt mask register => PR

5. IRQ TO NVIC

All paths must be cleared for signal to be processed correctly.
*/

/*For lab 4 specifically: Two GPIO pins, PA0 and PA1, are to be configured as external
interrupt signals EXTI0 and EXTI1*/

/* Unblocks CPU signals from PA0 and PA1 specifically, and then
   does this in the NVIC as well.
 */
// TODO: make more generic
void init_interrupts(volatile uint32_t *SYSCFG_EXTICR)
{
	//SystemInit();
	//RCC_APB2ENR_SYSCFGEN
	__enable_irq();

	/*
	0000: PA[x] pinX
	0001: PB[x] pin
	0010: PC[x] pin
	0011: PD[x] pin
	0100: PE[x] pin
	*/
	*SYSCFG_EXTICR &= ~(SYSCFG_EXTICR1_EXTI1); // 0x000000F0

	//EXTI0 and EXTI1 are both rising edge triggered
	EXTI->FTSR |= EXTI_FTSR_TR1;

	//EXTI0 and EXT1 lines are both unmasked
	EXTI->IMR |= EXTI_IMR_MR1;

	//Enable NVIC IRQ lines for
	NVIC_EnableIRQ(EXTI1_IRQn);

	NVIC_SetPriority(EXTI1_IRQn, 1);
}

/* EXTI1 Interrupt Handler
 * Switches direction of counter[1] to count up, toggles green LED
 */
void EXTI1_IRQHandler(void)
{
	uint16_t loops = 50;
	for (int k = 0; k < loops; k++);

	bool failure = false;
	location loc = detect(&failure);

	uint16_t key = keys[loc.row][loc.col];
	if (!failure) {

		if(key == 0xf || key == 0xd) {
			toggle_timers(6);
		} else {
			//TIM10->CCR1 =  ( (TIM10->ARR+1) * (key * 10) ) / 100;
			set_SP( (TIMX_CLOCK_SPEED/( (SPACING*key) ) ) / (1 + TIM11->PSC) );
			
			GPIOC->ODR = key | ( GPIOC->ODR & GREEN_LED );
		}
		//display_keypad_count = SECONDS_TO_DISPLAY_KEYPAD_PRESS;
	} else {
		GPIOC->ODR |= BLUE_LED;
	}

	memset(data, 0, sizeof data); // clear data array
	datacol = 0; // reset current index
  toggle_timers(9); //turn on sampler
	//NVIC_EnableIRQ(TIM9_IRQn);
	//counters[1].direction = COUNTING_UP;

	GPIOB->ODR &= ~(0xF0); // Clear PB7-4
	EXTI->PR |= EXTI_PR_PR1;
}

void TIM6_IRQHandler(void)
{
	for (int i = 0; i < COUNTERS_SIZE; i++) {

		counters[i].current++;
		if (counters[i].current >= counters[i].period) {
			count(i);
			counters[i].current = 0;
		}
	}
	TIM6->SR ^= 0x1;
	NVIC_ClearPendingIRQ(TIM6_IRQn);
}

void TIM9_IRQHandler(void)
{
//	if (datacol >= 200) {
//		return;
//	}
	
	//frequency = get_tachometer_frequency();
	//period = 1/frequency;
	//t = t * datacol;

	
	//if (TIM11->CCR1 < CERTAIN_AMT) { this may need to be used
	//	set_PV(x);
	//}

/*	
	int difference;
	difference = TIM11->CCR1 - old_CCR1;
	if (difference < 0) {
		difference = difference * -1;
	}
	
	if (difference < x) { // to avoid changing PV when CCR1 jumps wildly */

	set_PV(TIM11->CCR1); // is the CCR1 valid?
	
	data[datacol] = TIM11->CCR1; //period;
	
	old_CCR1 = TIM11->CCR1;
	
	//domain[datacol] = t;
	datacol++;
	if(datacol == size)
	{
		//TIM9->SR ^= 0x1;
		//NVIC_ClearPendingIRQ(TIM9_IRQn);
		toggle_timers(9); //turn off sampler
		//NVIC_DisableIRQ(TIM9_IRQn);
		//TIM9->DIER &= ~0x01;
	} else {
		TIM9->SR ^= 0x1;
		NVIC_ClearPendingIRQ(TIM9_IRQn);
	}
}

void TIM11_IRQHandler(void)
{
	TIM11->CNT = 0;
	TIM11->SR ^= 0x3;
	NVIC_ClearPendingIRQ(TIM11_IRQn);
}

/*void TIM10_IRQHandler(void)
{
	for (int i = 0; i < COUNTERS_SIZE; i++) {

		counters[i].current++;
		if (counters[i].current >= counters[i].period) {
			count(i);
			counters[i].current = 0;
		}
	}
	TIM10->SR ^= 0x1;
	NVIC_ClearPendingIRQ(TIM10_IRQn);
}*/
