
; TRIAL: ..............done
; <<< driver module >>>
; start
;     declare v: integer;
;     get_value(v);
;     print(v);
; end


; main:
;     call _get_user_input
;     mov eax, rsi
;     mov rax, eax
;     call _print_num

;     mov rax, 60
;     mov rdi, 0
;     syscall


; TRIAL: ..............
; <<< driver module >>>
; start
;   declare v1, v2: integer;
    ; v1 := 3
    ; v2 := 4
    ; v1 := v1 + v2;
    ;print(v1)
; end


; total available registers
;      AL/AH, CL/CH, DL/DH, BL/BH, SPL, BPL, SIL, DIL, R8B-R15B     8
;      AX, CX, DX, BX, SP, BP, SI, DI, R8W-R15W                     16
;      EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D-R15D             32
;      RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8-R15               64

; directives
    global _start

; add meta info like file name, etc...

; constants
section .data
    text db "hello", 10,0 ; 10 stands for newline

; unalloc variables (currently for io related stuff)
section .bss
    buffer	resb	120 ; reserve space for ip
    intg	resd	1
    isneg	resb	1

    leftchild resb  1
    rightchild resb 1
    

; code
section .text


    ; declare v1: integer
    _declare:
        push

; io operations
    ; // printing stuff
    _print_num:

    _print_new_line:
    
    _print_str:
        push rax ; points to str to print
        mov rbx, 0 ;counts length for printing string

        .printLoop:
            inc rax ; access next char in str
            inc rbx ; increase count
            mov cl, [rax]  ; store next char in c
            cmp cl, 0   ; check if EOS reached
            jne .printLoop  ; continue with loop if string not over

            ; setup to print to stdout
            mov rax, 1
            mov rdi, 1

            ; location of string stored in rsi
            pop rsi ; points to location of str

            mov rdx, rbx    ; move size (stored in rbx) to rdx
            syscall

            ret

    _atoi:
        push    ebx
        push    ecx
        push    edx
        push    esi
        mov     esi, eax        
        mov     eax, 0          
        mov     ecx, 0          
        
        mov     bl, [esi+ecx]   
        cmp     bl,'-'          
        jne     .multiplyLoop
        inc     ecx

    .multiplyLoop:
        mov     ebx, 10         
        mul     ebx             
        mov     bl, [esi+ecx]   
        cmp     bl, 48          
        jl      .finished       
        cmp     bl, 57          
        jg      .finished       
    
        sub     bl, 48          
        add     eax, ebx        
        inc     ecx             
        jmp     .multiplyLoop  

;constructs
    forloop:
        mov ecx, (loop count)
        iter:
                ; some stuff
            loop iter

    whileloop:
    


    switchcase:


; getting user input
    ; stores it in rsi
    _get_user_input:
        mov rax, 0 ; for ip
        mov rdi, 0 ; for stdip

        mov rsi, buffer
        mov rdx, 120
        syscall

        ret

    ; data types

; arithmetic operations
    _addint:
        push ebp    ; save the prior EBP value
        mov ebp, esp

        mov eax, [ ebp + 8 ]        ; based-indexed addressing
        add eax, [ ebp + 12 ]

        ; postamble
        pop ebp     ; minimal cleanup
        ret

    _addreal:
    ; using the xmm registers

    _subint:
        push ebp    ; save the prior EBP value
        mov ebp, esp

        mov eax, [ ebp + 8 ]        ; based-indexed addressing
        sub eax, [ ebp + 12 ]

        ; postamble
        pop ebp     ; minimal cleanup
        ret

    _subreal:

; logical operations
    _or:
        
    _and:

    _exit:
        ; exit call (return status 0)
        mov rax, 60
        mov rdi, 0
        syscall

;main
    _start:
        ; mov rax, text
        call _get_user_input ;iostmt (GETVAL)

        mov rax, rsi

        call _print_str ; iostmt (PRINT)

        call _exit

    _user_proc:
        .main:


    simple_stmt:
    
    declare:    


    