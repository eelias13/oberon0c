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
	subq	$104, %rsp
	.seh_stackalloc 104
	.seh_endprologue
	movq	80(%rsp), %rax
	cmpq	72(%rsp), %rax
	jle	.LBB0_4
	movq	64(%rsp), %rax
	cmpq	56(%rsp), %rax
	je	.LBB0_4
	movq	40(%rsp), %rax
	cmpq	48(%rsp), %rax
	jl	.LBB0_4
	cmpq	$50, 40(%rsp)
.LBB0_4:
	movq	40(%rsp), %rax
	.p2align	4, 0x90
.LBB0_5:
	cmpq	48(%rsp), %rax
	jl	.LBB0_5
	movq	64(%rsp), %rcx
	movq	56(%rsp), %rdx
	callq	func
	xorl	%eax, %eax
	addq	$104, %rsp
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
	movq	(%rsp), %rax
	addq	%rdx, %rcx
	.p2align	4, 0x90
.LBB1_1:
	cmpq	%rcx, %rax
	jl	.LBB1_1
	popq	%rax
	retq
	.seh_endproc

