#include "output.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <windows.h>

EFI_STATUS efi_txt_output_str(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this, wchar_t *str) {
	HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleW(hndl, str, wcslen(str), NULL, NULL);
	return EFI_SUCCESS;
}

EFI_STATUS efi_txt_query_mode(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this, UINTN mode_number, UINTN *columns, UINTN *rows) {
	HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO psbi;
	GetConsoleScreenBufferInfo(hndl, &psbi);
	*columns = psbi.dwSize.X;
	*rows = psbi.dwSize.Y;
	return EFI_SUCCESS;
}

EFI_STATUS efi_txt_set_mode(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this, UINTN mode_number) {
	return EFI_SUCCESS;
}

EFI_STATUS efi_txt_set_attr(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this, UINTN attr) {
	WORD fg = (WORD)(attr & 0x0F);
	WORD bg = (WORD)((attr >> 4) & 0x0F);
	WORD wfg = 0, wbg = 0;

	if (fg & 0x1) wfg |= FOREGROUND_BLUE;
	if (fg & 0x2) wfg |= FOREGROUND_GREEN;
	if (fg & 0x4) wfg |= FOREGROUND_RED;
	if (fg & 0x8) wfg |= FOREGROUND_INTENSITY;

	if (bg & 0x1) wbg |= BACKGROUND_BLUE;
	if (bg & 0x2) wbg |= BACKGROUND_GREEN;
	if (bg & 0x4) wbg |= BACKGROUND_RED;
	if (bg & 0x8) wbg |= BACKGROUND_INTENSITY;

	HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hndl, (WORD)(wfg | wbg));

	return EFI_SUCCESS;
}

EFI_STATUS efi_txt_set_cursor_pos(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this, UINTN column, UINTN row) {
	COORD coord;
	coord.X = column;
	coord.Y = row;
	HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hndl, coord);
	return EFI_SUCCESS;
}

EFI_STATUS clear_screen(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this) {
	UINTN columns;
	UINTN rows;
	efi_txt_query_mode(this, 0, &columns, &rows);
	UINTN str_len = columns * rows;
	efi_txt_set_cursor_pos(this, 0, 0);
	wchar_t empy_str[str_len + 1];
	wmemset(empy_str, L' ', str_len);
	empy_str[str_len] = L'\0';
	efi_txt_output_str(this, empy_str);
	efi_txt_set_cursor_pos(this, 0, 0);
	return EFI_SUCCESS;
}

void efi_txt_output_init(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *output) {
	memset(output, 0, sizeof(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL));
	output->OutputString = efi_txt_output_str;
	output->QueryMode = efi_txt_query_mode;
	output->SetMode = efi_txt_set_mode;
	output->SetAttribute = efi_txt_set_attr;
	output->ClearScreen = clear_screen;
	output->SetCursorPosition = efi_txt_set_cursor_pos;
}
