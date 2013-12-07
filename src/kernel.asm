[BITS 32]               ; indique a nasm que l'on travaille en 16 bits
[ORG 0x1000]            ; Offset à ajouter aux adresses référencées

%define MULTIBOOT_HEADER_MAGIC  0x1BADB002
%define MULTIBOOT_HEADER_FLAGS  0x00000003
%define CHECKSUM - (MULTIBOOT_HEADER_MAGIC +MULTIBOOT_HEADER_FLAGS)

_start:
    jmp start

align 4
multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd CHECKSUM

%include "inc/STDIO.INC"
%include "inc/GDT.INC"

start:
    
    call _init_kernel_gdt
    mov word ax, 0x18
    mov word ss, ax
    mov dword esp, 0x20000
    call _main

    cli     ; Clear interrupts
    hlt     ; Halt the CPU

_main:
    call _clear

    push dword 0x00
    push dword 0x00
    call _gotoxy
    add esp, 8

    push dword msg00
    call _printk
    add esp, 4

    push dword 0x04
    call _setattr
    add esp, 4

    push dword 0x01
    push dword 0x00
    call _gotoxy
    add esp, 8

    push dword msg01
    call _printk
    add esp, 4

;Boucle infinie
end:
	jmp end

;----------------------------------
msg00: db "Kernel is running ...", 0
msg01: db "Hello world ! ", 0
;----------------------------------
