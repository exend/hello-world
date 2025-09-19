#include "test_utils.h"
#include "mocks/cout.h"
#include "mocks/mock_hellctrl.h"
#include <efi/types.h>
#include <efi/status.h>
#include <inttypes.h>

struct hellview;

struct hellview_vtable {
	int (*draw_pending)(struct hellview *);
	void (*draw)(struct hellview *);
	void (*loaded)(struct hellview *);
	void (*upd_sec)(struct hellview *, uint64_t);
};

struct hellview {
	struct hellview_vtable *vptr;
	int64_t draw_pending;
	struct mock_hellctrl *hellctrl_ptr;
	uint64_t seconds;
	struct mock_cout *cout_ptr;
	uint64_t ccolumns;
	uint64_t crows;
};

extern void _hellview_init(
	struct hellview *,
	struct mock_hellctrl *,
	struct mock_cout *
);

extern void _hellview_deinit(struct hellview *);

void test_hellview_init() {
	struct mock_cout cout;
	struct mock_hellctrl hellctrl;
	struct hellview hellview;

	mock_cout_init(&cout);
	mock_hellctrl_init(&hellctrl);
	_hellview_init(&hellview, &hellctrl, &cout);

	int success = hellview.hellctrl_ptr == &hellctrl
		&& hellview.cout_ptr == &cout
		&& cout.mode_num == 0
		&& cout.set_mode_called
		&& cout.attr == 0x06
		&& cout.set_attr_called
		&& cout.query_mode_called
		&& hellview.ccolumns == mock_cout_cols
		&& hellview.crows == mock_cout_rows
		&& hellctrl.hellview_ptr == &hellview
		&& hellctrl.set_view_called == 1;

	ASSERT(success);
}

void test_hellview_draw_pending() {
	int success;
	struct mock_cout cout;
	struct mock_hellctrl hellctrl;
	struct hellview hellview;

	mock_cout_init(&cout);
	mock_hellctrl_init(&hellctrl);
	_hellview_init(&hellview, &hellctrl, &cout);

	hellview.draw_pending = 0;	
	success = !hellview.vptr->draw_pending(&hellview);

	hellview.draw_pending = 1;	
	success = success && hellview.vptr->draw_pending(&hellview);

	ASSERT(success);
}

void test_hellview_draw() {
	struct mock_cout cout;
	struct mock_hellctrl hellctrl;
	struct hellview hellview;

	mock_cout_init(&cout);
	mock_hellctrl_init(&hellctrl);
	_hellview_init(&hellview, &hellctrl, &cout);

	hellview.draw_pending = 1;
	hellview.vptr->draw(&hellview);

	int success = !hellview.draw_pending
		&& cout.clear_screen_called == 1
		&& cout.set_cursor_pos_called
		&& cout.output_str_called
		&& wcslen(cout.str);

	ASSERT(success);
}

void test_hellview_loaded() {
	struct mock_cout cout;
	struct mock_hellctrl hellctrl;
	struct hellview hellview;

	mock_cout_init(&cout);
	mock_hellctrl_init(&hellctrl);
	_hellview_init(&hellview, &hellctrl, &cout);

	hellview.vptr->loaded(&hellview);

	ASSERT(hellctrl.view_loaded_called);
}

void test_hellview_upd_sec() {
	struct mock_cout cout;
	struct mock_hellctrl hellctrl;
	struct hellview hellview;

	mock_cout_init(&cout);
	mock_hellctrl_init(&hellctrl);
	_hellview_init(&hellview, &hellctrl, &cout);

	hellview.draw_pending = 0;
	hellview.vptr->upd_sec(&hellview, 42);

	int success = hellview.draw_pending
		&& hellview.seconds == 42;

	ASSERT(success);
}

void test_hellview() {
	test_hellview_init();
	test_hellview_draw_pending();
	test_hellview_draw();
	test_hellview_loaded();
	test_hellview_upd_sec();
}
