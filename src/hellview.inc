.include "src/view.inc"
.equ hellview_upd_sec, 0x18

.equ sizeof_hellview, 0x40

/**
 * hellview.init
 * @param %rcx hellview
 * @param %rdx ctrl
 * @param %r8 EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
 */
.globl _hellview_init

/**
 * hellview.deinit
 * @param %rcx hellview
 */
.globl _hellview_deinit
