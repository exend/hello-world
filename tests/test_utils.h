#include <stdio.h>

#define PASSED_MSG() \
	printf("✅ %s passed.\n", __func__)

#define FAILED_MSG() \
	printf("❌ %s failed.\n", __func__)

#define ASSERT(arg) \
	do { if((arg)) PASSED_MSG(); else FAILED_MSG(); } while(0)
