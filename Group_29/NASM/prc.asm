
section .text
global main
extern printf

main:
        mov rdi, IPformat
        mov rsi, rsp
        xor rax, rax
        call printf

        push rax

        mov rdi, IPformat
        mov rsi, rsp
        xor rax, rax
        call printf

        pop rax

        ret

IPformat:
        db      "The current IP = %ld", 10, 0