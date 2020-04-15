%include 'functions.asm'
section .data
 	inputMsg 	 db	 'Input: Enter an ', 0h
	intMsg 	 db 	 'integer ', 0h
	realMsg 	 db 	 'real ', 0h
	boolMsg 	 db 	 'boolean ', 0h
	valMsg 	 db 	 'value ', 0h
	arrMsg 	 db 	 'array elements of ', 0h
	outMsg 	 db 	 'Output: ', 0h
	trueMsg 	 db 	 'true', 0h
	falseMsg 	 db 	 'false', 0h
section .bss
	sinput: 	 resb 	 255
section .text
	global 	 driver
driver:
mov 	 ebp, esp
	 mov 	 eax, 3
	mov 	 [ebp+0], eax
	 mov 	 eax, 5
	mov 	 [ebp+2], eax
	mov 	 eax, [ebp+0]
	mov 	 [ebp + 6], eax
	mov 	 eax, [ebp+2]
	add 	 eax, [ebp + 6]
	mov 	 [ebp+4], eax
	mov 	 eax, 1
int 	 80h	mov 	 ebx, 0
