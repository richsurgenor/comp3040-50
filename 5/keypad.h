/*
keypad.h
*/

#ifndef KEYPAD_H
#define KEYPAD_H

#include "STM32L1xx.h"
#include "stdint.h"
#include "stdbool.h"

#define COL_SIZE (uint8_t) 4
#define ROW_SIZE (uint8_t) 4
#define SECONDS_TO_DISPLAY_KEYPAD_PRESS (uint8_t) 5

typedef struct {
		unsigned int row;
		unsigned int col;
} location;

void enable_keypad_GPIO(void);
location detect(bool *failure);

extern char keys[4][4];

#endif
