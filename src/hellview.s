.data

vtable:         .quad _draw_pending, _draw, _loaded, _upd_sec
hello_msg:      .2byte 'H', 'E', 'L', 'L', ' ', 'W', 'O', 'R', 'D', 0
shutdwn_msg:    .2byte 'T', 'h', 'e', ' ', 's', 'y', 's', 't', 'e', 'm', ' '
                .2byte 'w', 'i', 'l', 'l', ' ', 's', 'h', 'u', 't', ' '
                .2byte 'd', 'o', 'w', 'n', ' ', 'i', 'n', ' ', 0
seconds_msg:    .2byte ' ', 's', 'e', 'c', 'o', 'n', 'd', 's', 0
second_msg:     .2byte ' ', 's', 'e', 'c', 'o', 'n', 'd', 0
second_msg_end:

.text

.include "src/hellview.inc"
.include "src/ctrl.inc"
.include "src/efi.inc"

.equ vptr, 0x00
.equ draw_pending, 0x08
.equ ctrl_ptr, 0x10
.equ seconds, 0x18
.equ cout_ptr, 0x20
.equ ccolumns, 0x28
.equ crows, 0x30

.equ hello_msg_len, (shutdwn_msg - hello_msg - 2) / 2
.equ shutdwn_msg_len, (seconds_msg - shutdwn_msg - 2) / 2
.equ seconds_msg_len, (second_msg - seconds_msg - 2) / 2
.equ second_msg_len, (second_msg_end - second_msg - 2) / 2

/**
 * hellview.init
 * @param %rcx hellview
 * @param %rdx ctrl
 * @param %r8 EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
 */
.globl _hellview_init
_hellview_init:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)

	leaq vtable(%rip), %rax
	movq %rax, vptr(%rcx)
	movq $0, seconds(%rcx)
	movq %rdx, ctrl_ptr(%rcx)
	movq %r8, cout_ptr(%rcx)

	call _setup
	
	movq -0x08(%rbp), %rdx
	movq ctrl_ptr(%rdx), %rcx
	movq ctrl_vptr(%rcx), %rax
	call *ctrl_set_view(%rax)

	leave
	ret

/**
 * hellview.deinit
 * @param %rcx hellview
 */
.globl _hellview_deinit
_hellview_deinit:
	ret

/**
 * hellview.draw_pending
 * @param %rcx hellview
 * @return %rax boolean
 */
_draw_pending:
	movq draw_pending(%rcx), %rax
	ret

/**
 * hellview.draw
 * @param %rcx hellview
 */
_draw:
	pushq %rbp
	movq %rsp, %rbp

	movq $0, draw_pending(%rcx)
	
	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)

	movq cout_ptr(%rcx), %rax
	movq %rax, %rcx
	call *cout_clear_scrn(%rcx)

	movq -0x08(%rbp), %rcx
	call _print_hello_msg

	movq -0x08(%rbp), %rcx
	movq seconds(%rcx), %rdx
	call _print_shutdwn_msg

	leave
	ret

/**
 * hellview.loaded
 * @param %rcx hellview
 */
_loaded:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x20, %rsp
	
	movq ctrl_ptr(%rcx), %rcx
	movq ctrl_vptr(%rcx), %rax
	call *ctrl_view_loaded(%rax)

	leave
	ret

/**
 * hellview.upd_sec
 * @param %rcx hellview
 * @param %rdx uint64_t
 */
_upd_sec:
	movq %rdx, seconds(%rcx)
	movq $1, draw_pending(%rcx)
	ret

/**
 * hellview.setup
 * @param %rcx this
 */
_setup:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	movq %rcx, %rax
	movq %rax, -0x08(%rbp)

	movq cout_ptr(%rax), %rcx
	xorq %rdx, %rdx
	call *cout_set_mode(%rcx)    # cout_set_mode(cout_ptr, 0)

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	movq $efi_brown, %rdx
	call *cout_set_attr(%rcx)    # cout_set_attr(cout_ptr, efi_brown)

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	xorq %rdx, %rdx
	leaq ccolumns(%rax), %r8
	leaq crows(%rax), %r9
	call *cout_query_mode(%rcx)  # cout_query_mode(cout_ptr, 0, &ccolumns, &crows)

	leave
	ret

/**
 * hellview.calc_hcentr
 * @param %rcx this
 * @param %rdx strlen
 */
_calc_hcentr:
	movq $2, %r9
	movq ccolumns(%rcx), %rax
	subq %rdx, %rax              # ccolumns - strlen
	xorq %rdx, %rdx
	divq %r9                     # (ccolumns - strlen) / 2
	ret

/**
 * hellview.calc_vcentr
 * @param %rcx this
 */
_calc_vcentr:
	movq $2, %r9
	movq crows(%rcx), %rax
	xorq %rdx, %rdx
	divq %r9                     # crows / 2
	ret

/**
 * hellview.print_hello
 * @param %rcx this
 */
_print_hello_msg:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)

	movq $hello_msg_len, %rdx
	call _calc_hcentr
	movq %rax, -0x10(%rbp)

	movq -0x08(%rbp), %rcx
	call _calc_vcentr
	movq %rax, %r10
	subq $1, %r10

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	movq -0x10(%rbp), %rdx
	movq %r10, %r8
	call *cout_set_cursor(%rcx)  # cout_set_cursor(cout_ptr, calc_hcentr(hello_msg_len), calc_vcentr() - 1)

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	leaq hello_msg(%rip), %rdx
	call *cout_oput_str(%rcx)    # cout_oput_str(cout_ptr, hello_msg)

	leave
	ret

/**
 * hellview.print_hello
 * @param %rcx this
 * @param %rdx sec_left
 */
_print_shutdwn_msg:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x40, %rsp
	movq %rcx, -0x08(%rbp)
	movq %rdx, -0x10(%rbp)

	movq %rdx, %rcx
	movq $0x08, %rdx
	leaq -0x20(%rbp), %r8
	call _num2str

	movq -0x08(%rbp), %rcx
	movq $shutdwn_msg_len, %rdx
	addq %rax, %rdx
	movq -0x10(%rbp), %rax
	cmpq $1, %rax                # sec_left == 1
	jne plural
	addq $second_msg_len, %rdx
	leaq second_msg(%rip), %rax
	movq %rax, -0x10(%rbp)
	jmp singular
plural:
	addq $seconds_msg_len, %rdx
	leaq seconds_msg(%rip), %rax
	movq %rax, -0x10(%rbp)
singular:
	call _calc_hcentr
	subq $0x10, %rsp
	movq %rax, -0x30(%rbp)

	movq -0x08(%rbp), %rcx
	call _calc_vcentr
	movq %rax, %r10
	addq $1, %r10

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	movq -0x30(%rbp), %rdx
	movq %r10, %r8
	call *cout_set_cursor(%rcx)  # cout_set_cursor(cout_ptr, calc_hcentr(shutdwn_msg_len + num2str(sec_left) + seconds_msg_len), calc_vcentr() + 1)

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	leaq shutdwn_msg(%rip), %rdx
	call *cout_oput_str(%rcx)    # cout_oput_str(cout_ptr, shutdwn_msg)

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	leaq -0x20(%rbp), %rdx
	call *cout_oput_str(%rcx)    # cout_oput_str(cout_ptr, sec_left)

	movq -0x08(%rbp), %rax
	movq cout_ptr(%rax), %rcx
	movq -0x10(%rbp), %rdx
	call *cout_oput_str(%rcx)    # cout_oput_str(cout_ptr, seconds_msg)

	leave
	ret
