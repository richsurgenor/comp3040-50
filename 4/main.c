/*====================================================
 Hunter Walden and Rich Surgenor
 ELEC 3040/3050 - Lab 4, Program 1 
 Description: Enables interrupt-driven input on PA0-1 for changing direction of counter[1]
====================================================*/

#include "STM32L1xx.h" /* Microcontroller information */
#include "interrupt_handler.h"
#include "common.h"

/* Define global variables */
volatile counter counters[] = { // TODO: Move this to main()
	{ .count = 0, .direction = COUNTING_UP, .speed = 1000 },
	{ .count = 0, .direction = COUNTING_UP, .speed = 500}
};

#define COUNTERS_SIZE (uint16_t) 2
#define COUNTER_HIGH (uint8_t) 9
#define COUNTER_LOW (uint8_t) 0

//TODO: Move constants out of functions

/* Updates the ODR with latest counter/LED information available */
void update_counters() 
{
	uint16_t output = 0;
	output |= (GPIOC->ODR & 0x0300); // keep status LEDs' states
	
	for (int i = 0; i < COUNTERS_SIZE; i++) {
		output |= counters[i].count << i*4;                    
	}
	
	GPIOC->ODR = output;  
}

/* counter function - decade up counter between 0 & 9
 * Green LED = counting up, Blue LED = counting down */
void counting0() 
{
	counters[0].count++;
	// Do not allow countere to leave decided range
	if ( counters[0].count > COUNTER_HIGH ) {
					counters[0].count = 0;
		}
	update_counters();
}

/* counter function - decade up/down counter between 0 & 9
 * Green LED = counting up, Blue LED = counting down */
void counting1 () {
	// Increment or decrement based on direction of counter
	if (counters[1].direction) { 						 
			counters[1].count++;     
		} else {
			counters[1].count--;                       
		}
		
		// Do not allow counter to leave decided range
		if ( counters[1].count > COUNTER_HIGH ) {
					counters[1].count = COUNTER_LOW;
		}
		if ( counters[1].count < COUNTER_LOW ) {
			counters[1].count = COUNTER_HIGH;
		}
		
		update_counters();
}

/* Initialize GPIO pins used in the program
 * PC3-0 = virtual LEDs 
 * PC8 = blue LED, PC9 = green LED */
void enable_GPIO(void) {
	/* Configure PA0 as input pin to read push button */
	RCC->AHBENR |= 0x01; /* Enable GPIOA clock (bit 0) */
	GPIOA->MODER &= ~(0x00000003C); /* General purpose input mode */
	   
	/* Configure PC3-PC0 as output pins to drive virtual LEDs */
	RCC->AHBENR |= 0x04;           // Enable GPIOC clock (bit 2) 
	GPIOC->MODER &= ~(0x000000FF); // Clear PC3-PC0 mode bits 
	GPIOC->MODER |= (0x00005555);  // General purpose output mode
	GPIOC->BSRR = 0x00FF << 16;    // Reset PC3-PC0 output bits to 0
	   
	/* Configure PC8,PC9 as output pins to drive LEDs */
	GPIOC->MODER &= ~(0x000F0000); // Clear PC9-PC8 mode bits
	GPIOC->MODER |= (0x00050000);  // General purpose output mode
	GPIOC->BSRR = 0x0300 << 16;    // Reset PC9-PC8 output bits to 0
}

int event_loop(void) {
	/* Endless loop */
	while (1) {  
			delay(500);
			counting0();
			delay(500);
			counting1();
		  counting0();
		
	} /* repeat forever */
} 

int main(void) {
	
	enable_GPIO();
	init_interrupts(&SYSCFG->EXTICR[0]); // // EXTICR[0] will select EXTI3-0
	
	event_loop();
}
