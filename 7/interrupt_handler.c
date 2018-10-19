
#include "interrupt_handler.h"
#include "keypad.h"
#include "common.h"
#include "timers.h"

uint8_t display_keypad_count = 0;

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

		TIM10->CCR1 =  ( (TIM10->ARR+1) * (key * 10) ) / 100;
		GPIOC->ODR = key | ( GPIOC->ODR & GREEN_LED );

		//display_keypad_count = SECONDS_TO_DISPLAY_KEYPAD_PRESS;
	} else {
		GPIOC->ODR |= BLUE_LED;
	}

	//counters[1].direction = COUNTING_UP;

	GPIOB->ODR &= ~(0xF0); // Clear PB7-4
	EXTI->PR |= EXTI_PR_PR1;
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
