.text

/**
 * Base 2 logarithm function.
 * @param %xmm0 double arg.
 * @return The base 2 logarithm of %xmm0.
 */
.globl _log2
_log2:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x10, %rsp
	movq %xmm0, -8(%rbp)
	fld1					# st(0) = 1.
	fldl -8(%rbp)			# st(1) = st(0); st(0) = *(rbp-1)
	fyl2x					# st(0) = st(1) * log2(st(0))
	fstpl -8(%rbp)			# *(rbp-1) = log2(st(0))
	movq -8(%rbp), %xmm0

	movq %rbp, %rsp
	popq %rbp
	ret
