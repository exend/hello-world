.text

/**
 * Convert number to string
 * @param %rcx uint64_t num
 * @param %rdx (size_t buff_size) The max number of utf-16 characters.
 * @param %r8 wchar_t *buff
 * @return size_t The number of utf-16 characters written.
 */
.globl _num2str
_num2str:
	pushq %rbp
	movq %rsp, %rbp

	cmpq $0, %rcx
	jne not0
	cmpq $2, %rdx
	jb ret0
	movl $'0', (%r8)		# memcpy(buff, L"0", 4)
	movq $1, %rax
	jmp exit

not0:
	subq $0x20, %rsp
	movq %rcx, -8(%rbp)
	movq %rdx, -0x10(%rbp)
	movq %r8, -0x18(%rbp)

	cvtsi2sd %rcx, %xmm0	# xmm0 = (double)num
	subq $0x20, %rsp
	call _log10				# xmm0 = log10(xmm0)
	cvttsd2si %xmm0, %rcx	# rcx = (uint64_t)xmm0

	cmpq %rcx, -0x10(%rbp)	# rcx >= buff_size
	jbe ret0

	movq %rcx, -0x20(%rbp)
	addq $1, -0x20(%rbp)

	movq -0x18(%rbp), %r8
	movw $0, 2(%r8, %rcx, 2)

	movq -8(%rbp), %rax		# rax = num
	movq $10, %r10
loop_start:
	xorq %rdx, %rdx
	divq %r10
	addw $'0', %dx
	movw %dx, (%r8, %rcx, 2)
	cmpq $0, %rcx
	je loop_end
	subq $1, %rcx
	jmp loop_start
loop_end:

	movq -0x20(%rbp), %rax
	movw $0, (%r8, %rax, 2)

exit:
	movq %rbp, %rsp
	popq %rbp
	ret

ret0:
	xorq %rax, %rax
	jmp exit
