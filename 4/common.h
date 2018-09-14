/*
common.h
Common variables/functions throughout the program
*/

#ifndef COMMON_H
#define COMMON_H

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

extern volatile counter counters[];

#endif