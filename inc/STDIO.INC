%define 	SCREEN_BASE	0xB8000

global _printk, _putcar, _setattr, _clear, _gotoxy

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
	push ebx
	call _putcar
	pop ebx
	jmp .start

.end:
	pop ebp
	ret

;-----------------------------------------------------------
; Name   : _putcar(char c)
; Desc.  : 
;-----------------------------------------------------------
_putcar:
	push ebp
	mov ebp, esp

	mov bl, byte [ebp+8]

	push eax
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
	pop eax
	pop ebp
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
; Name   : _setattr(u8 attr)
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
; Name   : _clear()
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
        push dword 0
        push dword 0
        call _gotoxy
        add esp, 8

	pop eax
	ret

;----------------------------------------------------------
; Name   : _gotoxy(int x, int y)
; Desc.  : Go to X, Y position on screen
;----------------------------------------------------------
_gotoxy:
	push ebp
	mov ebp, esp

	push eax

	mov eax, [ebp+8]
	mov [screen], eax

	mov eax, [ebp+12]
	mov [screen+4], eax

	pop eax
	pop ebp

	ret
