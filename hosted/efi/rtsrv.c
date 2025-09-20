#include "rtsrv.h"
#include <string.h>
#include <stdlib.h>

void efi_reset_sys(
	EFI_RESET_TYPE reset_type,
	EFI_STATUS reset_status,
	UINTN data_size,
	void *reset_data
) {
	exit(0);
}

void efi_rnsrv_init(EFI_RUNTIME_SERVICES *rtsrv) {
	memset(rtsrv, 0, sizeof(EFI_RUNTIME_SERVICES));
	rtsrv->ResetSystem = efi_reset_sys;
}
