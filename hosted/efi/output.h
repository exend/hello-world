#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdbool.h>
#include <wchar.h>
#include <efi/types.h>
#include <efi/status.h>

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (*EFI_TEXT_STRING) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, wchar_t *String);
typedef EFI_STATUS (*EFI_TEXT_TEST_STRING) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, wchar_t *String);
typedef EFI_STATUS (*EFI_TEXT_QUERY_MODE) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber, UINTN *Columns, UINTN *Rows);
typedef EFI_STATUS (*EFI_TEXT_SET_MODE) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN ModeNumber);
typedef EFI_STATUS (*EFI_TEXT_SET_ATTRIBUTE) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Attribute);
typedef EFI_STATUS (*EFI_TEXT_CLEAR_SCREEN) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
typedef EFI_STATUS (*EFI_TEXT_SET_CURSOR_POSITION) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINTN Column, UINTN Row);
typedef EFI_STATUS (*EFI_TEXT_ENABLE_CURSOR) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, bool Visible);

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	void                                     *Reset;
	EFI_TEXT_STRING                          OutputString;
	EFI_TEXT_TEST_STRING                     TestString;
	EFI_TEXT_QUERY_MODE                      QueryMode;
	EFI_TEXT_SET_MODE                        SetMode;
	EFI_TEXT_SET_ATTRIBUTE                   SetAttribute;
	EFI_TEXT_CLEAR_SCREEN                    ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION             SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR                   EnableCursor;
	void                                     *Mode;
};

void efi_txt_output_init(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *output);

#endif // OUTPUT_H
