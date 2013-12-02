;-----------------------------------------------------------
; Synopsis: Affiche une chaine de caracteres se terminant
;	    par 0x0
; Entree:   DS:SI -> Pointe sur la chaine a afficher
;-----------------------------------------------------------
print:
	push ax
	push bx

.start:
	lodsb		; ds:si -> al
	cmp al, 0	; Fin de chaine ?
	jz .end
	mov ah, 0x0e	; appel au serive 0x0e, int 0x10 du bios
	mov bx, 0x07	; bx -> attribut, al -> caractere ascii
	int 0x10
	jmp .start

.end:
	pop bx
	pop ax
	ret
