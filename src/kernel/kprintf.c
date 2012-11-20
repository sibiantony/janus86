#include "kprintf.h"

int vsnprintf( char *str, size_t size, const char *fmt, va_list args)
{
	unsigned state, flags, radix, actual_wd, count, given_wd;
	unsigned char *where, buf[ PR_BUFLEN ];
	long num;

	state = flags = count = given_wd = 0;

	/* Begin scanning the format string . */
	for ( ; *fmt; fmt++ )
	{
		switch (state)
		{
			case 0:
				if (*fmt != '%') {	
					*str++ = *fmt;	/* not % .. add it to str. */
					count++;
					break;
				}
	/* Found %, get next character and advance state to check if next char is a flag*/
				state++;
				fmt++;
			/* state 1: awaiting flags ( % - 0 ) */
			case 1:
				if (*fmt == '%') {	/* %%	*/
					*str++ = *fmt;
					count++;
					state = flags = given_wd = 0;
					break;
				}

				if ( *fmt == '-' ) {
					if ( flags & PR_LJFY )	/* %-- is illegal */
						state = flags = given_wd = 0;
					else
						flags |= PR_LJFY;
					break;
				}
			/* Not a flag char: advance state to check if it's field width */
				state++;
			/* Check now for '%0...' */
				if ( *fmt == '0' ) {
					flags |= PR_ZEROPAD;
					fmt++;
				}
			/* state 2: awaiting (numeric) field width */
			case 2:
				if ( *fmt >= '0' && *fmt <= '9' ) {
					given_wd = 10 * given_wd +
						( *fmt - '0' );
					break;
				}
	/* Not field width: advance state to check if it's a modifier */
				state++;
			/* state 3: awaiting modifier chars ( FNlh ) */
			case 3:
				if ( *fmt == 'F' ) {
					flags |= PR_FARPTR;
					break;
				}
				if ( *fmt == 'N' )
					break;
				if ( *fmt == 'l' ) {
					flags |= PR_NUM32;
					break;
				}
				if ( *fmt == 'h' ) {
					flags |= PR_NUM16;
					break;
				}

	/* Not modifier: advance state to check if it's a conversion char */
				state++;
			/* state 4: awaiting conversion chars ( Xxpndiuocs ) */
			case 4:
				where = buf + PR_BUFLEN - 1;
				*where = '\0';
				switch ( *fmt )
				{
					case 'X':
						flags |= PR_CAPS;
			/* xxx - far pointers (%Fp, %Fn) not yet supported */
					case 'x':
					case 'p':
					case 'n':
						radix = 16;
						goto DO_NUM;
					case 'd':
					case 'i':
						flags |= PR_SIGN;
					case 'u':
						radix = 10;
						goto DO_NUM;
					case 'o':
						radix = 8;
		/* Load the value to be printed. l=long=32 bits:	*/
DO_NUM:					if ( flags & PR_NUM32 )
						num = va_arg( args, unsigned long );
		/* h=short=16 bits ( signed or unsigned )	*/
					else if ( flags & PR_NUM16 ) {
						if ( flags & PR_SIGN )
							num = va_arg( args, short );
						else
							num = va_arg( args, unsigned short );
					}

		/* No h nor l: sizeof(int) bits ( signed or unsigned ) */
					else {
						if ( flags & PR_SIGN )
							num = va_arg( args, int );
						else
							num = va_arg( args, unsigned int );
					}
		/* Take care of sign */
					if ( flags & PR_SIGN ) {
						if ( num < 0 ) {
							flags |= PR_NUM0;
							num = -num;
						}
					}

		/* Convert binary to octal/decimal/hex ASCII
		 The math here is ALWAYS unsigned	*/
					do {
						unsigned long temp;
						temp = ( unsigned long ) num % radix;
						where--;
						if ( temp < 10 )
							*where = temp + '0';

						else if ( flags & PR_CAPS )
							*where = temp - 10 + 'A';

						else
							*where = temp - 10 + 'a';
						num = ( unsigned long ) num / radix;
					}
					while ( num != 0 );
					goto EMIT;
					case 'c':
		/* Disallow pad-left-with-zeroes for %c	*/
						flags &= ~PR_ZEROPAD;
						where--;

					*where = ( unsigned char )va_arg(args, unsigned char);
					actual_wd = 1;
					goto EMIT2;

					case 's':
		/* Disallow pad-left-with-zeroes for %s	*/
						flags &= ~PR_ZEROPAD;
				where = va_arg( args, unsigned char * );
EMIT:
				actual_wd = strlen( ( char* )where );

				if ( flags & PR_NUM0 )
					actual_wd++;

		/* If we pad left with ZEROES, do the sign now	*/
				if ( ( flags & ( PR_NUM0 | 
					PR_ZEROPAD ) ) == ( PR_NUM0 | PR_ZEROPAD ) ) {
					*str++ = '-';
					count++;
				}

		/* pad on left with spaces or zeroes ( for right justify )*/
EMIT2:				if ( ( flags & PR_LJFY ) == 0 ) {
					while ( given_wd > actual_wd ) {
						*str++ = flags & PR_ZEROPAD ? '0' :' ';
						count++;
						given_wd--;
					}
				}
		/* If we pad left with SPACES, do the sign now */
				if ( ( flags & ( PR_NUM0 | 
					PR_ZEROPAD ) ) == PR_NUM0 ) {
					*str++ = '-'; 
					count++;
				}
		/* Emit string/char/converted number	*/
				while ( *where != '\0' ) {
					*str++ = *where++;
					count++;
				}
		/* pad on right with spaces ( for left justify )	*/
				if ( given_wd < actual_wd )
					given_wd = 0;
				else
					given_wd -= actual_wd;
				for ( ; given_wd; given_wd-- )
				{
					*str++ = ' ';
					count++;
				}
				break;
			default:
				break;
			}
		default:
			state = flags = given_wd = 0;
			break;
		}
	}
	*str = '\0';
	return count;
}

int snprintf(char *str,size_t size,const char *fmt,...)
{
	va_list vargs;
	int ret;

	va_start(vargs,fmt);
	ret = vsnprintf(str, size, fmt, vargs);
	va_end(vargs);

	return ret;
}

unsigned int kprintf(const char *fmt, ...)
{
	va_list vargs;
	char buffer[BUFFER_SIZE];
	int buffer_index = 0;
	int ret;

	char *vidmem = (char *)VIDEO_MEM;
	unsigned  int i = (cursor_y * TEXTMODE_COL + cursor_x) * 2;
	
	va_start (vargs, fmt);
	ret = vsnprintf (buffer, BUFFER_SIZE, fmt, vargs);
	va_end (vargs);

  	while(buffer[buffer_index] != 0)
    	{
      		while (buffer[buffer_index] == '\n')
		{
			if (cursor_y == TEXTMODE_ROW)
				kscroll_display ();
			else
	  			set_cursor_position (0, cursor_y + 1);
		  	i = (cursor_y * TEXTMODE_COL * 2);
			buffer_index ++;
			if (buffer[buffer_index] == 0) 
				return ret;
		};
      		if (i == TEXTMODE_ROW * TEXTMODE_COL * 2)
	    	{
			kscroll_display();
			i = (TEXTMODE_ROW - 1) * TEXTMODE_COL * 2;
		}
	  	vidmem[i] = buffer[buffer_index];
	  	buffer_index ++;
	  	i ++;
	  	vidmem[i] = WHITE ;
	  	i ++;
		set_cursor_position (cursor_x + 1,cursor_y );
    	}
  	return ret;
}

