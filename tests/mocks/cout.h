#ifndef COUT_H
#define COUT_H

#include <wchar.h>
#include <efi/types.h>
#include <efi/status.h>

extern const UINTN mock_cout_cols;
extern const UINTN mock_cout_rows;

struct mock_cout {
	void *reset;
	EFI_STATUS (*output_str) (struct mock_cout *this, wchar_t *str);
	void *test_str;
	EFI_STATUS (*query_mode) (struct mock_cout *this, UINTN mode_num, UINTN *columns, UINTN *rows);
	EFI_STATUS (*set_mode) (struct mock_cout *this, UINTN mode_num);
	EFI_STATUS (*set_attr) (struct mock_cout *this, UINTN attr);
	EFI_STATUS (*clear_screen) (struct mock_cout *this);
	EFI_STATUS (*set_cursor_pos) (struct mock_cout *this, UINTN column, UINTN row);

	wchar_t *str;
	int output_str_called;

	UINTN mode_num;
	UINTN *columns;
	UINTN *rows;
	int query_mode_called;
	
	int set_mode_called;

	UINTN attr;
	int set_attr_called;
	
	int clear_screen_called;

	UINTN column;
	UINTN row;
	int set_cursor_pos_called;
};

void mock_cout_init(struct mock_cout *cout);

#endif // COUT_H
