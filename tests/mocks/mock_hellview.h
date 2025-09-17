#ifndef MOCK_HELLVIEW_H
#define MOCK_HELLVIEW_H

#include <inttypes.h>

struct mock_hellview {
	void *vptr;
	int draw_pending;
	uint64_t seconds;

	int draw_pending_called;

	int draw_called;

	int loaded_called;

	int upd_sec_called;
};

struct mock_hellview_vtable {
	int (*draw_pending)(struct mock_hellview *);
	void (*draw)(struct mock_hellview *);
	void (*loaded)(struct mock_hellview *);
	void (*upd_sec)(struct mock_hellview *, uint64_t);
};

extern const struct mock_hellview_vtable mock_hellview_vtable;

void mock_hellview_init(struct mock_hellview *);

#endif // MOCK_HELLVIEW_H
