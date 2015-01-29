section .data
    integer_format: db "%i", 0xA, 0

section .bss
    sb: resb 256

section .text

extern printf
global _start
_start:
    mov rax, 4 ; stat
    pop rdi
    pop rdi
    pop rdi
    mov rsi, sb
    syscall
    push rax ; saving the return value from stat
    mov rdi, integer_format
    mov rsi, rax
    mov rax, 0
    call printf
    pop rdi
    mov rax, 60 ; exit
    syscall
    ret

