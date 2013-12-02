[BITS 16]               ; indique a nasm que l'on travaille en 16 bits
[ORG 0x0]               ; Offset à ajouter aux adresses référencées

; initialisation des segments en 0x07C00
mov ax, 0x07C0
mov ds, ax              ; ds et es indiquent le début du segment de données
mov es, ax
mov ax, 0x8000  
mov ss, ax              ; Initialisation du segment de pile
mov sp, 0xf000          ; Initialisation du pointeur de pile 
                        ;       => stack de 0x8F000 -> 0x80000

; affiche un msg
mov si, msgDebut        ; Registre SI : Index de source (Offset de ds)
call afficher           ; Appel de la routine afficher


end:                    ; Boucle infinie
    jmp end


;--- Variables ---
    msgDebut db "Hello world !", 13, 10, 0
;-----------------

;---------------------------------------------------------
; Synopsis: Affiche une chaine de caracteres se terminant par 0x0
; Entree:   DS:SI -> pointe sur la chaine a afficher
;---------------------------------------------------------
afficher:
    push ax
    push bx
.debut:
    lodsb         ; ds:si -> al
    cmp al, 0     ; fin chaine ?
    jz .fin
    mov ah, 0x0E  ; appel au service 0x0e, int 0x10 du bios
    mov bx, 0x07  ; bx -> attribut, al -> caractere ascii
    int 0x10
    jmp .debut

.fin:
    pop bx
    pop ax
    ret

;--- NOP jusqu'a 510 ---
times 510-($-$$) db 144
dw 0xAA55
