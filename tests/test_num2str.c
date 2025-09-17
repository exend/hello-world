#include "test_utils.h"
#include <inttypes.h>
#include <wchar.h>

extern size_t _num2str(uint64_t num, size_t buff_size, wchar_t *buff);

void test_num2str() {
	wchar_t buff[21];
	size_t num = _num2str(1234567890, 21, buff);
	int success = num == 10 && !wcscmp(L"1234567890", buff);
	
	num = _num2str(42, 3, buff);
	success = success && num == 2 && !wcscmp(L"42", buff);

	num = _num2str(0, 21, buff);
	success = success && num == 1 && !wcscmp(L"0", buff);

	wchar_t teststr[] = L"test";
	wcscpy(buff, teststr);
	num = _num2str(42, 0, buff);
	success = success && num == 0 && !wcscmp(teststr, buff);

	ASSERT(success);
}
