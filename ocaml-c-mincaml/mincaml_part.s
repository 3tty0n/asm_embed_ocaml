# add caml_startup(argv) in the main function of stub.c
.data
.balign	8
.text
.globl	min_caml_start
min_caml_start:
.globl	_min_caml_start
_min_caml_start: # for cygwin
	pushl	%eax
	pushl	%ebx
	pushl	%ecx
	pushl	%edx
	pushl	%esi
	pushl	%edi
	pushl	%ebp
	movl	32(%esp),%ebp
	movl	36(%esp),%eax
	movl	%eax,min_caml_hp
	movl	$40,%eax
	call	min_caml_call_caml_fib
	call	min_caml_print_int
	call	min_caml_print_newline
	movl	$20,%ebx
	movl	$10,%eax
	call	min_caml_call_caml_add
	call	min_caml_print_int
	call	min_caml_print_newline
	popl	%ebp
	popl	%edi
	popl	%esi
	popl	%edx
	popl	%ecx
	popl	%ebx
	popl	%eax
	ret
