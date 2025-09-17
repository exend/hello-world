#include "bootsrv.h"
#include "evttypes.h"
#include "status.h"
#include <string.h>
#include <stdlib.h>

struct mock_event mock_event;

static EFI_STATUS mock_btsrv_create_evt(
	uint32_t type,
	EFI_TPL notify_tpl,
	EFI_EVENT_NOTIFY notify_func,
	void *notify_ctx,
	EFI_EVENT *event
) {
	memset(&mock_event, 0, sizeof(struct mock_event));
	mock_event.evt_type = type;
	mock_event.notify_tpl = notify_tpl;
	mock_event.notify_func = notify_func;
	mock_event.notify_ctx = notify_ctx;
	++mock_event.create_event_called;
	*event = &mock_event;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_btsrv_set_timer(
	EFI_EVENT event,
	EFI_TIMER_DELAY type,
	uint64_t trigger_time
) {
	event->delay_type = type;
	event->trigger_time = trigger_time;
	++event->set_timer_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_btsrv_w84evt(UINTN num_events, EFI_EVENT *event, UINTN *index) {
	++(*event)->wait_for_event_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_btsrv_close_evt(EFI_EVENT event) {
	++event->close_event_called;
	return EFI_SUCCESS;
}

void mock_btsrv_init(struct mock_btsrv *bootsrv) {
	memset(bootsrv, 0, sizeof(struct mock_btsrv));
	bootsrv->create_event = mock_btsrv_create_evt;
	bootsrv->set_timer = mock_btsrv_set_timer;
	bootsrv->wait_for_event = mock_btsrv_w84evt;
	bootsrv->close_event = mock_btsrv_close_evt;
}
