%define		BASE	0x100	; 0x0100:0x0 = 0x1000 
%define 	KSIZE	50	; nombre de secteurs de 512o a charger

[BITS 16]               ; indique a nasm que l'on travaille en 16 bits
[ORG 0x0]               ; Offset à ajouter aux adresses référencées

jmp start

%include "inc/UTIL.INC"

start:
	; initialisation des segments en 0x07C00
	mov ax, 0x07C0
	mov ds, ax              ; ds et es indiquent le début du segment de données
	mov es, ax
	mov ax, 0x8000  
	mov ss, ax              ; Initialisation du segment de pile
	mov sp, 0xf000          ; Initialisation du pointeur de pile 
	                        ;       => stack de 0x8F000 -> 0x80000

	mov [bootdrv], dl

	; affiche un msg
	mov si, startMsg        ; Registre SI : Index de source (Offset de ds)
	call print	        ; Appel de la routine print

	;Charger le noyau
	xor ax, ax
	int 0x13		; Charge en memoire en faisant appel au bios

	push es
	mov ax, BASE
	mov es, ax
	mov bx, 0
	mov ah, 2
	mov al, KSIZE
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, [bootdrv]
	int 0x13

	pop es

	mov si, msg00
	call print

	; Initialisation du pointeur sur la GDT
	mov ax, gdtend	;Calcul de la limite de GDT
	mov bx, gdt
	sub ax, bx
	mov word [gdtptr], ax

	xor eax, eax	;Calcul de l'adresse linéaire de GDT
	xor ebx, ebx
	mov ax, ds
	mov ecx, eax
	shl ecx, 4
	mov bx, gdt
	add ecx, ebx
	mov dword [gdtptr+2], ecx

	mov si, msg01
	call print

	;Passage en mode protege
	cli
	lgdt [gdtptr]
	mov eax, cr0
	or ax, 1
	mov cr0, eax

	jmp next

next:
	mov ax, 0x10	; segment de donne
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x9F000

	jmp dword 0x8:0x1000 ; reinitialise le segment de code

;--- Variables ---
    startMsg: db "AsmOs Start ...", 13, 10, 0
    msg00:    db "Chargement en memoire OK ...", 13, 10, 0
    msg01:    db "Initialisation GDT OK ...", 13, 10, 0
    bootdrv:  db 0
;-----------------
gdt:
    db 0, 0, 0, 0, 0, 0, 0, 0
gdt_cs:
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
gdt_ds:
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
gdtend:
;----------------------------------------------------------
gdtptr:
	dw 0	;limite
	dd 0	;base
;----------------------------------------------------------

;; NOP jusqu'à 510
times 510-($-$$) db 144
dw 0xAA55
