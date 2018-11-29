#include "stdint.h"
#include "STM32L1xx.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

void control_loop(void);
void set_SP(uint16_t val_SP);
void set_PV(uint16_t val_PV);


#endif
