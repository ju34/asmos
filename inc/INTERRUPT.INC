; VARIABLES ------------------------------------------------
;
;-----------------------------------------------------------
;Déclaration en C :
;-----------------------------------------------------------
;struct idtdesc{
;	unsigned short offset_0_15;
;	unsigned short select;
;	unsigned short type;
;	unsigned short offset_16_31;
;} __attribute__ ((packed));
;-----------------------------------------------------------
;Déclaration en assembleur :
;-----------------------------------------------------------
;idtdesc:
;	dw 0	; Offset [0:15]
;	dw 0	; Select
;	dw 0	; type
;	dw 0	; Offset [16:31]
;-----------------------------------------------------------

GLOBAL _init_idt_desc, _asm_default_int, _asm_keyboard_int
EXTERN isr_default_int, isr_keyboard_int

;-----------------------------------------------------------
; Name   : _init_idt_desc(struct idtdesc *desc, unsigned int offset, unsigned short select, unsigned short type)
; Desc.  :
;-----------------------------------------------------------
_init_idt_desc:
	push ebp
	push ebx
	push eax

	mov ebp, esp

	mov eax, [ebp+16]
	mov ebx, [ebp+20]
	mov word [eax], bx
	shr ebx, 16
	mov word [eax+6], bx

	mov ebx, [ebp+24]
	mov word [eax+2], bx
	
	mov ebx, [ebp+28]
	mov word [eax+4], bx	

	pop eax	
	pop ebx
	pop ebp

	ret

_asm_default_int:
	call isr_default_int
	mov al, 0x20
	out 0x20, al
	iret

_asm_keyboard_int:
	call isr_keyboard_int
	mov al, 0x20
	out 0x20, al
	iret
