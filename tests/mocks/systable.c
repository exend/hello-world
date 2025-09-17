#include "systable.h"

void efi_systbl_init(
	EFI_SYSTEM_TABLE *systbl,
	struct mock_cout *cout,
	struct mock_rtsrv *rtsrv,
	struct mock_btsrv *bootsrv
) {
	memset(systbl, 0, sizeof(EFI_SYSTEM_TABLE));
	mock_cout_init(cout);
	mock_rtsrv_init(rtsrv);
	mock_btsrv_init(bootsrv);
	systbl->ConOut = cout;
	systbl->RuntimeServices = rtsrv;
	systbl->BootServices = bootsrv;
}
