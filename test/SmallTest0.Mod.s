	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"SmallTest0.Mod"
	.def	SmallTest0;
	.scl	2;
	.type	32;
	.endef
	.globl	SmallTest0
	.p2align	4, 0x90
SmallTest0:
.seh_proc SmallTest0
	subq	$40, %rsp
	.seh_stackalloc 40
	.seh_endprologue
	movq	a(%rip), %rax
	movq	(%rax), %rax
	movq	b(%rip), %rcx
	cmpq	(%rcx), %rax
	jle	.LBB0_5
	movq	c(%rip), %rax
	movq	(%rax), %rax
	movq	d(%rip), %rcx
	cmpq	(%rcx), %rax
	je	.LBB0_5
	movq	g(%rip), %rax
	movq	(%rax), %rax
	movq	h(%rip), %rcx
	cmpq	(%rcx), %rax
	jl	.LBB0_5
	movq	g(%rip), %rax
	cmpq	$51, (%rax)
	jge	.LBB0_5
	movq	g(%rip), %rax
	cmpq	$49, (%rax)
.LBB0_5:
	movq	$0, a(%rip)
	.p2align	4, 0x90
.LBB0_6:
	movq	g(%rip), %rax
	movq	(%rax), %rax
	movq	h(%rip), %rcx
	cmpq	(%rcx), %rax
	jge	.LBB0_8
	movq	g(%rip), %rax
	movq	(%rax), %rax
	decq	%rax
	movq	%rax, g(%rip)
	jmp	.LBB0_6
.LBB0_8:
	movq	c(%rip), %rax
	movq	(%rax), %rcx
	movq	d(%rip), %rax
	movq	(%rax), %rdx
	callq	func
	xorl	%eax, %eax
	addq	$40, %rsp
	retq
	.seh_endproc

	.def	func;
	.scl	2;
	.type	32;
	.endef
	.globl	func
	.p2align	4, 0x90
func:
.seh_proc func
	pushq	%rax
	.seh_stackalloc 8
	.seh_endprologue
	movq	$0, (%rsp)
	addq	%rdx, %rcx
	cmpq	%rcx, (%rsp)
	jge	.LBB1_3
	.p2align	4, 0x90
.LBB1_2:
	incq	(%rsp)
	cmpq	%rcx, (%rsp)
	jl	.LBB1_2
.LBB1_3:
	popq	%rax
	retq
	.seh_endproc

	.lcomm	a,8,8
	.lcomm	b,8,8
	.lcomm	c,8,8
	.lcomm	d,8,8
	.lcomm	e,8,8
	.lcomm	f,8,8
	.lcomm	g,8,8
	.lcomm	h,8,8
