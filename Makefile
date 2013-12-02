all: clean run

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
	-rm bochs
