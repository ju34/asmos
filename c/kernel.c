extern void _setattr(unsigned char attr);
extern void _clear();
extern void _printk(unsigned char *str);

void _start(void)
{
	//setattr(0x0F);
	_clear();
	_printk("TEST");
	_setattr(0x04);
	_printk("Encore un test");
	while(1);
}
