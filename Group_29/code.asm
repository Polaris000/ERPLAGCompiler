	struc DARRAY
	.ptr: resd 1
	.count: resd 1
	endstruc
section .data
	printmessage: db " The number is", 0
 	scanmessage: db "input: Enter the number ", 0
	integerMsg: db "Input: Enter an integer value ", 0
	booleanMsg: db "Input: Enter a boolean value ", 0
	intMsg: db "integer", 0
	trueMsg: db "true", 0
	falseMsg: db "false", 0
	boolMsg: db "boolean", 0
	output: db "Output:", 0
	percentDin: db "%d", 0
	percentDout: db "%d", 10, 0
	percentDoutarr: db "%d ", 0
	percentS: db "%s", 0
	percentSout: db "%s", 10, 0
	OutofBoundError: db "RUN TIME ERROR in Line %d:  Array index out of bound", 10, 0
	BoundMismatchError: db "RUN TIME ERROR in Line %d:  Bounds do not match", 10, 0
	arrayInputString: db "Input: Enter %d array elements of %s type for range %d to %d", 0
section .bss
	 A_7_11 	 resd 	 6
section .text
	global  main
	extern scanf, printf, malloc, realloc, free
_getarrlength:
	pop ebp
	pop ebx
	pop eax
	sub ebx, eax
	inc ebx
	push ebx
	push ebp
	ret
main:
	mov ebp, esp
	sub esp, 8
	mov eax, 10
	push eax
	mov edi, 15
	push edi
	call _getarrlength
	pop esi
	push edi
	push eax
	push intMsg
	push esi
	push arrayInputString
	call printf
	mov edi, 0

scan_0:
	mov ebx, A_7_11
	mov eax, 4
	mul edi
	add ebx, eax
	push 	ebx
	push 	percentDin
	call 	scanf
	add esp, 8
	inc edi
	cmp edi, esi
	jnz scan_0
	mov esi, 10
	push esi
	mov esi, 15
	push esi
	call _getarrlength
	pop esi
	mov edi, 0

print_1:
	push 	dword [A_7_11 + 4 * edi]
	push 	dword percentDoutarr
	call 	printf
	add esp, 8
	inc edi
	cmp edi, esi
	jnz print_1

EXIT:
 	add esp, 8
	mov ebx, 0
	mov eax, 1
	int 80h 
