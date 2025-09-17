.include "src/ctrl.inc"

.equ sizeof_hellctrl, 0x30

/**
 * hellctrl.init
 * @param %rcx hellctrl
 * @param %rdx EFI_RUNTIME_SERVICES
 * @param %r8  EFI_BOOT_SERVICES
 */
.globl _hellctrl_init

/**
 * hellctrl.deinit
 * @param %rcx hellctrl
 */
.globl _hellctrl_deinit
