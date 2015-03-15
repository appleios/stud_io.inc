%include "stud_io.inc"

section .data
msg	db	"Hello World",0
msglen	equ	$-msg

section .text
global _start
_start:
	syscall_exit
