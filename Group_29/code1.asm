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
	sinput 	 resb 	 255
section .text
	global 	 driver
driver:
	mov 	 ebp, esp
	 mov 	 eax, 0
	mov 	 [ebp+12], eax
	 mov 	 eax, 3
	mov 	 [ebp+16], eax
	mov 	 eax, [ebp+12]
	mov 	 [ebp + 20], eax
	 mov 	 eax, 1
	add 	 eax, [ebp + 20]
	mov 	 [ebp+12], eax
 	mov 	 eax, 1
	int 	 80h 
	mov 	 ebx, 0
