all: bootsect boot.iso run

bootsect: boot.asm
	nasm -f bin -o bootsect boot.asm

boot.iso: bootsect
	cat bootsect /dev/zero | dd of=boot.iso bs=512 count=2880

run: boot.iso
	echo 6 | bochs 'boot:a' 'floppya: 1_44=boot.iso, status=inserted'

clean:
	-rm *~
	-rm *.iso
	-rm bootsect	
	-rm bochs
