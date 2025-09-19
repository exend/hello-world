#ifndef RTSRV_H
#define RTSRV_H

#include <efi/types.h>
#include <efi/status.h>
#include <efi/resettype.h>
#include <efi/tableheader.h>

struct rtsrv_reset_data {
	EFI_RESET_TYPE reset_type;
	EFI_STATUS reset_status;
	UINTN data_size;
	void *reset_data;
	int func_called;
};

extern struct rtsrv_reset_data rtsrv_reset_data;

struct mock_rtsrv {
	EFI_TABLE_HEADER hdr;
	void *padding[10];
	void (*reset_sys)(EFI_RESET_TYPE reset_type, EFI_STATUS reset_status, UINTN data_size, void *reset_data);
};

void mock_rtsrv_init(struct mock_rtsrv *);

#endif // RTSRV_H
