#include <stdio.h>
#include <dos.h>
#include <bios.h>

typedef void interrupt intfunc ();

intfunc *oldkey;

extern unsigned _heaplen = 1024;
extern unsigned _stklen = 512;
extern int far *mono_ptr = 0xB0000000;
extern int far *colour_ptr = 0xB8000000;

typedef struct shiftkey1
	{
	unsigned Rshift:1;
	unsigned Lshift:1;
	unsigned Ctrl:1;
	unsigned Alt:1;
	unsigned Scroll:1;
	unsigned Num:1;
	unsigned Caps:1;
	unsigned Insert:1;
	} shkey1_t;

typedef unsigned char screen_t[25] [80] [2];
screen_t far *screen;
shkey1_t huge *shift;

void printit (void)
	{
	int addr;
	int i=0;

	/* set up: COM2:9600,8,1 */
	bioscom (0, (0xE0|0x00|0x00|0x03), 1);
	bioscom (1, 10, 1);
	bioscom (1, 13, 1);

	/* send com2 the values of colour address */
	for (addr=0; addr < 2000; addr++)
		{
		i++;
		if (i == 80)
			{
			i = 0;
			bioscom (1, 13, 1); /* CR and LF */
			bioscom (1, 10, 1);
			}
		bioscom (1, *(colour_ptr + addr), 1);
		}
	}


void interrupt newkey (void)
	{
	(*oldkey)();

	if (shift -> Ctrl && shift -> Alt)
		printit ();
	}

void main (void)
	{
	unsigned mem = (_SP/16L) +1;

	screen = (screen_t far *) 0xb8000000;

	shift = (shkey1_t huge * )0x417;

	oldkey = (intfunc *) getvect (0x9);
	setvect (0x9, newkey);
	keep (0,mem);

	}