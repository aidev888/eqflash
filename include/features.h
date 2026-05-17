#pragma once

#define __THROW			__attribute__ ((__nothrow__, __leaf__))
#define __nonnull(params)	__attribute__ ((__nonnull__ params))
#define __attribute_pure__	__attribute__ ((__pure__))

#if __has_attribute(__access__)
#define __attr_access(x) __attribute__ ((__access__ x))
#else
#define __attr_access(x)
#endif
