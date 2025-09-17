#ifndef SYSTABLE_H
#define SYSTABLE_H

#include <inttypes.h>
#include "types.h"
#include "tableheader.h"
#include "cout.h"
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
	struct mock_cout                 *ConOut;
	EFI_HANDLE                       StandardErrorHandle;
	struct mock_cout                 *StdErr;
	struct mock_rtsrv                *RuntimeServices;
	struct mock_btsrv                *BootServices;
	UINTN                            NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE          *ConfigurationTable;
} EFI_SYSTEM_TABLE;

void efi_systbl_init(
	EFI_SYSTEM_TABLE *systbl,
	struct mock_cout *cout,
	struct mock_rtsrv *rtsrv,
	struct mock_btsrv *bootsrv
);

#endif // SYSTABLE_H
