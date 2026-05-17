#pragma once

#ifndef __ASSEMBLY__
#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L
/* bool, true and false are keywords.  */
#else
typedef _Bool	bool;
#define true	((_Bool)1)
#define false	((_Bool)0)
#endif

#endif /*  __ASSEMBLY__ */
