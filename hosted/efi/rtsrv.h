#ifndef RTSRV_H
#define RTSRV_H

#include <efi/types.h>
#include <efi/status.h>
#include <efi/tableheader.h>

typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
} EFI_RESET_TYPE;

struct _EFI_RUNTIME_SERVICES;
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;

typedef void (*EFI_RESET_SYSTEM) (EFI_RESET_TYPE reset_type, EFI_STATUS reset_status, UINTN data_size, void *reset_data);

struct _EFI_RUNTIME_SERVICES {
	EFI_TABLE_HEADER                 Hdr;

	//
	// Time Services
	//
	void                             *GetTime;
	void                             *SetTime;
	void                             *GetWakeupTime;
	void                             *SetWakeupTime;

	//
	// Virtual Memory Services
	//
	void                             *SetVirtualAddressMap;
	void                             *ConvertPointer;

	//
	// Variable Services
	//
	void                             *GetVariable;
	void                             *GetNextVariableName;
	void                             *SetVariable;

	//
	// Miscellaneous Services
	//
	void                             *GetNextHighMonotonicCount;
	EFI_RESET_SYSTEM                 ResetSystem;

	//
	// UEFI 2.0 Capsule Services
	//
	void                             *UpdateCapsule;
	void                             *QueryCapsuleCapabilities;

	//
	// Miscellaneous UEFI 2.0 Service
	//
	void                             *QueryVariableInfo;
};

void efi_rnsrv_init(EFI_RUNTIME_SERVICES *rnsrv);

#endif // RTSRV_H
