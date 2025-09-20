#include "efi/systable.h"

extern EFI_STATUS _efi_entry(EFI_HANDLE hndl, EFI_SYSTEM_TABLE *systbl);

int main() {
	EFI_BOOT_SERVICES btsrv;
	EFI_RUNTIME_SERVICES rtsrv;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL cout;
	EFI_SYSTEM_TABLE systbl;

	efi_bootsrv_init(&btsrv);
	efi_rnsrv_init(&rtsrv);
	efi_txt_output_init(&cout);
	efi_systbl_init(&systbl, &cout, &rtsrv, &btsrv);

	_efi_entry(0, &systbl);

	return 0;
}
