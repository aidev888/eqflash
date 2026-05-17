#pragma once
#include <stddef.h>
#include <features.h>

#ifndef __ASSEMBLY__

void *memcpy (void *__restrict __dest, const void *__restrict __src, size_t __n) __THROW __nonnull ((1, 2));

void *memmove (void *__dest, const void *__src, size_t __n) __THROW __nonnull ((1, 2));

void *memset (void *__s, int __c, size_t __n) __THROW __nonnull ((1));

int memcmp (const void *__s1, const void *__s2, size_t __n) __THROW __attribute_pure__ __nonnull ((1, 2));

char *strcpy (char *__restrict __dest, const char *__restrict __src) __THROW __nonnull ((1, 2));

char *strncpy (char *__restrict __dest, const char *__restrict __src, size_t __n)
	__THROW __nonnull ((1, 2));

char *strcat (char *__restrict __dest, const char *__restrict __src)
	__THROW __nonnull ((1, 2));
char *strncat (char *__restrict __dest, const char *__restrict __src, size_t __n)
	__THROW __nonnull ((1, 2));

int strcmp (const char *__s1, const char *__s2)
	__THROW __attribute_pure__ __nonnull ((1, 2));
int strncmp (const char *__s1, const char *__s2, size_t __n)
	__THROW __attribute_pure__ __nonnull ((1, 2));

size_t strlcpy (char *__restrict __dest,
		const char *__restrict __src, size_t __n)
	__THROW __nonnull ((1, 2)) __attr_access ((__write_only__, 1, 3));

size_t strlcat (char *__restrict __dest,
		const char *__restrict __src, size_t __n)
	__THROW __nonnull ((1, 2))  __attr_access ((__read_write__, 1, 3));

#endif
