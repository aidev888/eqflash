#pragma once
#include <limits.h>


#ifndef __ASSEMBLY__
typedef unsigned char uint8_t, u_int8_t;
typedef char int8_t;

typedef unsigned short uint16_t, u_int16_t;
typedef short int16_t;

typedef unsigned int uint32_t, u_int32_t;
typedef int int32_t;

typedef unsigned long long uint64_t, u_int64_t;
typedef long long int64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;
typedef long ptrdiff_t;

#endif /* __ASSEMBLY__ */

#define SIZE_MAX              (18446744073709551615UL)
#define SIZE_WIDTH		8
#define PTRDIFF_WIDTH		8
#define PTRDIFF_MAX		LONG_MAX
#define PTRDIFF_MIN		LONG_MIN
