/* interrupt_handler.h
 */
 
#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H
 
#include "STM32L1xx.h" 
#include "stdint.h"
 
/*
typedef enum
{
  EXTI_Trigger_Rising = 0x08,
  EXTI_Trigger_Falling = 0x0C,  
  EXTI_Trigger_Rising_Falling = 0x10
}EXTITrigger_TypeDef;
*/

void init_interrupts(volatile uint32_t *SYSCFG_EXTICR);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);

#endif
  