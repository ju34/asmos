%define STRING_INC

GLOBAL _memcpy

_memcpy:
    push ebp
    mov ebp, esp

    push esi
    push edi

    mov eax, [ebp+8]
    mov edi, eax
    mov esi, [ebp+12]
    mov ecx, [ebp+16]

    rep
    movsb

    pop edi
    pop esi
    pop ebp

    ret
