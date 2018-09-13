
#include "interrupt_handler.h"
#include "common.h"

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

// TODO: make more generic
void init_interrupts(volatile uint32_t *SYSCFG_EXTICR) { 
	//SystemInit();
	//RCC_APB2ENR_SYSCFGEN
	__enable_irq();
	
	//pins 3-0 are set to 0, configured for PA-0
	*SYSCFG_EXTICR &= 0xFFF0;
	
	//pins 7-4 are set to 0, configured for PA-1
	*SYSCFG_EXTICR &= 0xFF0F;
	
	//EXTI0 and EXTI1 are both rising edge triggered
	EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR1;
	
	//EXTI0 and EXT1 lines are both unmasked
	EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1;
	
	//Enable NVIC IRQ lines for 
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);

	NVIC_SetPriority(EXTI0_IRQn, 1);
	NVIC_SetPriority(EXTI1_IRQn, 1);
}



