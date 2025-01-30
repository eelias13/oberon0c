	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"Square0.Mod"
	.def	Square;
	.scl	2;
	.type	32;
	.endef
	.globl	Square
	.p2align	4, 0x90
Square:
.seh_proc Square
	subq	$40, %rsp
	.seh_stackalloc 40
	.seh_endprologue
	movq	$4, num(%rip)
	leaq	num(%rip), %rcx
	callq	CalcSquare
	xorl	%eax, %eax
	addq	$40, %rsp
	retq
	.seh_endproc

	.def	CalcSquare;
	.scl	2;
	.type	32;
	.endef
	.globl	CalcSquare
	.p2align	4, 0x90
CalcSquare:
	movq	(%rcx), %rax
	movq	(%rax), %rax
	imulq	%rax, %rax
	movq	%rax, result(%rip)
	retq

	.lcomm	num,8,8
	.lcomm	result,8,8
