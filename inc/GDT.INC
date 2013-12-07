%define GDTBASE     0x0
%define GDTSIZE     0xFF
%define GDTDESCSIZE 8

; VARIABLES ------------------------------------------------
;
;-----------------------------------------------------------
;Déclaration en C :
;-----------------------------------------------------------
;struct gdtdesc{
;	unsigned short limite_0_15;
;	unsigned short base_0_15;
;	unsigned char  base_16_23;
;       unsigned char  access;
;       unsigned char  limit_16_9:4;
;       unsigned char  other:4;
;	unsigned char  base_24_31;
;} __attribute__ ((packed));
;-----------------------------------------------------------
;Déclaration en assembleur :
;-----------------------------------------------------------
;struc gdtdesc
;.lim0_15    resw 0      ; Limite [0:15]
;.base0_15   resw 0	; Base [0:15]
;.base16_23  resb 0	; Base [16:23]
;.acces      resb 0	; Access
;.lim16_9    resb 0    ; Limit [16:9] - Other
;.base24_31  resb 0    ; Base [24:31]
;endstruc

gdtptr:
    dw 0    ; Limite
    dd 0    ; Base

;-----------------------------------------------------------
;COMMON kgdt   2040:32 ;resb    GDTSIZE*gdtdesc_size
kgdt: resb GDTSIZE*8

GLOBAL _init_gdt_desc, _init_kernel_gdt
EXTERN _memcpy, _putcar
;-----------------------------------------------------------
; Name   : _init_gdt_desc(unsigned int base, unsigned int limit, unsigned char access, unsigned char other, struct gdtdesc *desc);
; Desc.  :
;-----------------------------------------------------------
_init_gdt_desc:
    push ebp
    push ebx
    push eax

    mov ebp, esp

    mov eax, [ebp+32]

;    mov byte [eax], '0'      ;Limite[0:15]      => OK
;    mov byte [eax+2], '1'    ;Base[0:15]
;    mov byte [eax+4], '2'    ;Base[16:23]
;    mov byte [eax+5], '3'    ;Acces             => OK
;    mov byte [eax+6], '4'    ;Limite - Other    => OK   -   NOK
;    mov byte [eax+7], '5'    ;Base [24:31]

;    mov ebx, [ebp+20]       ; argument = Base (ebp+16), limit (ebp+20), access (ebp+24), other (ebp+28), *desc (ebp+32)

    mov ebx, [ebp+16]       ; BASE OK
    mov word [eax+2], bx
    mov ebx, [ebp+18]
    mov byte [eax+4], bl
    mov ebx, [ebp+19]
    mov byte [eax+7], bl

    mov ebx, [ebp+20]       ; OK
    mov [eax], bx
    mov ebx, [ebp+22]
    and ebx, 0x0f
    mov byte [eax+6], bl

    mov ebx, [ebp+24]       ; ACCES OK
    mov byte [eax+5], bl

    mov ebx, [ebp+28]       ; OTHER OK
    shl ebx, 4
    and ebx, 0xf0
    or [eax+6], bl

    pop eax
    pop ebx
    pop ebp

    ret

_init_kernel_gdt:

    enter 0,0

    push eax

    push dword kgdt
    push dword 0x00
    push dword 0x00
    push dword 0x00
    push dword 0x00

    call _init_gdt_desc
;   add esp, 20
    pop eax
    pop eax
    pop eax
    pop eax
    pop eax

    ;Init code segment
    
    push dword (kgdt+GDTDESCSIZE)
    push dword 0x0D
    push dword 0x9B
    push dword 0xFFFFF
    push dword 0x00

    call _init_gdt_desc
;   add esp, 20
    pop eax
    pop eax
    pop eax
    pop eax
    pop eax

    push dword kgdt+(GDTDESCSIZE*2)
    push dword 0x0D
    push dword 0x93
    push dword 0xFFFFF
    push dword 0x00

    call _init_gdt_desc
;   add esp, 20
    pop eax
    pop eax
    pop eax
    pop eax
    pop eax

    push dword kgdt+(GDTDESCSIZE*3)
    push dword 0x0D
    push dword 0x97
    push dword 0x00
    push dword 0x00

    call _init_gdt_desc
;   add esp, 20
    pop eax
    pop eax
    pop eax
    pop eax
    pop eax

       
    mov word [gdtptr], (GDTSIZE * 8)
    mov dword [gdtptr+2], GDTBASE

    push dword (GDTSIZE *8)
    push dword kgdt
    push dword GDTBASE

    call _memcpy
;    add esp, 12
    pop eax
    pop eax
    pop eax

    
    lgdt [gdtptr]

    mov word ax, 0x10
    mov word ds, ax
    mov word es, ax
    mov word fs, ax
    mov word gs, ax

    jmp next
;    jmp dword 0x8:next

    next:

    pop eax
    leave
    ret
