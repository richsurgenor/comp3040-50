/*
common.c
Common variables/functions throughout the program
*/

#include "common.h"
#include "STM32L1xx.h"

volatile counter counters[] = { // TODO: Move this to main()
	{ .count = 0, .direction = COUNTING_UP, .current = 0, .period = 10 },
	{ .count = 0, .direction = COUNTING_UP, .current = 0, .period = 1 }
};

/* delay in ms */
void delay (int ms)
{
	int j,n;
	for (j=0; j<266*ms; j++) {
		n = j; //dummy operation for single-step test
	} //do nothing
}

void update_counters(void)
{
	uint16_t output = 0;
	output |= (GPIOC->ODR & 0x0300); // keep status LEDs' states

	for (int i = 0; i < COUNTERS_SIZE; i++) {
		output |= counters[i].count << i*4;
	}

	GPIOC->ODR = output;
}

/* Initialize GPIO pins used in the program
 * PC3-0 = virtual LEDs
 * PC8 = blue LED, PC9 = green LED */
void enable_counter_GPIO(void)
{
	/* Configure PC3-PC0 as output pins to drive virtual LEDs */
	RCC->AHBENR |= 0x04;           // Enable GPIOC clock (bit 2)
	GPIOC->MODER &= ~(0x0000FFFF); // Clear PC7-PC0 mode bits
	GPIOC->MODER |= (0x00005555);  // General purpose output mode
	GPIOC->BSRR = 0x00FF << 16;    // Reset PC7-PC0 output bits to 0

	/* Configure PC8,PC9 as output pins to drive LEDs */
	GPIOC->MODER &= ~(0x000F0000); // Clear PC9-PC8 mode bits
	GPIOC->MODER |= (0x00050000);  // General purpose output mode
	GPIOC->BSRR = 0x0300 << 16;    // Reset PC9-PC8 output bits to 0
}

/* counter function - decade up counter between 0 & 9
 * Green LED = counting up, Blue LED = counting down
 */

void count(uint8_t num)
{
	counters[num].count++;
	// Do not allow counter to leave decided range
	if ( counters[num].count > COUNTER_HIGH ) {
		counters[num].count = 0;
	}
}

void clear_counters(void)
{
	for (int i = 0; i < COUNTERS_SIZE; i++) {
		counters[i].count = 0;
		counters[i].current=0;
	}
}
