#include <keyboard.h>

/* Gets the character next entered */
KEY const getch()
{
	RAWKEY scan;
	KEY retchar;

	/* Get the scancode */
	scan = getrawkey();
	/* Check to see if the shift key is down */
	if (shift)
		retchar = asciiShift[scan];
	/* Not down*/
	else if (!shift)
		retchar = asciiNonSh[scan];
	return retchar;
}


/* Gets the raw key value of the character entered */
UINT GetRawKey()
{
	RAWKEY scancode;

	while (TRUE) {
		while ((inportb(0x64) & 1) == FALSE);
		/* KEYPORT is the keyboard port for getting keys */
		scancode = getrawkeynow();

		/* Check to see if a key is up */
		if (scancode & KEYPRESS) {
			scancode &= 0x7F;

			/* A key is up, check to see if the shift key is up */
			if (scancode == KRLEFT_SHIFT
			    || scancode == KRRIGHT_SHIFT)
				shift = FALSE;	
			continue;	
		}
		/* Check to see if the shift key is down */
		if (scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT) {
			shift = TRUE;	
			continue;	
		}
		return scancode;
	}
}


UINT GetRawKeyNow()
{
	int rawkey;		/* For getting the raw scancode */

	rawkey = inportb(KEYPORT);	/* KEYPORT is 0x60 - the keyboard's keys port */

	return rawkey;
}
