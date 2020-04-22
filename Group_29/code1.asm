 extern    printf  

 SECTION .data              ; Data section, initialized variables

array: dd 1, 2, 3, 4, 5, 6, 7, 8, 9, 0; this is a test array for testing purposes

arrayLen: dd 9  ; length of array 

aoutput: db "%d ", 0 ; output format

SECTION .text               ; Code section.

global main             ; the standard gcc entry point

main:                   ; the program label for the entry point
    push    ebp         ; set up stack frame
    mov     ebp,esp

    mov ecx, [arrayLen] ; loop counter set up
    mov esi, 0      ; counter to increment set up for looping through array
.loop:

    push ecx                                    ; make sure to put ecx (counter) on stack so we don't lose it when calling printf)
    push dword [array + esi]                    ; put the value of the array at this (esi) index on the stack to be used by printf
    push dword aoutput                          ; put the array output format on the stack for printf to use
    call printf                                 ; call the printf command
    add esp, 8                                  ; add 4 bytes * 2
    pop ecx                                     ; get ecx back

    add esi, 4
    loop .loop

    mov     esp, ebp    ; takedown stack frame
    pop     ebp         ; same as "leave" op

    mov     eax,0       ; normal, no error, return value
    ret                 ; return