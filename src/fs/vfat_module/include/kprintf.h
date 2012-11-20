#ifndef __PRINTF_H
#define __PRINTF_H

/*
 * Various ..printf() functions are here. They will be used
 * generally for any formatted output that the kernel needs
 */
#include "stdlib.h"
#include "video.h"
#include "string.h"

/* printf defines */
#define	PR_LJFY		0x01	/* left Justify	*/
#define	PR_CAPS		0x02	/* Use A-F Instead Of a-f For Hex Numbers	*/
#define	PR_SIGN		0x04	/* Signed Numeric Conversion ( %d vs. %u )	*/
#define	PR_NUM32	0x08	/* Long ( 32-bit ) Numeric Conversion		*/
#define	PR_NUM16	0x10	/* Short ( 16-bit ) Numeric Conversion		*/
#define	PR_NUM0		0x20	/* PR_SG set and num was < 0		*/
#define	PR_ZEROPAD	0x40	/* Pad Left With '0' Instead Of ' '	*/
#define	PR_FARPTR	0x80	/* Pointers Are Far			*/

#define	PR_BUFLEN	16
/* For the kprintf buffer */
#define BUFFER_SIZE	100

int 		snprintf(char *, size_t, const char *, ...);
unsigned int  	kprintf(const char *, ...);

#endif
