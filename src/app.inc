.equ sizeof_app, 0x20

/**
 * app.init
 * @param %rcx app
 * @param %rdx view
 * @param %r8  EFI_BOOT_SERVICES
 */
.globl _app_init

/**
 * app.deinit
 * @param %rcx app
 */
.globl _app_deinit

/**
 * app.exit
 * @param %rcx app
 */
.globl _app_exit

/**
 * app.run_loop
 * @param %rcx app
 */
 .globl _app_run_loop
 