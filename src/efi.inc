.equ efi_success, 0
.equ efi_shutdwn, 2
.equ efi_brown, 0x06
.equ efi_timer_periodic, 1

.equ efi_evt_timer, 0x80000000
.equ efi_evt_notify_signal, 0x00000200

.equ efi_tpl_app, 4
.equ efi_tpl_callback, 8
.equ efi_tpl_notify, 16

.equ cout_ptr, 0x40
.equ cout_oput_str, 0x08
.equ cout_query_mode, 0x18
.equ cout_set_mode, 0x20
.equ cout_set_attr, 0x28
.equ cout_clear_scrn, 0x30
.equ cout_set_cursor, 0x38

.equ rtsrv_ptr, 0x58
.equ rtsrv_reset_sys, 0x68

.equ btsrv_ptr, 0x60
.equ btsrv_create_evt, 0x50
.equ btsrv_set_timer, 0x58
.equ btsrv_w84evt, 0x60
.equ btsrv_close_evt, 0x70
