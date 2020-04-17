; %include 'functions.asm'

	global 	 main
    extern printf

section .data
 	inputMsg 	    db	 'Input: Enter an ', 0h
	intMsg 	        db 	 'integer ', 10h
	realMsg 	    db 	 'real ', 0h
	boolMsg 	    db 	 'boolean ', 0h
	valMsg 	        db 	 'value ', 0h
	arrMsg 	        db 	 'array elements of ', 0h
	outMsg 	        db 	 'Output: ', 0h
	trueMsg 	    db 	 'true', 0h
	falseMsg 	    db 	 'false', 0h

    intformat       db    "%d", 10
    strformat       db    "%s", 10
    doubleformat    db    "%f", 10

section .bss
	sinput 	 resb 	 255

section .text

main:
	mov 	 ebp, esp
	mov 	 eax, 3
	mov 	 [ebp+0], eax
	mov 	 eax, 5
	mov 	 [ebp+4], eax
	mov 	 eax, [ebp+0]
	mov 	 [ebp + 12], eax
	mov 	 eax, [ebp+4]
	add 	 eax, [ebp + 12]
	mov 	 [ebp+8], eax
    ; mov      eax, 0
    ; push     eax
    push intMsg
    push strformat
    call    printf
    add esp,8
 	mov 	 eax, 1 
	mov 	 ebx, 0
    int 	 80h 
