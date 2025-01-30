	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"Sort0.Mod"
	.def	Sort0;
	.scl	2;
	.type	32;
	.endef
	.globl	Sort0
	.p2align	4, 0x90
Sort0:
.seh_proc Sort0
	subq	$40, %rsp
	.seh_stackalloc 40
	.seh_endprologue
	movq	$20, Dim(%rip)
	callq	Init
	callq	QuickSort
	xorl	%eax, %eax
	addq	$40, %rsp
	retq
	.seh_endproc

	.def	Init;
	.scl	2;
	.type	32;
	.endef
	.globl	Init
	.p2align	4, 0x90
Init:
.seh_proc Init
	pushq	%rax
	.seh_stackalloc 8
	.seh_endprologue
	movq	$0, (%rsp)
	movq	Dim(%rip), %rax
	leaq	a(%rip), %rcx
	.p2align	4, 0x90
.LBB1_1:
	movq	(%rsp), %rdx
	cmpq	(%rax), %rdx
	jge	.LBB1_3
	movq	(%rax), %rdx
	movq	(%rsp), %r8
	subq	%r8, %rdx
	movq	%rdx, (%rcx,%r8,8)
	incq	%r8
	movq	%r8, (%rsp)
	jmp	.LBB1_1
.LBB1_3:
	popq	%rax
	retq
	.seh_endproc

	.def	Swap;
	.scl	2;
	.type	32;
	.endef
	.globl	Swap
	.p2align	4, 0x90
Swap:
.seh_proc Swap
	pushq	%rax
	.seh_stackalloc 8
	.seh_endprologue
	movq	(%rcx), %rax
	movq	(%rax), %rax
	movq	%rax, (%rsp)
	movq	(%rdx), %rax
	movq	(%rax), %rax
	movq	%rax, (%rcx)
	movq	(%rsp), %rax
	movq	%rax, (%rdx)
	popq	%rax
	retq
	.seh_endproc

	.def	InsertionSort;
	.scl	2;
	.type	32;
	.endef
	.globl	InsertionSort
	.p2align	4, 0x90
InsertionSort:
.seh_proc InsertionSort
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	subq	$56, %rsp
	.seh_stackalloc 56
	.seh_endprologue
	movq	$1, 48(%rsp)
	movq	Dim(%rip), %rsi
	leaq	a(%rip), %rdi
	jmp	.LBB3_1
	.p2align	4, 0x90
.LBB3_6:
	incq	48(%rsp)
.LBB3_1:
	movq	48(%rsp), %rax
	cmpq	(%rsi), %rax
	jge	.LBB3_7
	movq	48(%rsp), %rax
	movq	%rax, 40(%rsp)
	.p2align	4, 0x90
.LBB3_3:
	movq	40(%rsp), %rax
	testq	%rax, %rax
	jle	.LBB3_6
	movq	(%rdi,%rax,8), %rcx
	cmpq	%rcx, -8(%rdi,%rax,8)
	jle	.LBB3_6
	movq	40(%rsp), %rax
	leaq	(%rdi,%rax,8), %rcx
	leaq	(%rdi,%rax,8), %rdx
	addq	$-8, %rdx
	callq	Swap
	decq	40(%rsp)
	jmp	.LBB3_3
.LBB3_7:
	addq	$56, %rsp
	popq	%rdi
	popq	%rsi
	retq
	.seh_endproc

	.def	SelectionSort;
	.scl	2;
	.type	32;
	.endef
	.globl	SelectionSort
	.p2align	4, 0x90
SelectionSort:
.seh_proc SelectionSort
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	subq	$56, %rsp
	.seh_stackalloc 56
	.seh_endprologue
	movq	$0, 32(%rsp)
	movq	Dim(%rip), %rsi
	leaq	a(%rip), %rdi
	jmp	.LBB4_1
	.p2align	4, 0x90
.LBB4_9:
	incq	32(%rsp)
.LBB4_1:
	movq	(%rsi), %rax
	decq	%rax
	cmpq	%rax, 32(%rsp)
	jge	.LBB4_10
	movq	32(%rsp), %rax
	movq	%rax, 40(%rsp)
	incq	%rax
	movq	%rax, 48(%rsp)
	jmp	.LBB4_3
	.p2align	4, 0x90
.LBB4_6:
	incq	48(%rsp)
.LBB4_3:
	movq	48(%rsp), %rax
	cmpq	(%rsi), %rax
	jge	.LBB4_7
	movq	48(%rsp), %rax
	movq	(%rdi,%rax,8), %rax
	movq	40(%rsp), %rcx
	cmpq	(%rdi,%rcx,8), %rax
	jge	.LBB4_6
	movq	48(%rsp), %rax
	movq	%rax, 40(%rsp)
	jmp	.LBB4_6
	.p2align	4, 0x90
.LBB4_7:
	movq	40(%rsp), %rax
	cmpq	32(%rsp), %rax
	je	.LBB4_9
	movq	32(%rsp), %rax
	leaq	(%rdi,%rax,8), %rcx
	movq	40(%rsp), %rax
	leaq	(%rdi,%rax,8), %rdx
	callq	Swap
	jmp	.LBB4_9
.LBB4_10:
	addq	$56, %rsp
	popq	%rdi
	popq	%rsi
	retq
	.seh_endproc

	.def	BubbleSort;
	.scl	2;
	.type	32;
	.endef
	.globl	BubbleSort
	.p2align	4, 0x90
BubbleSort:
.seh_proc BubbleSort
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	subq	$56, %rsp
	.seh_stackalloc 56
	.seh_endprologue
	movq	$0, 48(%rsp)
	movq	Dim(%rip), %rsi
	leaq	a(%rip), %rdi
	jmp	.LBB5_1
	.p2align	4, 0x90
.LBB5_8:
	incq	48(%rsp)
.LBB5_1:
	movq	48(%rsp), %rax
	cmpq	(%rsi), %rax
	jge	.LBB5_7
	movq	(%rsi), %rax
	decq	%rax
	movq	%rax, 40(%rsp)
	jmp	.LBB5_3
	.p2align	4, 0x90
.LBB5_6:
	decq	40(%rsp)
.LBB5_3:
	movq	40(%rsp), %rax
	cmpq	48(%rsp), %rax
	jle	.LBB5_8
	movq	40(%rsp), %rax
	movq	-8(%rdi,%rax,8), %rcx
	cmpq	(%rdi,%rax,8), %rcx
	jle	.LBB5_6
	movq	40(%rsp), %rax
	leaq	(%rdi,%rax,8), %rcx
	addq	$-8, %rcx
	leaq	(%rdi,%rax,8), %rdx
	callq	Swap
	jmp	.LBB5_6
.LBB5_7:
	addq	$56, %rsp
	popq	%rdi
	popq	%rsi
	retq
	.seh_endproc

	.def	QuickSort;
	.scl	2;
	.type	32;
	.endef
	.globl	QuickSort
	.p2align	4, 0x90
QuickSort:
.seh_proc QuickSort
	subq	$40, %rsp
	.seh_stackalloc 40
	.seh_endprologue
	movq	Dim(%rip), %rax
	movq	(%rax), %rdx
	decq	%rdx
	xorl	%ecx, %ecx
	callq	QSort
	nop
	addq	$40, %rsp
	retq
	.seh_endproc

	.def	QSort;
	.scl	2;
	.type	32;
	.endef
	.globl	QSort
	.p2align	4, 0x90
QSort:
.seh_proc QSort
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	pushq	%rbx
	.seh_pushreg %rbx
	subq	$64, %rsp
	.seh_stackalloc 64
	.seh_endprologue
	movq	%rdx, %rsi
	movq	%rcx, %rdi
	movq	%rcx, 48(%rsp)
	movq	%rdx, 40(%rsp)
	leaq	(%rdx,%rcx), %rax
	movq	%rax, %rcx
	shrq	$63, %rcx
	addq	%rax, %rcx
	andq	$-2, %rcx
	leaq	a(%rip), %rbx
	movq	(%rbx,%rcx,4), %rax
	movq	%rax, 56(%rsp)
	.p2align	4, 0x90
.LBB7_1:
	movq	48(%rsp), %rax
	cmpq	40(%rsp), %rax
	jle	.LBB7_7
	jmp	.LBB7_2
	.p2align	4, 0x90
.LBB7_8:
	incq	48(%rsp)
.LBB7_7:
	movq	48(%rsp), %rax
	movq	(%rbx,%rax,8), %rax
	cmpq	56(%rsp), %rax
	jl	.LBB7_8
	jmp	.LBB7_9
	.p2align	4, 0x90
.LBB7_10:
	decq	40(%rsp)
.LBB7_9:
	movq	56(%rsp), %rax
	movq	40(%rsp), %rcx
	cmpq	(%rbx,%rcx,8), %rax
	jl	.LBB7_10
	movq	48(%rsp), %rax
	cmpq	40(%rsp), %rax
	jg	.LBB7_1
	movq	48(%rsp), %rax
	leaq	(%rbx,%rax,8), %rcx
	movq	40(%rsp), %rax
	leaq	(%rbx,%rax,8), %rdx
	callq	Swap
	incq	48(%rsp)
	decq	40(%rsp)
	jmp	.LBB7_1
.LBB7_2:
	cmpq	40(%rsp), %rdi
	jge	.LBB7_4
	movq	40(%rsp), %rdx
	movq	%rdi, %rcx
	callq	QSort
.LBB7_4:
	cmpq	%rsi, 48(%rsp)
	jge	.LBB7_6
	movq	48(%rsp), %rcx
	movq	%rsi, %rdx
	callq	QSort
.LBB7_6:
	nop
	addq	$64, %rsp
	popq	%rbx
	popq	%rdi
	popq	%rsi
	retq
	.seh_endproc

	.section	.rdata,"dr"
	.p2align	3, 0x0
Dim:
	.quad	0

	.lcomm	a,160,16
