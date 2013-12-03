[BITS 32]               ; indique a nasm que l'on travaille en 16 bits
[ORG 0x1000]            ; Offset à ajouter aux adresses référencées

jmp start

%include "inc/STDIO.INC"

start:
	call _clear

	mov eax, 5
	push eax
	push eax
	call _gotoxy
	pop eax
	pop eax

	mov eax, msg
	push eax
	call _printk
	pop eax

	mov ax, 0x04
	push ax
	call _setattr
	pop ax

	mov eax, msg
	push eax
	call _printk
	pop eax

;Boucle infinie
end:
	jmp end

;----------------------------------
msg: db "Message de test suffisament long pour depasser les 80 caracteres et tester si on arrive a retourner a la ligne. Ca serait super ! :)", 0
;----------------------------------
