.text

/**
 * Base 10 logarithm function.
 * @param %xmm0 double arg.
 * @return The base 10 logarithm of %xmm0.
 */
.globl _log10
_log10:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	call _log2
	movq %xmm0, -8(%rbp)
	fldl2t					# st(0) = log2(10)
	fldl -8(%rbp)			# st(1) = st(0); st(0) = log2(xmm0)
	fdivp					# st(0) = st(1) / st(0)
	fstpl -8(%rbp)			# *(rbp-1) = log2(xmm0) / log2(10)
	movq -8(%rbp), %xmm0	# xmm0 = log10(*(rbp-1))

	movq %rbp, %rsp
	popq %rbp
	ret
