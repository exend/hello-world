#include "mock_hellview.h"
#include <string.h>

static int draw_pending(struct mock_hellview *hellview) {
	++hellview->draw_pending_called;
	return hellview->draw_pending;
}

static void draw(struct mock_hellview *hellview) {
	++hellview->draw_called;
}

static void loaded(struct mock_hellview *hellview) {
	++hellview->loaded_called;
}

static void upd_sec(struct mock_hellview *hellview, uint64_t sec) {
	hellview->seconds = sec;
	++hellview->upd_sec_called;
}

const struct mock_hellview_vtable mock_hellview_vtable = {
	.draw_pending = draw_pending,
	.draw = draw,
	.loaded = loaded,
	.upd_sec = upd_sec
};

void mock_hellview_init(struct mock_hellview *hellview) {
	memset(hellview, 0, sizeof(struct mock_hellview));
	hellview->vptr = (void *)&mock_hellview_vtable;
}
