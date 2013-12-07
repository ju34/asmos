#include "include/gdt.h"
#include "include/types.h"
#include "include/stdio.h"
#include "include/string.h"

#define IDTSIZE	0xFF

#define cli asm("cli"::)
#define sti asm("sti"::)
#define outb(port,value) \
	 asm volatile("outb %%al, %%dx" :: "d" (port), "a" (value));

#define inb(port) ({    \
	unsigned char _v;       \
	asm volatile ("inb %%dx, %%al" : "=a" (_v) : "d" (port)); \
        _v;     \
})

struct idtdesc{
	unsigned short offset0_15;
	unsigned short select;
	unsigned short type;
	unsigned short offset16_31;
} __attribute__ ((packed));

struct idtr{
	unsigned short limite;
	unsigned int base;
} __attribute__ ((packed));


extern void _asm_default_int();
extern void _asm_keyboard_int();

void init_pic();
void init_idt_desc(unsigned short select, unsigned int offset, unsigned short type, struct idtdesc *desc);
void main();

struct idtr kidtr;
struct idtdesc kidt[IDTSIZE];

void _start(){

	_init_kernel_gdt();

	asm("	movw $0x18, %ax \n \
		movw %ax, %ss \n \
		movl $0x20000, %esp");

        main();

        while(1);
}

void main(void)
{

    _clear();
    _printk("Kernel is runnig ... ");
//	int i = 0;

//	for(i=0; i < IDTSIZE; i++)
//		_init_idt_desc(&kidt[i], (unsigned int) _asm_default_int, 0x08, 0x8E00);
		
//	_init_idt_desc(&kidt[33], (unsigned int) _asm_keyboard_int, 0x08, 0x8E00);

//	kidtr.limite = 0xFF*8;
//	kidtr.base = 0x800;

//        memcpy((char *) kidtr.base, (char *) kidt, kidtr.limite);

//	asm("lidtl (kidtr)");

//	init_pic();
	
//	sti;

//	_clear();
//        _printk("OK");
//	_gotoxy(5,5);
//	_printk("TEST\t");
//	_setattr(0x04);
//	_printk("Encore un test");


	while(1);
}


void init_pic(void)
{
        /* Initialisation de ICW1 */
        outb(0x20, 0x11);
        outb(0xA0, 0x11);

        /* Initialisation de ICW2 */
        outb(0x21, 0x20);       /* vecteur de depart = 32 */
        outb(0xA1, 0x70);       /* vecteur de depart = 96 */

        /* Initialisation de ICW3 */
        outb(0x21, 0x04);
        outb(0xA1, 0x02);

        /* Initialisation de ICW4 */
        outb(0x21, 0x01);
        outb(0xA1, 0x01);

        /* masquage des interruptions */
        outb(0x21, 0x0);
        outb(0xA1, 0x0);
}

void isr_default_int(){
	//_printk("INTERRUPT");
}

int x = 0;
int y = 0; 

void isr_keyboard_int(){
    _printk("Keyboard interrupt");
}
