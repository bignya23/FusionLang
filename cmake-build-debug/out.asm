global _start
_start:
    mov rax, 10
    push rax
    mov rax, 2
    push rax
    push QWORD [rsp + 8]
    mov rax, 60
    pop rdi
    syscall
    mov rax , 60
    mov rdi ,0
    syscall
