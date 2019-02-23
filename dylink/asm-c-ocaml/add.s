.data
.balign	8
.text
.globl _add
_add:
	pushl   %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %ebx
    movl    12(%ebp), %eax
    addl    %ebx, %eax
    popl    %ebp
	ret
