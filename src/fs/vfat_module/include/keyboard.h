
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "x86.h"
#include "types.h"

/* If someone likes using lower-case functions, define GetRawKey to getrawkey, and others*/

#define getrawkey GetRawKey
#define getrawkeynow GetRawKeyNow

/* Includes: */
#include "keydefs.h"

/* For getting keys from the keyboard. */
UCHAR const getch();

/* For getting raw keyboard scancodes */
UINT GetRawKey();

/* For getting raw keyboard scancodes without waiting for a key. */
UINT GetRawKeyNow();

/* For keeping track if the shift key is down or up*/
int shift;

/* Create a typedef for getting keys, to hold the correct value:*/
typedef UCHAR KEY;

/* Create a raw scancode data type for getting scancodes:*/
typedef UINT RAWKEY;

#endif
