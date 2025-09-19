#ifndef TPL_H
#define TPL_H

#include "types.h"

//******************************************************
// EFI_TPL
//******************************************************
typedef UINTN EFI_TPL;

//******************************************************
// Task Priority Levels
//******************************************************
#define TPL_APPLICATION    4
#define TPL_CALLBACK       8
#define TPL_NOTIFY         16
#define TPL_HIGH_LEVEL     31

#endif // TPL_H
