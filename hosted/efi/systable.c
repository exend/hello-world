#include "systable.h"

void efi_systbl_init(
	EFI_SYSTEM_TABLE *systbl,
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout,
	EFI_RUNTIME_SERVICES *rtsrv,
	EFI_BOOT_SERVICES *bootsrv
) {
	memset(systbl, 0, sizeof(EFI_SYSTEM_TABLE));
	efi_txt_output_init(cout);
	efi_rnsrv_init(rtsrv);
	efi_bootsrv_init(bootsrv);
	systbl->ConOut = cout;
	systbl->RuntimeServices = rtsrv;
	systbl->BootServices = bootsrv;
}
