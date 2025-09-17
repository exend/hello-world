.text

.include "src/hellctrl.inc"
.include "src/hellview.inc"
.include "src/app.inc"
.include "src/efi.inc"

.equ systbl_ptr, -0x10
.equ hellctrl, systbl_ptr - sizeof_hellctrl
.equ hellview, hellctrl - sizeof_hellview
.equ app, hellview - sizeof_app
.equ stacksize, -app

.globl _efi_entry
_efi_entry:                      # EFI_STATUS (EFI_HANDLE hndl, EFI_SYSTEM_TABLE *systbl)
	pushq %rbp
	movq %rsp, %rbp
	
	subq $stacksize, %rsp
	movq %rdx, systbl_ptr(%rbp)

	subq $0x20, %rsp             # shadow space

	leaq hellctrl(%rbp), %rcx
	movq systbl_ptr(%rbp), %rax
	movq rtsrv_ptr(%rax), %rdx
	movq btsrv_ptr(%rax), %r8
	call _hellctrl_init

	leaq hellview(%rbp), %rcx
	leaq hellctrl(%rbp), %rdx
	movq systbl_ptr(%rbp), %rax
	movq cout_ptr(%rax), %r8
	call _hellview_init

	leaq app(%rbp), %rcx
	leaq hellview(%rbp), %rdx
	movq systbl_ptr(%rbp), %rax
	movq btsrv_ptr(%rax), %r8
	call _app_init
	
	leaq app(%rbp), %rcx
	call _app_run_loop

	leaq app(%rbp), %rcx
	call _app_deinit
	leaq hellview(%rbp), %rcx
	call _hellview_deinit
	leaq hellctrl(%rbp), %rcx
	call _hellctrl_deinit

	movq %rbp, %rsp
	popq %rbp

	xorq %rax, %rax
	ret
