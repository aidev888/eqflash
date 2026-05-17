#pragma once

#ifndef __ASSEMBLY__
#define NULL ((void *)0)

typedef unsigned long size_t;
typedef long int ssize_t;
#endif

#define offsetof(TYPE, MEMBER)	__builtin_offsetof(TYPE, MEMBER)
