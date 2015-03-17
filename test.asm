%include "/home/student/stud_io.inc"

%macro pcall 1-*
%rep %0-1
	%rotate -1
	push dword %1
%endrep
	%rotate -1
	call %1
	add esp, (%0-1)*4
%endmacro

%define arg(N)	[ebp + (4*N) + 4]

section .bss
buf	resb	1024

section .data
msg	db	"Hello World",10,13,0
msglen	equ	$-msg

section .text
global _start
_start:
	pcall print_binary, 135
	print_nl
	syscall_exit 0

print_dec:
	enter 0,0
	push eax
	push edx
	
	pop edx
	pop eax
	leave
	ret

print_binary:
	enter 0,0
	push eax
	mov eax, arg(1)
.lp:	shl	eax,1
	jc	.print_one
	print_string "0"
	jmp .lp_end
.print_one:
	print_string "1"
.lp_end:
	test eax, eax
	jnz .lp
	pop eax
	leave
	ret
