.data
.balign	8
.text
.globl add
add:
	pushl	%eax
	pushl	%ebx
	pushl	%ecx
	pushl	%edx
	pushl	%esi
	pushl	%edi
	pushl	%ebp
	movl	32(%esp), %eax
	movl	36(%esp), %ebx
	call	min_caml_add
	popl	%ebp
	popl	%edi
	popl	%esi
	popl	%edx
	popl	%ecx
	popl	%ebx
	popl	%eax
	ret
min_caml_add:
	addl	%ebx, %eax
	call	min_caml_print_int
	call	min_caml_print_newline
	ret
