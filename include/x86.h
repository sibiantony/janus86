
/*
	x86.h : This file is for using the x86 instructions in C
*/

#ifndef __X86_H
#define __X86_H

#define enable_ints sti
#define disable_ints cli



/* inb: get a byte from the selected port*/
__inline__ unsigned inportb(unsigned port);

/* outb: send the selected byte to the selected port*/
__inline__ void outportb(unsigned port, unsigned val);

/* Enable Interrupts: */
__inline__ void sti();

/* Disable Interrupts: */
__inline__ void cli();

/* A reboot function for rebooting the computer: */
void reboot();


int checkints;		/* for checking if interrupts are disabled or enabled*/

#endif
