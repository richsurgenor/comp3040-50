/*
common.h
Common variables/functions throughout the program
*/

#include "stdint.h"
#include "STM32L1xx.h"

#ifndef COMMON_H
#define COMMON_H


#define COUNTERS_SIZE (uint16_t) 2
#define COUNTER_HIGH (uint8_t) 9
#define COUNTER_LOW (uint8_t) 0

#define GREEN_LED (uint16_t) 0x0200
#define BLUE_LED (uint16_t) 0x0100

#define COUNTER_0_DELAY (uint16_t) 1000 // in miliseconds

#define PULLUP (uint8_t) 0x01
#define PULLDOWN uint8_t 0x10

typedef enum {
	COUNTING_UP = 1,
	COUNTING_DOWN = 0
}COUNTING_DIRECTION;

typedef struct {
		signed int count : 5;
		unsigned int direction : 1;
		unsigned int speed : 16; // not yet used
} counter;

void delay (int ms);
void enable_counter_GPIO(void);
void counting0(void);
void update_counters(void);

extern volatile counter counters[];

#endif
