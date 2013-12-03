all: run

c: c/kernel.c inc/STDIO.INC bin/bootsect
	nasm -f elf inc/STDIO.INC
	mv inc/STDIO.o obj/stdio.o
	nasm -f elf inc/INTERRUPT.INC
	mv inc/INTERRUPT.o obj/interrupt.o
	gcc -c c/kernel.c -o obj/kernel.o
	ld --oformat binary -Ttext 1000 obj/kernel.o obj/stdio.o obj/interrupt.o -o bin/kernel_c
	cat bin/bootsect bin/kernel_c /dev/zero | dd of=img/boot_c.iso bs=512 count=2880
	echo 6 | bochs 'boot:a' 'floppya: 1_44=img/boot_c.iso, status=inserted'

bin/bootsect: src/boot.asm
	nasm -f bin -o bin/bootsect src/boot.asm

bin/kernel: src/kernel.asm
	nasm -f bin -o bin/kernel src/kernel.asm

img/boot.iso: bin/bootsect bin/kernel
	cat bin/bootsect bin/kernel /dev/zero | dd of=img/boot.iso bs=512 count=2880

run: img/boot.iso
	echo 6 | bochs 'boot:a' 'floppya: 1_44=img/boot.iso, status=inserted'

clean:
	-find . -name *~ -exec rm {} \;
	-rm img/boot.iso
	-rm bin/bootsect
	-rm bin/kernel	
	-rm bin/kernel_c
	-rm img/boot_c.iso
	-rm obj/*.o
	-rm bochs
