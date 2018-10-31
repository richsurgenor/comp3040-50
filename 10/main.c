/*====================================================
 Hunter Walden and Rich Surgenor
 ELEC 3040/3050 - Lab 9, Program 1
 Description: Tachometer
====================================================*/

#include "STM32L1xx.h" /* Microcontroller information */
#include "interrupt_handler.h"
#include "keypad.h"
#include "common.h"
#include "timers.h"

//TODO: Move constants out of functions
/* Updates the ODR with latest counter/LED information available */

double frequency;
double period;

int event_loop(void)
{
	/* Endless loop */
	
	while (1) {
			frequency = get_tachometer_frequency();
			period = 1/frequency;
		/*delay(COUNTER_0_DELAY);
		counting0();

		if (!display_keypad_count) {
			update_counters();
			GPIOC->ODR |= GREEN_LED;
		} else {
			GPIOC->ODR &= ~(GREEN_LED);
			display_keypad_count--;
		}*/

		//update_counters();

	} /* repeat forever */
}

int main(void)
{
	enable_keypad_GPIO();
	init_timers();
	enable_counter_GPIO();
	enable_timer_GPIO();
	init_interrupts(&SYSCFG->EXTICR[0]); // EXTICR[0] will select EXTI3-0
	toggle_timers(10);
	toggle_timers(11);

	set_frequency(500);
	TIM10->CCR1 = (TIM10->ARR / 2);

	event_loop();
}
