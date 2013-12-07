#ifndef GDT_H
#define GDT_H

#include "types.h"

#define GDTBASE    0x0		/* addr. physique ou doit resider la gdt */
#define GDTSIZE    0xFF		/* nombre max. de descripteurs dans la table */

/* Descripteur de segment */
struct gdtdesc {
	u16 lim0_15;
	u16 base0_15;
	u8 base16_23;
	u8 acces;
	u8 lim16_19:4; //_other;
	u8 other:4;
	u8 base24_31;
} __attribute__ ((packed));

/* Registre GDTR */
struct gdtr {
	u16 limite;
	u32 base;
} __attribute__ ((packed));

extern void _init_gdt_desc(u32, u32, u8, u8, struct gdtdesc *);
extern void _memcpy(char *dest, char *source, int size);

#endif
