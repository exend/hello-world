#include "mock_hellctrl.h"
#include <string.h>

static void set_view(struct mock_hellctrl *hellctrl, struct hellview *hellview) {
	hellctrl->hellview_ptr = hellview;
	++hellctrl->set_view_called;
}

static void view_loaded(struct mock_hellctrl *hellctrl) {
	++hellctrl->view_loaded_called;
}

static struct {
	void (*set_view)(struct mock_hellctrl *, struct hellview *);
	void (*view_loaded)(struct mock_hellctrl *);
} hellctrl_vtable = {
	.set_view = set_view,
	.view_loaded = view_loaded
};


void mock_hellctrl_init(struct mock_hellctrl *hellctrl) {
	memset(hellctrl, 0, sizeof(struct mock_hellctrl));
	hellctrl->vptr = &hellctrl_vtable;
}
