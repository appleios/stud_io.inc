;; File stud_io.inc for Linux O.S.
;; Copyright (c) Andrey Vikt. Stolyarov, 2009.
;; I, the author, hereby grant everyone the right to use this
;; file for any purpose, in any manner, in it's original or
;; modified form, provided that any modified versions are
;; clearly marked as such.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; system dependend part

; generic 3-param syscall
%macro _syscall_3 4
	push edx
	push ecx
	push ebx ; it is senseless to save eax as it holds the return
	push %1
	push %2
	push %3
	push %4
	pop edx
	pop ecx
	pop ebx
	pop eax 
	int 0x80
	pop ebx
	pop ecx
	pop edx
%endmacro

; syscall_exit is the only syscall we use that has 1 parameter
%macro syscall_exit 1
	mov ebx, %1	; exit code
	mov eax, 1	; 1 = sys_exit
	int 0x80
%endmacro

;; system dependent part ends here
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; %1: descriptor   %2: buffer addr   %3: buffer length 
; output: eax: read bytes
%macro _syscall_read 3
	_syscall_3 3,%1,%2,%3
%endmacro

; %1: descriptor   %2: buffer addr   %3: buffer length 
; output: eax: written bytes
%macro _syscall_write 3
	_syscall_3 4,%1,%2,%3
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%macro print_string 1-*
	pushad
	pushf
	
%ifstr %1 			; its a string

	jmp %%astr	
%%str 
%rep %0-1
	%rotate 1
	db	%1
%endrep

	%%strln	equ	$-%%str
%%astr:	
	_syscall_write 1, %%str, %%strln
	
%else 		; let's hope that it's a pointer to the string
	xor ecx, ecx
%%loop_for_finding_string_length:
	mov al, [%1 + ecx]
	test al, al
	jz %%count_found
	inc ecx
	jmp %%loop_for_finding_string_length
%%count_found:
	_syscall_write 1, %1, ecx
%endif
	popf
	popad
%endmacro

%macro print_nl 1
	print_string 10,13
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%macro get_string 2
	pushf
	_syscall_read 0, %1, %2
	test eax, eax
	jz	%%eof_reached
	cmp eax, buflen
	je	%%add_zero_at_string_end
	inc eax
%%add_zero_at_string_end:
	mov byte [buf + eax], 0
	jmp	%%gcquit
%%eof_reached:
	xor	eax, eax
	not	eax 		; eax := -1
%%gcquit:
	popf
%endmacro
