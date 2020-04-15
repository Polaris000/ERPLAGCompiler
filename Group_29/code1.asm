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
	 mov 	 eax, 12
	mov 	 [ebp+84], eax
	mov 	 eax, 23.559999
	mov 	 [ebp+91], eax
	mov 	 eax, inputMsg
	 call 	 sprint
	mov 	 eax, intMsg
	 call 	 sprint
	mov 	 eax, valMsg
	 call 	 sprintLF
	 mov 	 edx, 255 	 ;number of bytes to read
	 mov 	 ecx, sinput 	 ;buffer
	 mov 	 ebx, 0 	 ;write to the STDIN file
	 mov 	 eax, 3 	 ;invoke SYS_READ
	 int 	 80h
	mov 	 [ebp+80], sinput
	mov 	 eax, 0.058900
	mov 	 [ebp+95], eax
	mov 	 eax, inputMsg
	 call 	 sprint
	mov 	 eax, intMsg
	 call 	 sprint
	mov 	 eax, valMsg
	 call 	 sprintLF
	 mov 	 edx, 255 	 ;number of bytes to read
	 mov 	 ecx, sinput 	 ;buffer
	 mov 	 ebx, 0 	 ;write to the STDIN file
	 mov 	 eax, 3 	 ;invoke SYS_READ
	 int 	 80h
	mov 	 [ebp+99], sinput
	mov 	 eax, inputMsg
	 call 	 sprint
	mov 	 eax, realMsg
	 call 	 sprint
	mov 	 eax, valMsg
	 call 	 sprintLF
	 mov 	 edx, 255 	 ;number of bytes to read
	 mov 	 ecx, sinput 	 ;buffer
	 mov 	 ebx, 0 	 ;write to the STDIN file
	 mov 	 eax, 3 	 ;invoke SYS_READ
	 int 	 80h
	mov 	 [ebp+107], sinput
	mov 	 eax, 2367.000000
	mov 	 [ebp+111], eax
	mov 	 eax, [ebp+91]
	mov 	 [ebp + 132], eax
	mov 	 eax, [ebp+107]
	mov 	 [ebp + 136], eax
	mov 	 eax, [ebp+111]
	mul 	[ebp+136]
	mov 	 [ebp + 140], eax
	mov 	 eax, [ebp+95]
	mov 	 [ebp + 144], eax
	mov 	 eax, [ebp+103]
	mul 	[ebp+144]
	sub 	 eax, [ebp + 140]
	add 	 eax, [ebp + 132]
	mov 	 [ebp+103], eax
	mov 	 [ebp+90], eax
	mov 	 [ebp+88], eax
	 mov 	 eax, 23
	mov 	 [ebp + 148], eax
	mov 	 eax, [ebp+0]
	add 	 eax, [ebp + 148]
	mov 	 [ebp+115], eax
	mov 	 eax, [ebp+95]
	mov 	 [ebp + 152], eax
	 mov 	 eax, 25
	mul 	[ebp+152]
	mov 	 [ebp + 156], eax
	 mov 	 eax, 100
	mov 	 [ebp + 160], eax
	mov 	 eax, [ebp+0]
	mul 	[ebp+160]
	add 	 eax, [ebp + 156]
	mov 	 [ebp+99], eax
	mov 	 eax, [ebp+103]
	mov 	 [ebp + 164], eax
	mov 	 eax, [ebp+107]
	mov 	 [ebp + 168], eax
	mov 	 eax, [ebp+111]
	add 	 eax, [ebp + 168]
	sub 	 eax, [ebp + 164]
	mov 	 [ebp+0], eax
	mov 	 eax, [ebp+115]
	mov 	 [ebp + 172], eax
	 mov 	 eax, 5
	add 	 eax, [ebp + 172]
	mov 	 [ebp+115], eax
	mov 	 eax, [ebp+0]
	mov 	 [ebp+0], eax
	mov 	 eax, [ebp+0]
	mov 	 [ebp+0], eax
 	mov 	 eax, 1
	int 	 80h 
	mov 	 ebx, 0
