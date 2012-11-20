#include <x86.h>

/*
 * Read a byte from an I/O port.
 */
__inline__ unsigned inportb(unsigned port)
{
	unsigned ret_val;

	__asm__ __volatile__("inb %w1,%b0"
		: "=a"(ret_val)
		: "d"(port));

	return ret_val;
}

/*
 * Write a byte to an I/O port.
 */
__inline__ void outportb(unsigned port, unsigned val)
{
	__asm__ __volatile__("outb %b0,%w1"
		:
		: "a"(val), "d"(port));
}


/*
 * Read a word from an I/O port.
 */
__inline__ unsigned inportw(unsigned port)
{
    unsigned ret_val;

    __asm__ __volatile__ ("inw %w1, %w0"
	: "=a" (ret_val)
	: "d" (port)
    );

    return ret_val;
}

/*
 * Write a word to an I/O port.
 */
__inline__ void outportw(unsigned port, unsigned val)
{
    __asm__ __volatile__ ("outw %w0, %w1"
	:
	: "a" (val), "Nd" (port)
    );
}


void reboot()
{
	int temp;	/* A temporary int for storing keyboard info. The keyboard can be use to
			  reboot the PC */
	/* flush the keyboard controller */
        do
        {
                temp = inportb( 0x64 );
                if ( temp & 1 )
                        inportb( 0x60 );
        }
	while ( temp & 2 );

        /*  Reboot the computer... */
        outportb(0x64, 0xFE);
}

/*
 * Set the interrupt flag.
 */
__inline__ void sti()
{
	__asm__ __volatile__( "sti" );	

	/* Set checkints to 1 - on */
	checkints = 1;
}


/*
 * Clear the interrupt flag.
 */
__inline__ void cli()
{
	__asm__ __volatile__( "cli" );	

	/* Set checkints to 0 - off */
	checkints = 0;
}
