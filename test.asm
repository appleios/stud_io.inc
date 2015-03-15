%include "stud_io.asm.inc"

section .bss
buf	resb 	1024
buflen	equ	$-buf

section .data
msg	db	"Hello World",10,13,0
msglen	equ	$-msg

section .text
global _start
_start:
	print_string msg
	get_string buf, buflen
	print_string buf
	mov eax, buf
	print_string eax
	syscall_exit 0
