#include "bootsrv.h"
#include <efi/evttypes.h>
#include <string.h>
#include <stdlib.h>

EFI_STATUS efi_create_evt(uint32_t, EFI_TPL, EFI_EVENT_NOTIFY, void *, EFI_EVENT *);
EFI_STATUS efi_set_timer(EFI_EVENT, EFI_TIMER_DELAY, uint64_t);
EFI_STATUS efi_w84evt(UINTN, EFI_EVENT *, UINTN *);
EFI_STATUS efi_signal_evt(EFI_EVENT event);
EFI_STATUS efi_close_evt(EFI_EVENT);
EFI_STATUS efi_stall(UINTN microseconds);

void efi_bootsrv_init(EFI_BOOT_SERVICES *bootsrv) {
	memset(bootsrv, 0, sizeof(EFI_BOOT_SERVICES));
	bootsrv->CreateEvent = efi_create_evt;
	bootsrv->SetTimer = efi_set_timer;
	bootsrv->WaitForEvent = efi_w84evt;
	bootsrv->SignalEvent = efi_signal_evt;
	bootsrv->CloseEvent = efi_close_evt;
	bootsrv->Stall = efi_stall;
}

#include <windows.h>

struct event {
	HANDLE hndl;
	PTP_TIMER timer;
	EFI_EVENT_NOTIFY notify_func;
	void *notify_ctx;
};

VOID CALLBACK timer_callback(PTP_CALLBACK_INSTANCE inst, PVOID ctx, PTP_TIMER timer) {
	struct event *evt = (struct event *)ctx;
	SetEvent(evt->hndl);
	if (evt->notify_func)
		evt->notify_func(evt, evt->notify_ctx);
}

EFI_STATUS efi_create_evt(
	uint32_t type,
	EFI_TPL notify_tpl,
	EFI_EVENT_NOTIFY notify_func,
	void *notify_ctx,
	EFI_EVENT *event
) {
	struct event *evt = malloc(sizeof(struct event));
	evt->hndl = CreateEventW(NULL, FALSE, FALSE, NULL);
	if (type & EVT_TIMER)
		evt->timer = CreateThreadpoolTimer(timer_callback, evt, NULL);
	evt->notify_func = notify_func;
	evt->notify_ctx = notify_ctx;
	*event = evt;
	return EFI_SUCCESS;
}

EFI_STATUS efi_set_timer(
	EFI_EVENT event,
	EFI_TIMER_DELAY type,
	uint64_t trigger_time
) {
	struct event *evt = (struct event *)event;
	LONG period = trigger_time / 10000L;
	LARGE_INTEGER due_time;
	FILETIME ft;
	due_time.QuadPart = -trigger_time;
	ft.dwLowDateTime = due_time.LowPart;
	ft.dwHighDateTime = due_time.HighPart;
	SetThreadpoolTimer(evt->timer, &ft, period, 0);
	return EFI_SUCCESS;
}

EFI_STATUS efi_w84evt(UINTN num_events, EFI_EVENT *event, UINTN *index) {
	struct event *evt = (struct event *)*event;
	WaitForSingleObject(evt->hndl, INFINITE);
	return EFI_SUCCESS;
}

EFI_STATUS efi_signal_evt(EFI_EVENT event) {
	struct event *evt = (struct event *)event;
	SetEvent(evt->hndl);
	return EFI_SUCCESS;
}

EFI_STATUS efi_close_evt(EFI_EVENT event) {
	struct event *evt = (struct event *)event;
	CloseThreadpoolTimer(evt->timer);
	CloseHandle(evt->hndl);
	free(evt);
	return EFI_SUCCESS;
}

EFI_STATUS efi_stall(UINTN microseconds) {
	Sleep(microseconds / 1000U);
	return EFI_SUCCESS;
}
