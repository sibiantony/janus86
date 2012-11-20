
#include <video.h>

/* Clear the screen	*/
void kclear_screen()
{
	char *vidmem;
	vidmem = (char *) VIDEO_MEM;
	unsigned int i = 0;

	while (i < TEXTMODE_COL * TEXTMODE_ROW) {
		*vidmem++ = ' ';
		*vidmem++ = BLACK;
		i++;
	}
	cursor_x = 10;		// modify to prompt size 
	cursor_y = 0;
}

/* Scroll the display one line up	*/
void kscroll_display()
{
	int i;
	char *vidmem = (char *) VIDEO_MEM;

	for (i = 0; i <= TEXTMODE_COL * (TEXTMODE_ROW - 1) * 2; i++)
		*(vidmem + i) = *(vidmem + i + 2 * TEXTMODE_COL);
	for (i = (TEXTMODE_ROW - 1) * TEXTMODE_COL * 2;
	     i < TEXTMODE_ROW * TEXTMODE_COL * 2; i += 2) {
		*(vidmem + i) = ' ';
		*(vidmem + i + 1) = BLACK;
	}
	set_cursor_position(0, cursor_y - 1);
}

void set_cursor_position(int x, int y)
{
	unsigned cursor_pos;
	char *vidmem = (char *) VIDEO_MEM;

	if (x < 0)
		cursor_x = 0;
	else
		cursor_x = x;
	if (x == TEXTMODE_COL) {
		cursor_x = 0;
		y++;
	}
	if (y < 0 || y > TEXTMODE_ROW)
		cursor_y = 0;
	else
		cursor_y = y;
	/* display the cursor   */
	cursor_pos = cursor_y * TEXTMODE_COL + cursor_x;
		outportb(VID_INDEX_PORT, 14);	/* write to register 14 */
		outportb(VID_DATA_PORT, (cursor_pos >> 8) & 0xFF);	/* output high byte */
		outportb(VID_INDEX_PORT, 15);	/* now to register 15 */
		outportb(VID_DATA_PORT, cursor_pos & 0xFF);	/* output low byte */
	*(vidmem + 2 * cursor_pos + 1) = WHITE;
}
