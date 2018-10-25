/*
keypad.c
Hndles keypad functions/initialization
*/

#include "keypad.h"
#include <math.h>

char keys[4][4] = { {0x1, 0x2, 0x3, 0xa},
                  {0x4, 0x5, 0x6, 0xb},
                  {0x7, 0x8, 0x9, 0xc},
                  {0xf, 0x0, 0xe, 0xd} };

void enable_keypad_GPIO(void)
{
	/* Configure PA0 as input pin to read push button */
	RCC->AHBENR |= 0x01; /* Enable GPIOA clock (bit 0) */
	GPIOA->MODER &= ~(0x00000000C); /* General purpose input mode */

	/* Configure PB3-0 as inputs & PB7-4 as outputs */
	RCC->AHBENR |= 0x02; /* Enable GPIOB clock (bit 1) */
	GPIOB->MODER &= ~(0x0000FFFF); // Clear PB7-PB0 mode bits
	GPIOB->MODER |= (0x00005500);  // General purpose output mode PB7-4

	GPIOB->PUPDR &= ~(0x000000FF); // Clear PB3-PB0 mode bits
	GPIOB->PUPDR |= (0x00000055);  // Set PB3-PB0 to pull-up resistors

  GPIOB->ODR &= ~(0xF0); // Clear PB7-4
}

location detect(bool* failure)
{
  location loc = {.col=0, .row=0};

  //loop through each column
  //drive col low
  //check if row is shorted or not

  for (int i = 0; i < COL_SIZE; i++) {
    //All cols low, except  for pin4+i
		GPIOB->ODR = (0xf << 4);
    GPIOB->ODR = GPIOB->ODR ^ (1 << (4+i));

    //small delay
    int loops = 50;
    for (int z = 0; z < loops; z++);

    for (int j = 0; j < ROW_SIZE; j++) {

      // if row(j), or pin(j) is low, the button on col(i)row(j) is pressed.
      if ( !(GPIOB->IDR & (1 << j)) ) {
          loc.col=i;
          loc.row=j;
          return loc;
      }
    }
  }

  *failure = true;
  return loc;
}

//1. ALl columns are driven to 0 logic
//2. Keypad is pressed. interrupt begins
//3. Drive a single column at a time to 0 to figure what button was pressed
