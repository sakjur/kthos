section data
    string: db  "Hello, World!", 0xA, 0

section text

global _start
_start:
    mov rax, 1 ; write
    mov rdi, 1 ; stdout
    mov rsi, string
    mov rdx, 14 ; length of string
    syscall
    mov rax, 60 ; exit
    mov rdi, 0 ; exitcode
    syscall
    ret

