/*
common.h
Common variables/functions throughout the program
*/

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