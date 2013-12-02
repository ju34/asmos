%define		BASE	0x100	; 0x0100:0x0 = 0x1000 

[BITS 16]               ; indique a nasm que l'on travaille en 16 bits
[ORG 0x0]               ; Offset à ajouter aux adresses référencées

jmp start

%include "inc/UTIL.INC"

start:
	; initialisation des segments en 0x07C00
	mov ax, BASE
	mov ds, ax              ; ds et es indiquent le début du segment de données
	mov es, ax
	
	; Initialisation du segment de pile
	mov ax, 0x8000  
	mov ss, ax              ; Initialisation du segment de pile
	mov sp, 0xf000          ; Initialisation du pointeur de pile 
	                        ;       => stack de 0x8F000 -> 0x80000

	; affiche un msg
	mov si, msg00	        ; Registre SI : Index de source (Offset de ds)
	call print	        ; Appel de la routine print

;Boucle infinie
end:
	jmp end
	

;--- Variables ---
    msg00 db "Chargement du kernel ...", 13, 10, 0
;-----------------
