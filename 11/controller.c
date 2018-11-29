#include "controller.h"


#define KP (uint32_t) 2
#define KI (uint32_t) 2
#define KD (uint32_t) 2

#define MAX_VALUE (uint16_t) 0xffff

uint16_t SP = 250;
uint16_t PV = 0;

bool ENABLED = true;

int integral = 0;
int last_error = 0;
int derivative = 0;

int CV = 0;

void control_loop(void)
{
	if (!ENABLED) {
		return;
	}
	
	int error = SP - PV;
	integral = integral + error;
	derivative = error - last_error;
	
	CV = KP*error + KI*integral + KP*derivative;
	
	uint32_t sum = CV + TIM10->CCR1;
		
	if ( sum < 0xffff ) {
		TIM10->CCR1 = TIM10->CCR1 + CV;
	} else {
		TIM10->CCR1 = 0xffff;
	}
	
	last_error = error;
}

void set_SP(uint16_t val_SP)
{
	SP = val_SP;
	integral = 0;
	last_error = 0;
	derivative = 0;
}

void set_PV(uint16_t val_PV)
{
	PV = val_PV;
}

void disable_controller()
{
	ENABLED = false;
}

void enable_controller()
{
	ENABLED = true;
}

bool is_controller_enabled()
{
	return ENABLED;
}
