%define		BASE	0x100	; 0x0100:0x0 = 0x1000 
%define 	KSIZE	1	; nombre de secteurs de 512o a charger
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

	;Saut vers le kernel..
	jmp dword BASE:0
	

;--- Variables ---
    startMsg db "AsmOs Start ...", 13, 10, 0
    bootdrv: db 0
;-----------------

;; NOP jusqu'à 510
times 510-($-$$) db 144
dw 0xAA55
