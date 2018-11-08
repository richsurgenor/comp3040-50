/*====================================================
 Hunter Walden and Rich Surgenor
 ELEC 3040/3050 - Lab 11, Program 1
 Description: Tachometer
====================================================*/

#include "STM32L1xx.h" /* Microcontroller information */
#include "interrupt_handler.h"
#include "keypad.h"
#include "common.h"
#include "timers.h"

//TODO: Move constants out of functions
/* Updates the ODR with latest counter/LED information available */

/*
Lab 11 Objective -> Measure about 2 seconds of data upon step response
Data is then used to model a plant/process G(s)
Step response -> change of duty cycle, i.e press keypad button triggers new step response
Going to use tachometer period measurements because it is more reliable

Need to record first 2 seconds of step response in an array
	1. Need new timer interrupt (TIM9) to trigger every x amount of seconds (currently 10ms)
	2. When timer interrupt is triggered, record period and store in array
	3. Repeat until we have about 2 seconds of data (DO NOT OVERFILL ARRAY i.e STOP SAMPLING i.e turn off TIM9?)
	4. If step response is triggered (i.e. keypad button pressed) then empty array and measure new 2 seconds
	5. Read Nelson's guide on how to export this data into MATLAB/Excel (Method 2 does this)
	
Plot data in MATLAB and then use big brain to model a function G(s)

*/


int event_loop(void)
{
	/* Endless loop */
	
	while(true) {
							} /* repeat forever */
}

int main(void)
{
	enable_keypad_GPIO();
	init_timers();
	enable_counter_GPIO();
	enable_timer_GPIO();
	//enable_ADC();
	init_interrupts(&SYSCFG->EXTICR[0]); // EXTICR[0] will select EXTI3-0
	toggle_timers(10);
	toggle_timers(11);
	toggle_timers(9);

	set_frequency(500);
	TIM10->CCR1 = (TIM10->ARR / 2);

	event_loop();
}
