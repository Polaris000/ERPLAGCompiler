; Compiling this code for 32-bit use:
;    nasm -f elf file.asm
;    gcc -m32 -o file file.o
;
;~.~. Definitions for readability: ~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.
        %define  SYS_EXIT   1
        %define  SYS_READ   3
        %define  SYS_WRITE  4
        %define  STDIN      0
        %define  STDOUT     1
        %define  STDERR     2
        %define  MAX_NUMBER   5000
SECTION .data
format: db "The number is %d." , 10
SECTION .bss
input: resd MAX_NUMBER
SECTION .text
extern printf
extern atoi ; <~~ converts string to int32.
global _start
_start:
nop
GetInput:
        mov EAX, SYS_READ
        mov EBX, STDIN
        mov ECX, input
        mov EDX, MAX_NUMBER ; <~~ You must specify the length of the buffer.
        int 80H
        mov DWORD[input + EAX - 1], 0
Calculate:
	;; <~~ atoi is a C function used in converting strings to integers.
	push dword input
	call atoi
	add ESP, 4
Display:
        push EAX
        push format
        call printf
        add ESP, 8
        ret