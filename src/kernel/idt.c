
#include "8259.h"
#include "gdt.h"
#include "idt.h"
#include "x86.h"
#include "kprintf.h"

extern void int_null();
unsigned short gdt_reg[100];

desc_table(IDT, 256)
{
};

struct	{ 
  	unsigned short limit __attribute__ ((packed));
  	union DT_entry *idt  __attribute__ ((packed)); 
} loadidt= { (256 * sizeof(union DT_entry) - 1), IDT };

/* Initialise the Global Descriptor Table */
void init_gdt()
{
	
	void * gdtr_base;
	unsigned short * gdtr_base1;
	char buff[50];
	int i=0;

	asm("sgdt (%0)": :"g" ((char *)gdtr_base1));
	
	gdtr_base=(unsigned short *)IDT;
	snprintf(buff,50,"gdtr_base1 %x reg : %x value1:%x %x %x %x",gdtr_base1,gdt_reg,(unsigned short)*(gdtr_base1)
							,(unsigned short)*(gdtr_base1+1)	
							,(unsigned short)*(gdtr_base1+2)	
							,(unsigned short)*(gdtr_base1+3));

	kprintf(buff,0,0);				
	getch();
	
	for(i=0;i<=80;i+=4)
	{	
	gdt_reg[i+0]=(unsigned short)*(gdtr_base1);	
	gdt_reg[i+1]=(unsigned short)*(gdtr_base1+1);	
	gdt_reg[i+2]=(unsigned short)*(gdtr_base1+2);		
	gdt_reg[i+3]=(unsigned short)*(gdtr_base1+3);	
	gdtr_base1+=4;
	}	

	 
}

void load_gdt()
{
	char buff[50];
	unsigned short * gdtr_base1;

	asm("sgdt (%0)": :"g" ((char *)gdtr_base1));
	asm("lgdt (%0)": :"g" ((char *)gdt_reg));

	snprintf(buff,50,"base1 : %x value1:%x %x %x %x\n",gdt_reg,(unsigned short)*(gdt_reg)
							,(unsigned short)*(gdt_reg+1)	
							,(unsigned short)*(gdt_reg+2)	
							,(unsigned short)*(gdt_reg+3));
							

	kprintf(buff,0,0);				
	getch();
	snprintf(buff,50,"old gdt : %x value1:%x %x %x %x",gdtr_base1,(unsigned short)*(gdtr_base1)
							,(unsigned short)*(gdtr_base1+1)	
							,(unsigned short)*(gdtr_base1+2)	
							,(unsigned short)*(gdtr_base1+3));
	kprintf(buff,0,0);				
	getch();

}

/* Initialise the Interrupt Descriptor Table */
void InitIDT()
{
	int count=0;
	char buff[50];

	for(count = 16; count < 256; count++)
		set_vector(int_null, count, D_PRESENT + D_INT + D_DPL3);

	set_vector(_int0, 0, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int1, 1, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int2, 2, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int3, 3, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int4, 4, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int5, 5, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int6, 6, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int7, 7, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int8, 8, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int9, 9, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int10, 10, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int12, 12, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int13, 13, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int14, 14, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int15, 15, D_PRESENT +  D_INT + D_DPL3);
	set_vector(_int16, 16, D_PRESENT +  D_INT + D_DPL3);

	asm (
		"lidt (%0)                 \n"   /* Load the IDT                */
        	"pushfl                    \n"   /* Clear the NT flag           */
	        "andl $0xffffbfff,(%%esp)  \n"
	        "popfl                     \n"
	        :
	        : "r" ((char *) &loadidt)
	);
	asm("sti");
}

/* Initialise a interrupt vector with address of interrupt handler */
void set_vector(void *handler, unsigned char interrupt, unsigned short control_major)
{
	unsigned short codesegment = 0x08;
	asm volatile("movw %%cs,%0":"=g" (codesegment));
	
   	IDT[interrupt].gate.offset_low    = (unsigned short) (((unsigned long)handler)&0xffff);
 	IDT[interrupt].gate.selector      = codesegment;
	IDT[interrupt].gate.access        = control_major;
	IDT[interrupt].gate.offset_high   = (unsigned short) (((unsigned long)handler) >> 16);
}

asm (
   	".globl int_null        \n"
   	"int_null:              \n"
   	"   iret                \n" /* Exit interrupt                   */
);

/*List of interrupt handlers */

void _int0()
{
	kprintf("int0: Divide Error\n");
	while(1);
}
void _int1()
{
	kprintf("int1: Debug exception\n");
	while(1);
}
void _int2()
{
	kprintf("int2: unknown error\n");
	while(1);
}
void _int3()
{
	kprintf("int3: Breakpoint\n");
	while(1);
}
void _int4()
{
	kprintf("int4: Overflow\n");
	while(1);
}
void _int5()
{
	kprintf("int 5: Bounds check\n");
	while(1);
}
void _int6()
{
	kprintf("int6: Invalid opcode\n");
	while(1);
}
void _int7()
{
	kprintf("int7: Coprocessor not available\n");
	while(1);
}
void _int8()
{
	kprintf("int8: Double fault\n");
	while(1);
}
void _int9()
{
	kprintf("int9: Coprocessor Segment overrun\n");
	while(1);
}
void _int10()
{
	kprintf("int10: Invalid TSS\n");
	while(1);
}
void _int11()
{
	kprintf("int11: Segment not present\n");
	while(1);
}
void _int12()
{
	kprintf("int12: Stack exception\n");
	while(1);
}
void _int13()
{
	kprintf("int13: General Protection\n");
	while(1);
}
void _int14()
{
	kprintf("int14: Page fault\n");
	while(1);
}
void _int15()
{
	kprintf("in15: Unknown error\n");
	while(1);
}
void _int16()
{
	kprintf("int16: Coprocessor error\n");
	while(1);
}

