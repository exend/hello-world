#ifndef BOOTSRV_H
#define BOOTSRV_H

#include <efi/types.h>
#include <efi/status.h>
#include <efi/tpl.h>
#include <efi/timerdelay.h>
#include <efi/tableheader.h>
#include <inttypes.h>

typedef void *EFI_EVENT;
typedef void (*EFI_EVENT_NOTIFY) (EFI_EVENT event, void *ctx);

struct _EFI_BOOT_SERVICES;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;

typedef EFI_STATUS (*EFI_CREATE_EVENT) (uint32_t type, EFI_TPL notify_tpl, EFI_EVENT_NOTIFY notify_func, void *notify_ctx, EFI_EVENT *event);
typedef EFI_STATUS (*EFI_SET_TIMER) (EFI_EVENT event, EFI_TIMER_DELAY type, uint64_t trigger_time);
typedef EFI_STATUS (*EFI_WAIT_FOR_EVENT) (UINTN num_events, EFI_EVENT *event, UINTN *index);
typedef EFI_STATUS (*EFI_SIGNAL_EVENT) (EFI_EVENT event);
typedef EFI_STATUS (*EFI_CLOSE_EVENT) (EFI_EVENT event);
typedef EFI_STATUS (*EFI_STALL) (UINTN microseconds);

struct _EFI_BOOT_SERVICES {
	EFI_TABLE_HEADER     Hdr;

	//
	// Task Priority Services
	//
	void                 *RaiseTPL;       // EFI 1.0+
	void                 *RestoreTPL;     // EFI 1.0+

	//
	// Memory Services
	//
	void                 *AllocatePages;  // EFI 1.0+
	void                 *FreePages;      // EFI 1.0+
	void                 *GetMemoryMap;   // EFI 1.0+
	void                 *AllocatePool;   // EFI 1.0+
	void                 *FreePool;       // EFI 1.0+

	//
	// Event & Timer Services
	//
	EFI_CREATE_EVENT     CreateEvent;    // EFI 1.0+
	EFI_SET_TIMER        SetTimer;       // EFI 1.0+
	EFI_WAIT_FOR_EVENT   WaitForEvent;   // EFI 1.0+
	EFI_SIGNAL_EVENT     SignalEvent;    // EFI 1.0+
	EFI_CLOSE_EVENT      CloseEvent;     // EFI 1.0+
	void                 *CheckEvent;    // EFI 1.0+

	//
	// Protocol Handler Services
	//
	void                           *InstallProtocolInterface;           // EFI 1.0+
	void                           *ReinstallProtocolInterface;         // EFI 1.0+
	void                           *UninstallProtocolInterface;         // EFI 1.0+
	void                           *HandleProtocol;                     // EFI 1.0+
	void                           *Reserved;                           // EFI 1.0+
	void                           *RegisterProtocolNotify;             // EFI 1.0+
	void                           *LocateHandle;                       // EFI 1.0+
	void                           *LocateDevicePath;                   // EFI 1.0+
	void                           *InstallConfigurationTable;          // EFI 1.0+

	//
	// Image Services
	//
	void                           *LoadImage;        // EFI 1.0+
	void                           *StartImage;       // EFI 1.0+
	void                           *Exit;             // EFI 1.0+
	void                           *UnloadImage;      // EFI 1.0+
	void                           *ExitBootServices; // EFI 1.0+

	//
	// Miscellaneous Services
	//
	void                           *GetNextMonotonicCount; // EFI 1.0+
	EFI_STALL                      Stall;                  // EFI 1.0+
	void                           *SetWatchdogTimer;      // EFI 1.0+

	//
	// DriverSupport Services
	//
	void                           *ConnectController;     // EFI 1.1
	void                           *DisconnectController;  // EFI 1.1+

	//
	// Open and Close Protocol Services
	//
	void                           *OpenProtocol;           // EFI 1.1+
	void                           *CloseProtocol;          // EFI 1.1+
	void                           *OpenProtocolInformation;// EFI 1.1+

	//
	// Library Services
	//
	void                           *ProtocolsPerHandle;                    // EFI 1.1+
	void                           *LocateHandleBuffer;                    // EFI 1.1+
	void                           *LocateProtocol;                        // EFI 1.1+
	void                           *InstallMultipleProtocolInterfaces;     // EFI 1.1+
	void                           *UninstallMultipleProtocolInterfaces;   // EFI 1.1+*

	//
	// 32-bit CRC Services
	//
	void                   *CalculateCrc32;     // EFI 1.1+

	//
	// Miscellaneous Services
	//
	void                   *CopyMem;            // EFI 1.1+
	void                   *SetMem;             // EFI 1.1+
	void                   *CreateEventEx;      // UEFI 2.0+
};

void efi_bootsrv_init(EFI_BOOT_SERVICES *bootsrv);

#endif // BOOTSRV_H
