.data

vtable: .quad _set_view, _view_loaded

.text

.include "src/hellctrl.inc"
.include "src/hellview.inc"
.include "src/efi.inc"

.equ vptr, 0x00
.equ hellview_ptr, 0x08
.equ rtsrv_ptr, 0x10
.equ btsrv_ptr, 0x18
.equ event_ptr, 0x20
.equ sec_left, 0x28

.equ sec, 10000000
.equ delay, 0x10

/**
 * hellctrl.init
 * @param %rcx hellctrl
 * @param %rdx EFI_RUNTIME_SERVICES
 * @param %r8  EFI_BOOT_SERVICES
 */
_hellctrl_init:
	leaq vtable(%rip), %rax
	movq %rax, vptr(%rcx)
	movq $0, hellview_ptr(%rcx)
	movq %rdx, rtsrv_ptr(%rcx)
	movq %r8, btsrv_ptr(%rcx)
	movq $0, event_ptr(%rcx)
	movq $delay, sec_left(%rcx)
	ret

/**
 * hellctrl.deinit
 * @param %rcx hellctrl
 */
_hellctrl_deinit:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x20, %rsp
	call _stop_timer

	leave
	ret

/**
 * hellctrl.set_view
 * @param %rcx hellctrl
 * @param %rdx hellview
 */
_set_view:
	movq %rdx, hellview_ptr(%rcx)
	ret

/**
 * hellctrl.view_loaded
 * @param %rcx hellctrl
 */
_view_loaded:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)
	
	movq %rcx, %rdx
	movq hellview_ptr(%rdx), %rcx
	movq view_vptr(%rcx), %rax
	movq sec_left(%rdx), %rdx
	call *hellview_upd_sec(%rax) # hellview_ptr->upd_sec(sec_left)

	movq -0x08(%rbp), %rcx
	call _start_timer

	leave
	ret

/**
 * hellctrl.stop_timer
 * @param %rcx hellctrl
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
 * hellctrl.shutdwn
 * @param %rcx hellctrl
 */
_shutdwn:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x20, %rsp
	movq rtsrv_ptr(%rcx), %rax
	movq $efi_shutdwn, %rcx
	movq $efi_success, %rdx
	xorq %r8, %r8
	xorq %r9, %r9
	call *rtsrv_reset_sys(%rax)  # rtsrv_reset_sys(efi_shutdwn, efi_success, 0, 0)

	leave
	ret

/**
 * hellctrl.tick
 * @param %rcx EFI_EVENT
 * @param %rdx hellctrl
 */
_hellctrl_tick:
	pushq %rbp
	movq %rsp, %rbp

	subq $0x20, %rsp

	cmpq $0, sec_left(%rdx)
	ja continue

	subq $0x10, %rsp
	movq %rdx, -8(%rbp)

	movq %rdx, %rcx
	call _stop_timer

	movq -8(%rbp), %rcx
	call _shutdwn

	jmp tick_exit
continue:
	subq $1, sec_left(%rdx)
	movq hellview_ptr(%rdx), %rcx
	movq view_vptr(%rcx), %rax
	movq sec_left(%rdx), %rdx
	call *hellview_upd_sec(%rax) # hellview_ptr->upd_sec(sec_left)
tick_exit:	
	leave
	ret

/**
 * hellctrl.start_timer
 * @param %rcx hellctrl
 */
_start_timer:
	pushq %rbp
	movq %rsp, %rbp
	
	subq $0x30, %rsp
	movq %rcx, -0x08(%rbp)
	movq btsrv_ptr(%rcx), %rax
	movq %rax, -0x10(%rbp)

	subq $0x10, %rsp
	leaq event_ptr(%rcx), %rax
	movq %rax, 0x20(%rsp)
	movq %rcx, %r9
	leaq _hellctrl_tick(%rip), %r8
	movq $efi_tpl_callback, %rdx
	movq $efi_evt_timer, %rcx
	orq $efi_evt_notify_signal, %rcx
	movq -0x10(%rbp), %rax
	call *btsrv_create_evt(%rax) # btsrv_create_evt(efi_evt_timer | efi_evt_notify_signal, efi_tpl_callback, tick, this, &event_ptr)
	addq $0x10, %rsp

	movq $sec, %r8
	movq $efi_timer_periodic, %rdx
	movq -0x08(%rbp), %rax
	movq event_ptr(%rax), %rcx
	movq -0x10(%rbp), %rax
	call *btsrv_set_timer(%rax)  # btsrv_set_timer(event_ptr, efit_timer_periodic, sec)

	leave
	ret
