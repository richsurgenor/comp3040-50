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

double Vin = 0;
double Vref = 3;
double size = 4095; // (2^12)-1
double Vin_avg;


int event_loop(void)
{
	/* Endless loop */
	
	while(true) {
		int avg = 100;
		double sum = 0;
		for (int n=0; n<avg; n++)
		{
			ADC1->CR2 |= ADC_CR2_SWSTART; // (0x40000000)

			while (ADC1->SR & ADC_SR_EOC) { // ADC_SR_EOC = 0x2

				Vin = (( (double) ADC1->DR ) * Vref ) / size;
				sum = sum + Vin;
			}
		}
		Vin_avg = sum/avg;
	} /* repeat forever */
	return 0;
}

int main(void)
{
	enable_keypad_GPIO();
	init_timers();
	enable_counter_GPIO();
	enable_timer_GPIO();
	enable_ADC();
	init_interrupts(&SYSCFG->EXTICR[0]); // EXTICR[0] will select EXTI3-0
	toggle_timers(10);
	//toggle_timers(11);

	set_frequency(500);
	TIM10->CCR1 = (TIM10->ARR / 2);

	event_loop();
}
