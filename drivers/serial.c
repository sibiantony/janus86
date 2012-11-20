#include <serial.h>

int init_serial()
{
	outportb(PORT1 + 1, 0);
	outportb(PORT1 + 3, 0x80);
	outportb(PORT1 + 0, 0x03);
	outportb(PORT1 + 1, 0x00);
	outportb(PORT1 + 3, 0x03);
	outportb(PORT1 + 2, 0xC7);
	outportb(PORT1 + 4, 0x0B);
}


int send()
{
	char *buffer, /*c */ c, ch, b;
	init_serial();
	do {
		c = inportb(PORT1 + 5);
		if (c & 1) {
			ch = inportb(PORT1);
			kprintf("%c", ch);
		}
		b = getch();
		outportb(PORT1, b);
	} while (ch != 27);

}

int receive()
{
	char c, ch;
	init_serial();
	do {
		c = inportb(PORT1 + 5);
		if (c & 1) {
			ch = inportb(PORT1);
			kprintf("%c", ch);
		}
	} while (ch != '\n');
}
