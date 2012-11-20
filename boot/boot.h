#ifndef __BOOT_H
#define __BOOT_H

/* kernel load defines		*/
#define LOAD_ADDR	0x10000
#define LOAD_SEG	0x1000
#define LOAD_OFFSET	0x0000
#define LOAD_SIZE	0x40

/* The code and data segment descriptor values */
#define PMODE_CODESEG	0x08
#define PMODE_DATASEG	0x10
#define RMODE_CODESEG	0x18
#define RMODE_DATASEG	0x20

/* some miscellaneous definitions	*/
#define PMODE_STACK	0x90000
#define BIOS_BOOT_ADDR	0x7C00
#define BIOS_BOOT_SEG	0x07C0

/* At times, we need to compute the absolute address 
	where the code is executed */
#define ABSADDR(x)	((x) - LOAD_ADDR)

#endif
