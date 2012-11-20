#ifndef	__VIDEO_H
#define __VIDEO_H

#include "x86.h"
/* define the video mode parameters	*/
#define VIDEO_MEM 	0xB8000
#define TEXTMODE_COL	80
#define TEXTMODE_ROW	25

/* define the color values	*/
#define BLACK		0x00
#define	BLUE		0x01
#define GREEN		0x02
#define CYAN		0x03
#define RED		0x04
#define MAGENTA		0x05
#define BROWN		0x06
#define LIGHT_WHITE	0x07
#define GRAY		0x08
#define LIGHT_BLUE	0x09
#define LIGHT_GREEN	0x0A
#define LIGHT_CYAN	0x0B
#define LIGHT_RED	0x0C
#define LIGHT_MAGENTA	0x0D
#define YELLOW		0x0E
#define WHITE		0x0F

#define BLINK		0x80

/* 6845 Video controller ports defines */
#define VID_INDEX_PORT		0x3D4 /* selects the register [0-11h] to be accessed */
#define VID_DATA_PORT		0x3D5 /* selects the data register [0-11h] */
#define VID_MODECTL_PORT	0x3D8 /* CGA, EGA, VGA, except PCjr	*/
#define VID_SELECT_PALETTE	0x3D9 /* CGA, EGA, VGA	*/
#define VID_STATUS_PORT		0x3DA /* status register , read only */

int cursor_x;
int cursor_y;

void kclear_screen(void);
void kscroll_display(void);
void set_cursor_position(int ,int );

#endif
