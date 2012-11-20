
#include "x86.h"
#include "8259.h"

unsigned int irq_mask = 0xFFFF;	// All IRQs disabled initially

/* init8259() initialises the 8259 Programmable Interrupt Controller */

void Init8259(void)
{
	kprintf("8259 initialising", 0, 0);
	outportb(M_PIC, ICW1);	/* Start 8259 initialization    */
	outportb(S_PIC, ICW1);

	outportb(M_PIC + 1, M_VEC);	/* Base interrupt vector        */
	outportb(S_PIC + 1, S_VEC);

	outportb(M_PIC + 1, 1 << 2);	/* Bitmask for cascade on IRQ 2 */
	outportb(S_PIC + 1, 2);	/* Cascade on IRQ 2             */

	outportb(M_PIC + 1, ICW4);	/* Finish 8259 initialization   */
	outportb(S_PIC + 1, ICW4);

	outportb(M_IMR, 0xff);	/* Mask all interrupts          */
	outportb(S_IMR, 0xff);

	kprintf("8259 initialised", 0, 0);
	getch();
}

/* enables irq irq_no */
void enable_irq(unsigned short irq_no)
{
	irq_mask &= ~(1 << irq_no);
	if (irq_no >= 8)
		irq_mask &= ~(1 << 2);

	outportb(M_PIC + 1, irq_mask & 0xFF);
	outportb(S_PIC + 1, (irq_mask >> 8) & 0xFF);
}

/* disables irq irq_no */
void disable_irq(unsigned short irq_no)
{
	irq_mask |= (1 << irq_no);
	if ((irq_mask & 0xFF00) == 0xFF00)
		irq_mask |= (1 << 2);

	outportb(M_PIC + 1, irq_mask & 0xFF);
	outportb(S_PIC + 1, (irq_mask >> 8) & 0xFF);
}
