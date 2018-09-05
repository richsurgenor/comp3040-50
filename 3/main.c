/*====================================================*/
/* Hunter Walden and Rich Surgenor
/* ELEC 3040/3050 - Lab 2, Program 1 */
/* Description: Counts up or down depending on PA1-2 and outputs count on PC3-0
/*====================================================*/
#include "STM32L1xx.h" /* Microcontroller information */

/* Define global variables */
signed char count = 0;

/*---------------------------------------------------*/
/* Counter function - decade up/down counter between 0 & 9 */
/* Green LED = counting up, Blue LED = counting down */
/*---------------------------------------------------*/
void counting (unsigned char *sw2) {
	if (!(*sw2)) { 							     //sw2 == 0? increment 
		GPIOC->BSRR = 0x0100 << 16;             //Reset PC8=0 and turn off blue LED
		GPIOC->BSRR = 0x0200;                   //Set PC9=1 and turn on green LED
		count++;                                //Increment counter
		if ( count > 9 ) {       //Cycle back to 0 if incrementing
			count = 0;
		}
	} else {                                    //sw2==!0? decrement
	    GPIOC->BSRR = 0x0200 << 16;             //Reset PC9=0 and turn off green LED
	    GPIOC->BSRR = 0x0100;                   //Set PC8=1 and turn on blue LED
	    count--;                                //decrement counter
	    if ( count < 0 ) {       //Cycle back to 9 if decrementing
	    	count = 9;
	    }
	}
	GPIOC->ODR = count;                        //Update PC3-PC0 output to match count
}
/*----------------------------------------------------------*/
/* Delay function - do nothing for about 1 second */
/*----------------------------------------------------------*/
void delay () {
	int i,j,n;
	for (j=0; j<1000; j++) { //inner loop
		n = j; //dummy operation for single-step test
	} //do nothing
}
/*------------------------------------------------*/
/* Main program */
/*------------------------------------------------*/


int main(void) {
	unsigned char sw1 = 0; //state of sw1 (PA1)
	unsigned char sw2 = 0; //state of sw2 (PA2)
	
/*---------------------------------------------------*/
/* Initialize GPIO pins used in the program */
/* PC3-0 = virtual LEDs */
/* PC8 = blue LED, PC9 = green LED */
/*---------------------------------------------------*/
	
	/* Configure PA0 as input pin to read push button */
	RCC->AHBENR |= 0x01; /* Enable GPIOA clock (bit 0) */
	GPIOA->MODER &= ~(0x00000003C); /* General purpose input mode */
	   
	/* Configure PC3-PC0 as output pins to drive virtual LEDs */
	RCC->AHBENR |= 0x04;           // Enable GPIOC clock (bit 2) 
	GPIOC->MODER &= ~(0x000000FF); // Clear PC3-PC0 mode bits 
	GPIOC->MODER |= (0x00000055);  // General purpose output mode
	GPIOC->BSRR = 0x000F << 16;    // Reset PC3-PC0 output bits to 0
	   
	/* Configure PC8,PC9 as output pins to drive LEDs */
	GPIOC->MODER &= ~(0x000F0000); // Clear PC9-PC8 mode bits
	GPIOC->MODER |= (0x00050000);  // General purpose output mode
	GPIOC->BSRR = 0x0300 << 16;    // Reset PC9-PC8 output bits to 0
	
	/* Endless loop */
	while (1) {  
	 
		sw1 = GPIOA->IDR & 0x2; //Read GPIOA inputs and mask all but bit 1	 
		sw2 = GPIOA->IDR & 0x4; //Read GPIOA inputs and mask all but bit 2
		
		if (sw1 == 0x2) {
			counting(&sw2);
			delay();
		}
	 
	} /* repeat forever */
} 
