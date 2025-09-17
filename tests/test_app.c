#include "test_utils.h"
#include "mocks/evttypes.h"
#include "mocks/mock_hellview.h"
#include "mocks/bootsrv.h"

struct app {
	struct mock_hellview *view_ptr;
	struct mock_btsrv *btsrv_ptr;
	EFI_EVENT event;
	int64_t exit;
};

extern void _app_init(
	struct app *,
	struct mock_hellview *,
	struct mock_btsrv *
);

extern void _app_deinit(struct app *);

extern void _app_exit(struct app *);

extern void _app_run_loop(struct app *);


static struct app *app_ptr = 0;
static int counter = 0;

static struct subview_vtable {
	int (*draw_pending)(struct mock_hellview *);
	void (*draw)(struct mock_hellview *);
	void (*loaded)(struct mock_hellview *);
	void (*upd_sec)(struct mock_hellview *, uint64_t);
} subview_vtable;

static int subview_draw_pending(struct mock_hellview *hellview) {
	mock_hellview_vtable.draw_pending(hellview);
	return counter-- % 2;
}

static void subview_draw(struct mock_hellview *hellview) {
	mock_hellview_vtable.draw(hellview);
	if (app_ptr && !counter)
		_app_exit(app_ptr);
}

void subview_init(struct mock_hellview *hellview) {
	mock_hellview_init(hellview);
	subview_vtable = *(struct subview_vtable *)hellview->vptr;
	subview_vtable.draw_pending = subview_draw_pending;
	subview_vtable.draw = subview_draw;
	hellview->vptr = &subview_vtable;
}


void test_app_init() {
	struct mock_hellview hellview;
	struct mock_btsrv btsrv;
	struct app app;

	mock_hellview_init(&hellview);
	mock_btsrv_init(&btsrv);
	_app_init(&app, &hellview, &btsrv);

	int success = app.view_ptr == &hellview
		&& app.btsrv_ptr == &btsrv
		&& !app.event
		&& !app.exit;

	ASSERT(success);
}

void test_app_deinit() {
	int success;
	struct mock_hellview hellview;
	struct mock_btsrv btsrv;
	struct app app;

	mock_hellview_init(&hellview);
	mock_btsrv_init(&btsrv);
	_app_init(&app, &hellview, &btsrv);

	mock_event.close_event_called = 0;
	app.event = &mock_event;
	_app_deinit(&app);
	success = mock_event.close_event_called == 1 && !app.event;

	_app_deinit(&app);
	success = success && mock_event.close_event_called == 1;

	ASSERT(success);
}

void test_app_exit() {
	struct mock_hellview hellview;
	struct mock_btsrv btsrv;
	struct app app;

	mock_hellview_init(&hellview);
	mock_btsrv_init(&btsrv);
	_app_init(&app, &hellview, &btsrv);

	_app_exit(&app);

	ASSERT(app.exit);
}

void test_app_run_loop() {
	struct mock_hellview hellview;
	struct mock_btsrv btsrv;
	struct app app;

	subview_init(&hellview);
	mock_btsrv_init(&btsrv);
	_app_init(&app, &hellview, &btsrv);

	counter = 20;
	app_ptr = &app;
	_app_run_loop(&app);

	int success = hellview.loaded_called == 1
		&& mock_event.evt_type == EVT_TIMER
		&& mock_event.notify_tpl == TPL_APPLICATION
		&& mock_event.create_event_called == 1
		&& mock_event.delay_type == TIMER_PERIODIC
		&& mock_event.trigger_time == 500000
		&& mock_event.set_timer_called == 1
		&& hellview.draw_pending_called == 20
		&& hellview.draw_called == 10
		&& mock_event.wait_for_event_called == 20;

	ASSERT(success);
}

void test_app() {
	test_app_init();
	test_app_deinit();
	test_app_exit();
	test_app_run_loop();
}
