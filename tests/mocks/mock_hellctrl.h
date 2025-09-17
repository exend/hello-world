#ifndef MOCK_HELLCTRL_H
#define MOCK_HELLCTRL_H

struct hellview;

struct mock_hellctrl {
	void *vptr;
	struct hellview *hellview_ptr;
	int set_view_called;
	int view_loaded_called;
};

void mock_hellctrl_init(struct mock_hellctrl *);

#endif // MOCK_HELLCTRL_H
