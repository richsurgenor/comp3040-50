/*====================================================
 Hunter Walden and Rich Surgenor
 ELEC 3040/3050 - Lab 7, Program 1
 Description: PWM
====================================================*/

#include "STM32L1xx.h" /* Microcontroller information */
#include "interrupt_handler.h"
#include "keypad.h"
#include "common.h"
#include "timers.h"

//TODO: Move constants out of functions
/* Updates the ODR with latest counter/LED information available */

int event_loop(void)
{
	/* Endless loop */
	while (1) {
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
	toggle_timers();
	
	set_frequency(10 * 1000);
	TIM10->CCR1 = (TIM10->ARR / 2);

	event_loop();
}
