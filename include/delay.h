
#ifndef INCLUDE_DELAY
#define INCLUDE_DELAY

#include "pit.h"                       
#include "8259.h"
#include "gdt.h"
#include "idt.h"

void delay(unsigned long milliseconds);
unsigned long calibrateDelayLoop(void);

#endif
