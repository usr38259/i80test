
#include <stdio.h>
#include <memory.h>
#include <conio.h>
#include <io.h>
#include "i8x.h"

#define TEXT_BIN "8080exer.com"

#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

struct i8085_state i80;
unsigned char mem [65536];

int _CRTAPI1 main ()
{
	FILE *f;
	long fl;
	int i;

	memset (mem, sizeof (mem), 0);
	f = fopen (TEXT_BIN, "rb");
	if (f == NULL) {
		puts ("BIN file '" TEXT_BIN "' open error");
		return 1;
	}
	fl = filelength (fileno (f));
	if (fl <= 0 || fl >= 65536) {
		if (ferror (f)) {
			puts ("File I/O error:");
			perror (NULL);
		} else	puts ("File length error");
		fclose (f);
		return 1;
	}
	if (fread (mem + 0x0100, fl, 1, f) != 1) {
		puts ("BIN file '" TEXT_BIN "' read error");
		fclose (f);
		return 1;
	}
	fclose (f);

	i8080_cpu_init (&i80);
	i808x_cpu_reset (&i80);
	i80.SP.w.l = 0x7ffc;
	*(unsigned short*)&mem [0x7ffe] = 0x7676;
	*(unsigned short*)&mem [0x7ffc] = 0x7ffe;
	mem [0x0005] = 0xC9;
	*(unsigned short*)&mem [0x0006] = 0x7ffc;
	i80.PC.w.l = 0x0100;

	while (1) {
		if (i80.PC.w.l == 0) break;
		if (i80.PC.w.l == 5) switch (i80.BC.b.l) {
		case 2: putch (i80.DE.b.l); break;
		case 9: for (i = i80.DE.w.l; mem [i] != '$'; i++) putch (mem [i]); break;
		default: puts ("Unsupported BDOS call"); return 0;
		}
		if (mem [i80.PC.w.l] == 0x76) {
			printf ("Halt at %04X\n", i80.PC.w.l); break; }
		i808x_cpu_execute (&i80);
	}
	return 0;
}
