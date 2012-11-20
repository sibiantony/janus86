

#include "pit.h"
#include "x86.h"

extern void pit_ISR();

void init_pit(float h, unsigned char channel)
{
	kprintf("PIT initialising", 0, 0);
	getch();

	unsigned int temp = 0;

	temp = 1193180 / h;

	outportb(TMR_CTRL, (channel * 0x40) + TMR_BOTH + TMR_MD3);
	outportb((0x40 + channel), (unsigned char) temp);
	outportb((0x40 + channel), (unsigned char) (temp >> 8));
	kprintf("PIT initialised", 0, 0);
	getch();

}

unsigned int pit_getchannel(unsigned char channel)
{
	unsigned int x = 0;

	outportb(TMR_CTRL, (channel * 0x40) + TMR_LATCH);
	x = inportb(0x40 + channel);
	x += (inportb(0x40 + channel) << 8);
	return x;
}

asm(".globl pit_ISR         \n" 
	"pit_ISR:               \n" 
	"   pusha               \n"	/* Save all registers               */
    	"   pushw %ds           \n"	/* Set up the data segment          */
    	"   pushw %es           \n" 
	"   pushw %ss           \n"	/* Note that ss is always valid     */
    	"   pushw %ss           \n" 
	"   popw %ds            \n" 
	"   popw %es            \n" 
	"                       \n" 
	"   call pit_handler    \n" 
	"                       \n" 
	"   popw %es            \n" 
	"   popw %ds            \n"	/* Restore registers                */
    	"   popa                \n" 
	"   iret                \n"	/* Exit interrupt                   */
    );

void pit_handler()
{
	asm("int $0x1C");	// Extra timer interrupt, used by floppy for timeout etc..      
	outportb(0x20, 0x20);
}
