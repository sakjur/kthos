section .data
    minus_two: db "254", 0xA, 0
    nil: db "0", 0xA, 0

section .bss
    sb: resb 1024

section .text

global _start
_start:
    mov rax, 4 ; stat
    pop rdi
    pop rdi
    pop rdi
    mov rsi, sb
    syscall
    mov r12, rax ; saving the return value from stat
    mov rax, 1
    mov rcx, r12
    mov rdi, 1
    jrcxz zero
    mov rsi, minus_two
    mov rdx, 5
    jmp write
zero:
    mov rsi, nil
    mov rdx, 3
write:
    syscall
    mov rdi, r12
    mov rax, 60 ; exit
    syscall
    ret

