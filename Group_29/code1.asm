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
	mov 	 [ebp+56], eax
	mov 	 eax, 23.559999
	mov 	 [ebp+62], eax
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
	mov 	 [ebp+52], sinput
	mov 	 eax, 0.058900
	mov 	 [ebp+66], eax
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
	mov 	 [ebp+70], sinput
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
	mov 	 [ebp+78], sinput
	mov 	 eax, 2367.000000
	mov 	 [ebp+82], eax
	mov 	 eax, [ebp+62]
	mov 	 [ebp + 90], eax
	mov 	 eax, [ebp+78]
	mov 	 [ebp + 94], eax
	mov 	 eax, [ebp+82]
	mul 	[ebp+94]
	mov 	 [ebp + 98], eax
	mov 	 eax, [ebp+66]
	mov 	 [ebp + 102], eax
	mov 	 eax, [ebp+74]
	mul 	[ebp+102]
	sub 	 eax, [ebp + 98]
	add 	 eax, [ebp + 90]
	mov 	 [ebp+74], eax
	mov 	 [ebp+61], eax
	mov 	 [ebp+60], eax
	 mov 	 eax, 23
	mov 	 [ebp + 106], eax
	mov 	 eax, [ebp+0]
	add 	 eax, [ebp + 106]
	mov 	 [ebp+86], eax
	mov 	 eax, [ebp+66]
	mov 	 [ebp + 110], eax
	 mov 	 eax, 25
	mul 	[ebp+110]
	mov 	 [ebp + 114], eax
	 mov 	 eax, 100
	mov 	 [ebp + 118], eax
	mov 	 eax, [ebp+0]
	mul 	[ebp+118]
	add 	 eax, [ebp + 114]
	mov 	 [ebp+70], eax
	mov 	 eax, [ebp+74]
	mov 	 [ebp + 122], eax
	mov 	 eax, [ebp+78]
	mov 	 [ebp + 126], eax
	mov 	 eax, [ebp+82]
	add 	 eax, [ebp + 126]
	sub 	 eax, [ebp + 122]
	mov 	 [ebp+0], eax
	mov 	 eax, [ebp+86]
	mov 	 [ebp + 130], eax
	 mov 	 eax, 5
	add 	 eax, [ebp + 130]
	mov 	 [ebp+86], eax
	mov 	 eax, [ebp+0]
	mov 	 [ebp+0], eax
	mov 	 eax, [ebp+0]
	mov 	 [ebp+0], eax
 	mov 	 eax, 1
	int 	 80h 
	mov 	 ebx, 0
