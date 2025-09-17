#include "rtsrv.h"
#include <string.h>

struct rtsrv_reset_data rtsrv_reset_data;

static void mock_rtsrv_reset(
	EFI_RESET_TYPE reset_type,
	EFI_STATUS reset_status,
	UINTN data_size,
	void *reset_data
) {
	rtsrv_reset_data.reset_type = reset_type;
	rtsrv_reset_data.reset_status = reset_status;
	rtsrv_reset_data.data_size = data_size;
	rtsrv_reset_data.reset_data = reset_data;
	++rtsrv_reset_data.func_called;
}

void mock_rtsrv_init(struct mock_rtsrv *rtsrv) {
	memset(rtsrv, 0, sizeof(struct mock_rtsrv));
	memset(&rtsrv_reset_data, 0, sizeof(struct rtsrv_reset_data));
	rtsrv->reset_sys = mock_rtsrv_reset;
}
