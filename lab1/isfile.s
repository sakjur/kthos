    .data
path:
.asciz "/etc/passwd"
    .bss
sb:
.skip 64
    .text
.global main

main:
    mov $106, %eax
    mov $path, %ebx
    mov $sb, %ecx
    int $0x80
    mov %eax, %ebx
    mov $1, %eax
    int $0x80

