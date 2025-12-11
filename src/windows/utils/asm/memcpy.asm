global asm_call

section .data

section .text

asm_call:
    mov rax, rcx
    add rax, rdx
    ret