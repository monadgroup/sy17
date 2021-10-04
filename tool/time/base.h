/**
* Copyright (C) 2007 Contributors
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SYNC_BASE_H
#define SYNC_BASE_H

#ifdef _MSC_VER
 #define _CRT_SECURE_NO_WARNINGS 1
 #define _CRT_NONSTDC_NO_DEPRECATE 1
#endif

#include <stddef.h>

/* configure inline keyword */
#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) && !defined(__cplusplus)
 #if defined(_MSC_VER) || defined(__GNUC__) || defined(__SASC)
  #define inline __inline
 #else
  /* compiler does not support inline */
  #define inline
 #endif
#endif

/* configure lacking CRT features */
#ifdef _MSC_VER
 #if _MSC_VER < 1900
  #define snprintf _snprintf
 #endif
 /* int is 32-bit for both x86 and x64 */
 typedef unsigned int uint32_t;
 #define UINT32_MAX UINT_MAX
#elif defined(__GNUC__)
 #include <stdint.h>
#elif defined(M68000)
 typedef unsigned int uint32_t;
#endif

#endif /* SYNC_BASE_H */
