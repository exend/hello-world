.text

.include "src/app.inc"
.include "src/view.inc"
.include "src/efi.inc"

.equ view_ptr, 0x00
.equ btsrv_ptr, 0x08
.equ event_ptr, 0x10
.equ exit, 0x18

.equ ms20, 500000

/**
 * app.init
 * @param %rcx app
 * @param %rdx view
 * @param %r8  EFI_BOOT_SERVICES
 */
_app_init:
	movq %rdx, view_ptr(%rcx)
	movq %r8, btsrv_ptr(%rcx)
	movq $0, event_ptr(%rcx)
	movq $0, exit(%rcx)
	ret

/**
 * app.deinit
 * @param %rcx app
 */
_app_deinit:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x20, %rsp
	call _stop_timer

	leave
	ret

/**
 * app.exit
 * @param %rcx app
 */
_app_exit:
	movq $1, exit(%rcx)
	ret

/**
 * app.start_timer
 * @param %rcx app
 */
_start_timer:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)

	movq btsrv_ptr(%rcx), %rax
	leaq event_ptr(%rcx), %r10
	movq $efi_evt_timer, %rcx
	movq $efi_tpl_app, %rdx
	xorq %r8, %r8
	xorq %r9, %r9
	subq $0x10, %rsp
	movq %r10, 0x20(%rsp)
	call *btsrv_create_evt(%rax) # btsrv_create_evt(efi_evt_timer, efi_tpl_app, 0, 0, &event_ptr)
	addq $0x10, %rsp

	movq -0x08(%rbp), %rcx
	movq btsrv_ptr(%rcx), %rax
	movq event_ptr(%rcx), %rcx
	movq $efi_timer_periodic, %rdx
	movq $ms20, %r8
	call *btsrv_set_timer(%rax)  # btsrv_set_timer(event_ptr, efit_timer_periodic, ms20)

	leave
	ret

/**
 * app.stop_timer
 * @param %rcx app
 */
_stop_timer:
	pushq %rbp
	movq %rsp, %rbp

	cmpq $0, event_ptr(%rcx)
	je st_exit
	
	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)

	movq btsrv_ptr(%rcx), %rax
	movq event_ptr(%rcx), %rcx
	call *btsrv_close_evt(%rax)

	movq -0x08(%rbp), %rcx
	movq $0, event_ptr(%rcx)
st_exit:
	leave
	ret

/**
 * app.wait_timer
 * @param %rcx app
 */
_wait_timer:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp

	movq btsrv_ptr(%rcx), %rax
	leaq event_ptr(%rcx), %rdx
	leaq -0x08(%rbp), %r8
	movq $1, %rcx
	call *btsrv_w84evt(%rax)     # w84evt(1, &event, &evt_idx)

	leave
	ret

/**
 * app.run_loop
 * @param %rcx app
 */
 _app_run_loop:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)

	movq view_ptr(%rcx), %rcx
	movq view_vptr(%rcx), %rax
	call *view_loaded(%rax)

	movq -0x08(%rbp), %rcx
	call _start_timer
mloop:
	movq -0x08(%rbp), %rax
	movq view_ptr(%rax), %rcx
	movq view_vptr(%rcx), %rdx
	call *view_draw_pending(%rdx)

	cmpq $0, %rax
	je wait

	movq -0x08(%rbp), %rax
	movq view_ptr(%rax), %rcx
	movq view_vptr(%rcx), %rdx
	call *view_draw(%rdx)
wait:
	movq -0x08(%rbp), %rcx
	call _wait_timer

	movq -0x08(%rbp), %rcx
	cmpq $0, exit(%rcx)
	je mloop

	leave
	ret
