	.code32
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
	call	add.4
	movl	%eax, 32(%esp)
	popl	%ebp
	popl	%edi
	popl	%esi
	popl	%edx
	popl	%ecx
	popl	%ebx
	popl	%eax
	movl	4(%esp), %eax
	ret
	.globl add.4
add.4:
	addl	%ebx, %eax
	ret
