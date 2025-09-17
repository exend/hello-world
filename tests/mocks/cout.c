#include "cout.h"
#include "status.h"
#include <string.h>

static const UINTN mock_cout_cols = 42;
static const UINTN mock_cout_rows = 24;

static EFI_STATUS mock_cout_output_str(struct mock_cout *this, wchar_t *str) {
	this->str = str;
	++this->output_str_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_cout_query_mode(struct mock_cout *this, UINTN mode_num, UINTN *columns, UINTN *rows) {
	*columns = mock_cout_cols;
	*rows = mock_cout_rows;
	this->mode_num = mode_num;
	this->columns = columns;
	this->rows = rows;
	++this->query_mode_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_cout_set_mode(struct mock_cout *this, UINTN mode_num) {
	this->mode_num = mode_num;
	++this->set_mode_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_cout_set_attr(struct mock_cout *this, UINTN attr) {
	this->attr = attr;
	++this->set_attr_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_cout_clear_screen(struct mock_cout *this) {
	++this->clear_screen_called;
	return EFI_SUCCESS;
}

static EFI_STATUS mock_cout_set_cursor_pos(struct mock_cout *this, UINTN column, UINTN row) {
	this->column = column;
	this->row = row;
	++this->set_cursor_pos_called;
	return EFI_SUCCESS;
}

void mock_cout_init(struct mock_cout *cout) {
	memset(cout, 0, sizeof(struct mock_cout));
	cout->output_str = mock_cout_output_str;
	cout->query_mode = mock_cout_query_mode;
	cout->set_mode = mock_cout_set_mode;
	cout->set_attr = mock_cout_set_attr;
	cout->clear_screen = mock_cout_clear_screen;
	cout->set_cursor_pos = mock_cout_set_cursor_pos;
}
