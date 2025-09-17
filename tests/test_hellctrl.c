#include "test_utils.h"
#include "mocks/types.h"
#include "mocks/tpl.h"
#include "mocks/evttypes.h"
#include "mocks/timerdelay.h"
#include "mocks/status.h"
#include "mocks/rtsrv.h"
#include "mocks/bootsrv.h"
#include "mocks/mock_hellview.h"

struct hellctrl;

struct hellctrl_vtable {
	void (*set_view)(struct hellctrl *, struct mock_hellview *);
	void (*view_loaded)(struct hellctrl *);
};

struct hellctrl {
	struct hellctrl_vtable *vptr;
	struct mock_hellview *hellview_ptr;
	struct mock_rtsrv *rtsrv_ptr;
	struct mock_btsrv *btsrv_ptr;
	EFI_EVENT event_ptr;
	uint64_t sec_left;
};

extern void _hellctrl_init(
	struct hellctrl *,
	struct mock_rtsrv *,
	struct mock_btsrv *
);

extern void _hellctrl_deinit(struct hellctrl *);

void test_hellctrl_init() {
	struct hellctrl hellctrl;
	struct mock_rtsrv rtsrv;
	struct mock_btsrv btsrv;

	mock_rtsrv_init(&rtsrv);
	mock_btsrv_init(&btsrv);
	_hellctrl_init(&hellctrl, &rtsrv, &btsrv);

	int success = !!hellctrl.vptr
		&& hellctrl.rtsrv_ptr == &rtsrv
		&& hellctrl.btsrv_ptr == &btsrv
		&& hellctrl.sec_left == 0x10;

	ASSERT(success);
}

void test_hellctrl_deinit() {
	struct hellctrl hellctrl;
	struct mock_rtsrv rtsrv;
	struct mock_btsrv btsrv;
	struct mock_hellview hellview;

	mock_rtsrv_init(&rtsrv);
	mock_btsrv_init(&btsrv);
	mock_hellview_init(&hellview);
	_hellctrl_init(&hellctrl, &rtsrv, &btsrv);
	
	hellctrl.vptr->set_view(&hellctrl, &hellview);
	hellctrl.vptr->view_loaded(&hellctrl);

	mock_event.close_event_called = 0;
	_hellctrl_deinit(&hellctrl);
	int success = mock_event.close_event_called == 1 && !hellctrl.event_ptr;
	
	mock_event.close_event_called = 0;
	_hellctrl_deinit(&hellctrl);
	success = success && !mock_event.close_event_called;

	ASSERT(success);
}

void test_hellctrl_set_view() {
	struct hellctrl hellctrl;
	struct mock_rtsrv rtsrv;
	struct mock_btsrv btsrv;
	struct mock_hellview hellview;

	mock_rtsrv_init(&rtsrv);
	mock_btsrv_init(&btsrv);
	mock_hellview_init(&hellview);
	_hellctrl_init(&hellctrl, &rtsrv, &btsrv);
	
	hellctrl.vptr->set_view(&hellctrl, &hellview);

	ASSERT(hellctrl.hellview_ptr == &hellview);
}

void test_hellctrl_view_loaded() {
	struct hellctrl hellctrl;
	struct mock_rtsrv rtsrv;
	struct mock_btsrv btsrv;
	struct mock_hellview hellview;

	mock_rtsrv_init(&rtsrv);
	mock_btsrv_init(&btsrv);
	mock_hellview_init(&hellview);
	_hellctrl_init(&hellctrl, &rtsrv, &btsrv);
	
	hellctrl.vptr->set_view(&hellctrl, &hellview);
	hellctrl.vptr->view_loaded(&hellctrl);
	
	uint64_t delay = hellctrl.sec_left + 1;
	if (mock_event.notify_func)
		for (int i = delay + 10; i && !mock_event.close_event_called; --i)
			mock_event.notify_func(&mock_event, mock_event.notify_ctx);

	int success = mock_event.set_timer_called == 1
		&& mock_event.close_event_called == 1
		&& rtsrv_reset_data.reset_type == EFI_RESET_SHUTDOWN
		&& rtsrv_reset_data.reset_status == EFI_SUCCESS
		&& rtsrv_reset_data.func_called == 1
		&& hellview.upd_sec_called == delay;

	ASSERT(success);
}

void test_hellctrl() {
	test_hellctrl_init();
	test_hellctrl_deinit();
	test_hellctrl_set_view();
	test_hellctrl_view_loaded();
}
