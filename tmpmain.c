/*====================================================
 Hunter Walden and Rich Surgenor
 ELEC 3040/3050 - Lab 4, Program 1 
 Description: counts up or down depending on PA1-2 and outputs counter[0] on PC3-0
====================================================*/

#include "STM32L1xx.h" /* Microcontroller information */
#include "interrupt_handler.h"
#include "common.h"

#define COUNTERS_SIZE (uint16_t) 2

typedef enum {
	COUNTING_UP = 1,
	COUNTING_DOWN = 0
}COUNTING_DIRECTION;

typedef struct {
		unsigned int count : 4;
		unsigned int direction : 1;
		unsigned int speed : 16;
} counter;

/* Define global variables */
counter counters[] = {
	{ .count = 0, .direction = COUNTING_UP, .speed = 1000 },
	{ .count = 0, .direction = COUNTING_UP, .speed = 500 }
};

volatile unsigned char sw1 = 0; //state of sw1 (PA0)
volatile unsigned char sw2 = 0; //state of sw2 (PA1)

void update_counters(int ctr) {
	char output = counters[ctr].count << ctr*4 ;
	GPIOC->ODR = output;                        //Update PC3-PC0 output to match counter[0]
}

/*---------------------------------------------------*/
/* counter function - decade up/down counter between 0 & 9 */
/* Green LED = counting up, Blue LED = counting down */
/*---------------------------------------------------*/
void counting () {
	// This needs to be reconfigured:
	
	// 1. first counter should always be counting 0-9
	// 2. second counter should change increasing to decreasing when PA0 interrupt occurs
	// 3. second counter should change from decreasing to increasing when PA1 interrupt occurs
	
	for (int i = 0; i < COUNTERS_SIZE; i++) {
		if (counters[i].direction) { 						 
			counters[i].count++;
		} else {                                  
			counters[i].count--;                         
		}

		//temporary delay solution:
		// delay 500ms for the first counter, and then another 500ms the next time for the next counter 
		delay(500);
		
		update_counters(i);
		//TEMPORARY
		
		if ( counters[i].count > 9 ) {       				//Cycle back to 0 if incrementing
					counters[i].count = 0;
		}
		
		if ( counters[i].count < 0 ) {
			counters[i].count = 9;
		}
	}
}

/*---------------------------------------------------*/
/* Initialize GPIO pins used in the program */
/* PC3-0 = virtual LEDs */
/* PC8 = blue LED, PC9 = green LED */
/*---------------------------------------------------*/
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
	 
		//sw1 = GPIOA->IDR & 0x2; //Read GPIOA inputs and mask all but bit 1	 
		//sw2 = GPIOA->IDR & 0x4; //Read GPIOA inputs and mask all but bit 2
		
		counting();
	} /* repeat forever */
} 

int main(void) {
	// EXTICR[0] will select EXTI3-0
	enable_GPIO();
	init_interrupts(&SYSCFG->EXTICR[0]); 
	
	event_loop();
}

