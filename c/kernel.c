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

extern void _setattr(unsigned char attr);
extern void _clear();
extern void _printk(unsigned char *str);
extern void _putcar(unsigned char c);
extern void _gotoxy(int x, int y);
extern void _init_idt_desc(struct idtdesc *desc, int offset, unsigned short select, unsigned short type);

extern void _asm_default_int();
extern void _asm_keyboard_int();

void *memcpy(char *dest, char *src, int size);
void init_pic();
void init_idt_desc(unsigned short select, unsigned int offset, unsigned short type, struct idtdesc *desc);

struct idtr kidtr;
struct idtdesc kidt[IDTSIZE];

void _start(void)
{
	int i = 0;

	for(i=0; i < IDTSIZE; i++)
		_init_idt_desc(&kidt[i], (unsigned int) _asm_default_int, 0x08, 0x8E00);
		
	_init_idt_desc(&kidt[33], (unsigned int) _asm_keyboard_int, 0x08, 0x8E00);

	kidtr.limite = 0xFF*8;
	kidtr.base = 0x800;

	memcpy((char *) kidtr.base, (char *) kidt, kidtr.limite);

	asm("lidtl (kidtr)");

	init_pic();
	
	sti;

	_clear();
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

void *memcpy(char *dest, char *src, int size){
	char *p = dest;
	while(size--){
		*dest++ = *src++;
	}
	return p;
}

void isr_default_int(){
	//_printk("INTERRUPT");
}

int x = 0;
int y = 0; 

void isr_keyboard_int(){
	unsigned char i;
	static int lshift_enable;
	static int rshift_enable;
	static int alt_enable;
	static int ctrl_enable;

	do {
		i = inb(0x64);
	} while ((i & 0x01) == 0);

	i = inb(0x60);
	i--;

	//// putcar('\n'); dump(&i, 1); putcar(' ');

	if (i < 0x80) {		/* touche enfoncee */
		switch (i) {
		case 0x29:
			lshift_enable = 1;
			break;
		case 0x35:
			rshift_enable = 1;
			break;
		case 0x1C:
			ctrl_enable = 1;
			break;
		case 0x37:
			alt_enable = 1;
			break;
		default:
			_clear();
			if(i == 'L' && x < 75)
				x++;
			else if(i == 'J' && x > 0)
				x--;
			else if(i == 'O' && y < 23)
				y++;
			else if (i == 'G' && y > 0)
				y--;
				
			_clear();
			_gotoxy(x,y);
			_printk("*****"); //putcar(kbdmap [i * 4 + (lshift_enable || rshift_enable)]);
			_gotoxy(x, y+1);
			_printk("*****");
		}
	} else {		/* touche relachee */
		i -= 0x80;
		switch (i) {
		case 0x29:
			lshift_enable = 0;
			break;
		case 0x35:
			rshift_enable = 0;
			break;
		case 0x1C:
			ctrl_enable = 0;
			break;
		case 0x37:
			alt_enable = 0;
			break;
		}
	}

	//show_cursor();
}
