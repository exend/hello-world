#include "test_utils.h"
#include <math.h>

extern double _log2(double arg);

void test_log2() {
	int success = 1;

	for (int i = 1; i <= 1000 && success; ++i)
		success = fabs(_log2(i) - log2(i)) < .0000001;

	for (int i = 999999999; i <= 1000000100 && success; ++i)
		success = fabs(_log2(i) - log2(i)) < .0000001;

	ASSERT(success);
}
