section .data
    msg db 4

section .text
global  _start

_start:

    ; push ebp
    mov      ebp, esp
    sub      esp, 16

    ; move 3 to -4
    mov      eax, 2
    mov      dword [ebp - 4], eax

    ; move 5 to - 8
    mov      eax, 3
    mov      dword [ebp - 8], eax

    mov      eax, [ebp - 8] ;5
    mov      ebx, [ebp - 4] ; 3
    add      eax, ebx ;8

    ; print value stored in eax
    add      eax, 30h      
    mov     [msg], eax
    mov     ecx, msg    
    mov     edx, 4
    mov     eax, 4
    mov     ebx, 1
    int     80h

    ; exit
    mov eax,1           
    mov ebx,0           
    int 80h