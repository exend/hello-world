#ifndef BOOTSRV_H
#define BOOTSRV_H

#include "types.h"
#include "tpl.h"
#include "timerdelay.h"
#include "tableheader.h"
#include <inttypes.h>

struct mock_event;
typedef struct mock_event *EFI_EVENT;
typedef void (*EFI_EVENT_NOTIFY) (EFI_EVENT event, void *ctx);

struct mock_event {
	uint32_t evt_type;
	EFI_TPL notify_tpl;
	EFI_EVENT_NOTIFY notify_func;
	void *notify_ctx;
	int create_event_called;

	EFI_TIMER_DELAY delay_type;
	uint64_t trigger_time;
	int set_timer_called;

	int wait_for_event_called;

	int close_event_called;
};

extern struct mock_event mock_event;

struct mock_btsrv {
	EFI_TABLE_HEADER hdr;
	void *padding[7];
	EFI_STATUS (*create_event) (uint32_t type, EFI_TPL notify_tpl, EFI_EVENT_NOTIFY notify_func, void *notify_ctx, EFI_EVENT *event);
	EFI_STATUS (*set_timer) (EFI_EVENT event, EFI_TIMER_DELAY type, uint64_t trigger_time);
	EFI_STATUS (*wait_for_event) (UINTN num_events, EFI_EVENT *event, UINTN *index);
	void *signal_event;
	EFI_STATUS (*close_event) (EFI_EVENT event);
};

void mock_btsrv_init(struct mock_btsrv *);

#endif // BOOTSRV_H
