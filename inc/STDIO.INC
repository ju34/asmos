%define 	SCREEN_BASE	0xB8000

global _printk, _putcar, _SOL, _GNL, _setattr, _clear

; VARIABLES ------------------------------------------------
screen:
	dd 0 ;X
	dd 0 ;Y
	db 0x0F ;ATTR
;-----------------------------------------------------------


;-----------------------------------------------------------
; Name   :
; Desc.  :
;-----------------------------------------------------------
_printk:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
.start:
	mov bl, byte [eax]
	inc eax

	cmp bl, 0	; Fin de chaine ?
	jz .end
	;mov bl, al
	call _putcar
	jmp .start

.end:
	pop ebp
	ret

;-----------------------------------------------------------
; Name   :
; Desc.  : 
;-----------------------------------------------------------
_putcar:
	push eax
	push edx

	mov eax, [screen+4]
	mov edx, 0xA0
	mul edx
	
	add eax, SCREEN_BASE
	mov edx, [screen]
	shl edx, 1
	add eax, edx
	
	mov byte [eax], bl;
	inc eax
	mov dl, [screen+8]
	mov byte [eax], dl

	mov eax, [screen]
	inc eax
	cmp eax, 80
	jnb .goToLine
	mov [screen], eax
	jmp .end

.goToLine:
	call _SOL
	call _GNL

.end:
	pop edx
	pop eax
	ret

;-----------------------------------------------------------
; Name   : SOL()
; Desc.  : Goto start of line
;-----------------------------------------------------------
_SOL:
	push eax
	mov eax, 0
	mov [screen], eax
	pop eax
	ret

;-----------------------------------------------------------
; Name   : GNL()
; Desc.  : Goto next line
;-----------------------------------------------------------
_GNL:
	push eax
	mov eax, [screen+4]
	inc eax
	mov [screen+4], eax
	pop eax
	ret

;-----------------------------------------------------------
; Name   : setattr(u8 attr)
; Desc.  : Set display attribute
;
; Param. : attr pushed in stack befor call function
;-----------------------------------------------------------
_setattr:
	push ebp
	mov ebp, esp
	
	mov al, byte [ebp+8]
	mov byte [screen+8], al

	pop ebp
	ret

;-----------------------------------------------------------
; Name   : clear()
; Desc.  : Clear screen
;-----------------------------------------------------------
_clear:
	push eax

	mov eax, SCREEN_BASE

.start:
	cmp eax, 0xB8FA0
	jnb .end	
	
	mov byte [eax], 0x00
	inc eax
	jmp .start

.end:
	pop eax
	ret
