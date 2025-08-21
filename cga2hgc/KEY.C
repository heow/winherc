#include <stdio.h>
#include <dos.h>

typedef void interrupt intfunc ();

intfunc *oldkey;

extern unsigned _heaplen = 500;
extern unsigned _stklen = 500;

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
	int far *mono_ptr;
	int far *colour_ptr;
	int addr;

	mono_ptr = (int far *) 0xB0000000;
	colour_ptr = (int far *) 0xB8000000;

	/* fill mono screen with that in colour */
	for (addr=0; addr < 2000; addr++)
		*(mono_ptr + addr) = *(colour_ptr + addr) | 0x0700;
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