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
	movl	$10, %eax
	movl	$0, %ebx
	call	min_caml_create_array
	movl	$1, %ebx
	movl	%ebx, 0(%eax)
	movl	$2, %ebx
	movl	%ebx, 4(%eax)
	movl	$3, %ebx
	movl	%ebx, 8(%eax)
	pushl	%eax
	call	_h
	popl	%ebp
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
