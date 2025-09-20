#ifndef SYSTABLE_H
#define SYSTABLE_H

#include "output.h"
#include "rtsrv.h"
#include "bootsrv.h"

typedef void *EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef void *EFI_CONFIGURATION_TABLE;

typedef struct {
	EFI_TABLE_HEADER                 Hdr;
	wchar_t                         *FirmwareVendor;
	uint32_t                         FirmwareRevision;
	EFI_HANDLE                       ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL   *ConIn;
	EFI_HANDLE                       ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
	EFI_HANDLE                       StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *StdErr;
	EFI_RUNTIME_SERVICES             *RuntimeServices;
	EFI_BOOT_SERVICES                *BootServices;
	UINTN                            NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE          *ConfigurationTable;
} EFI_SYSTEM_TABLE;

void efi_systbl_init(
	EFI_SYSTEM_TABLE *systbl,
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout,
	EFI_RUNTIME_SERVICES *rtsrv,
	EFI_BOOT_SERVICES *bootsrv
);

#endif // SYSTABLE_H
