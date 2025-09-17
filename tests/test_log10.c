#include "test_utils.h"
#include <math.h>

extern double _log10(double arg);

void test_log10() {
	int success = 1;

	for (int i = 1; success && i <= 1000; ++i)
		success = fabs(_log10(i) - log10(i)) < .0000001;

	for (int i = 999999999; success && i <= 1000000100; ++i)
		success = fabs(_log10(i) - log10(i)) < .0000001;

	if (success)
		success = -INFINITY == _log10(0);

	ASSERT(success);
}
