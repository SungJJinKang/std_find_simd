#pragma once
/* Builtins and Intrinsics
 * Portable Snippets - https://github.com/nemequ/portable-snippets
 * Created by Evan Nemerson <evan@nemerson.com>
 *
 *   To the extent possible under law, the authors have waived all
 *   copyright and related or neighboring rights to this code.  For
 *   details, see the Creative Commons Zero 1.0 Universal license at
 *   https://creativecommons.org/publicdomain/zero/1.0/
 *
 * Some of these implementations are based on code from
 * https://graphics.stanford.edu/~seander/bithacks.html which is also
 * public domain (and a fantastic web site).
 */

#if !defined(PSNIP_BUILTIN_H)
#define PSNIP_BUILTIN_H

#if defined(HEDLEY_GCC_HAS_BUILTIN)
#  define PSNIP_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) HEDLEY_GCC_HAS_BUILTIN(builtin,major,minor,0)
#elif defined(__clang__) && defined(__has_builtin)
#  define PSNIP_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) __has_builtin(builtin)
#elif defined(__GNUC__)
#  define PSNIP_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) (__GNUC__ > major || (major == __GNUC__ && __GNUC_MINOR__ >= minor))
#else
#  define PSNIP_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) (0)
#endif

#if defined(HEDLEY_CLANG_HAS_BUILTIN)
#  define PSNIP_BUILTIN_CLANG_HAS_BUILTIN(builtin) HEDLEY_CLANG_HAS_BUILTIN(builtin)
#elif defined(__has_builtin)
#  define PSNIP_BUILTIN_CLANG_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#  define PSNIP_BUILTIN_CLANG_HAS_BUILTIN(builtin) (0)
#endif

#if defined(HEDLEY_MSVC_VERSION_CHECK)
#  define PSNIP_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) HEDLEY_MSVC_VERSION_CHECK(major,minor,0)
#elif !defined(_MSC_VER)
#  define PSNIP_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define PSNIP_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) (_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define PSNIP_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) (_MSC_FULL_VER >= ((major * 100000) + (minor * 1000)))
#else
#  define PSNIP_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) (_MSC_VER >= ((major * 100) + (minor)))
#endif

#if defined(_MSC_VER)
#  include <intrin.h>
#endif
#include <limits.h>
#include <stdlib.h>

#if defined(__i386) || defined(_M_IX86) || \
  defined(__amd64) || defined(_M_AMD64) || defined(__x86_64)
#  if defined(_MSC_VER)
#    define PSNIP_BUILTIN__ENABLE_X86
#  elif defined(__GNUC__)
#    define PSNIP_BUILTIN__ENABLE_X86
#    include <x86intrin.h>
#  endif
#endif

#if defined(__amd64) || defined(_M_AMD64) || defined(__x86_64)
#  if defined(_MSC_VER)
#    define PSNIP_BUILTIN__ENABLE_AMD64
#  elif defined(__GNUC__)
#    define PSNIP_BUILTIN__ENABLE_AMD64
#    include <x86intrin.h>
#  endif
#endif

#if defined(__ARM_ACLE)
#  include <arm_acle.h>
#endif

 /* For maximum portability include the exact-int module from
    portable snippets. */
#if \
  !defined(psnip_int64_t) || !defined(psnip_uint64_t) || \
  !defined(psnip_int32_t) || !defined(psnip_uint32_t) || \
  !defined(psnip_int16_t) || !defined(psnip_uint16_t) || \
  !defined(psnip_int8_t)  || !defined(psnip_uint8_t)
#  include <stdint.h>
#  if !defined(psnip_int64_t)
#    define psnip_int64_t int64_t
#  endif
#  if !defined(psnip_uint64_t)
#    define psnip_uint64_t uint64_t
#  endif
#  if !defined(psnip_int32_t)
#    define psnip_int32_t int32_t
#  endif
#  if !defined(psnip_uint32_t)
#    define psnip_uint32_t uint32_t
#  endif
#  if !defined(psnip_int16_t)
#    define psnip_int16_t int16_t
#  endif
#  if !defined(psnip_uint16_t)
#    define psnip_uint16_t uint16_t
#  endif
#  if !defined(psnip_int8_t)
#    define psnip_int8_t int8_t
#  endif
#  if !defined(psnip_uint8_t)
#    define psnip_uint8_t uint8_t
#  endif
#endif

#if defined(HEDLEY_LIKELY) && defined(HEDLEY_UNLIKELY)
#  define PSNIP_BUILTIN_LIKELY(expr) HEDLEY_LIKELY(expr)
#  define PSNIP_BUILTIN_UNLIKELY(expr) HEDLEY_UNLIKELY(expr)
#elif PSNIP_BUILTIN_GNU_HAS_BUILTIN(__builtin_expect,3,0)
#  define PSNIP_BUILTIN_LIKELY(expr) __builtin_expect(!!(expr), 1)
#  define PSNIP_BUILTIN_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#  define PSNIP_BUILTIN_LIKELY(expr) (!!(expr))
#  define PSNIP_BUILTIN_UNLIKELY(expr) (!!(expr))
#endif

#if !defined(PSNIP_BUILTIN_STATIC_INLINE)
#  if defined(__GNUC__)
#    define PSNIP_BUILTIN__COMPILER_ATTRIBUTES __attribute__((__unused__))
#  else
#    define PSNIP_BUILTIN__COMPILER_ATTRIBUTES
#  endif

#  if defined(HEDLEY_INLINE)
#    define PSNIP_BUILTIN__INLINE HEDLEY_INLINE
#  elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    define PSNIP_BUILTIN__INLINE inline
#  elif defined(__GNUC_STDC_INLINE__)
#    define PSNIP_BUILTIN__INLINE __inline__
#  elif defined(_MSC_VER) && _MSC_VER >= 1200
#    define PSNIP_BUILTIN__INLINE __inline
#  else
#    define PSNIP_BUILTIN__INLINE
#  endif

#  define PSNIP_BUILTIN__FUNCTION PSNIP_BUILTIN__COMPILER_ATTRIBUTES static PSNIP_BUILTIN__INLINE
#endif

#define PSNIP_BUILTIN__SUFFIX_B  1
#define PSNIP_BUILTIN__SUFFIX_S  2
#define PSNIP_BUILTIN__SUFFIX_   3
#define PSNIP_BUILTIN__SUFFIX_L  4
#define PSNIP_BUILTIN__SUFFIX_LL 5

#if !defined(PSNIP_BUILTIN__SIZEOF_CHAR)
#  if   CHAR_MIN == (-0x7fLL-1) && CHAR_MAX == 0x7fLL
#    define PSNIP_BUILTIN__SIZEOF_CHAR 8
#  elif CHAR_MIN == (-0x7fffLL-1) && CHAR_MAX == 0x7fffLL
#    define PSNIP_BUILTIN__SIZEOF_CHAR 16
#  elif CHAR_MIN == (-0x7fffffffLL-1) && CHAR_MAX == 0x7fffffffLL
#    define PSNIP_BUILTIN__SIZEOF_CHAR 32
#  elif CHAR_MIN == (-0x7fffffffffffffffLL-1) && CHAR_MAX == 0x7fffffffffffffffLL
#    define PSNIP_BUILTIN__SIZEOF_CHAR 64
#  endif
#endif

#if !defined(PSNIP_BUILTIN__SIZEOF_SHRT)
#  if   SHRT_MIN == (-0x7fLL-1) && SHRT_MAX == 0x7fLL
#    define PSNIP_BUILTIN__SIZEOF_SHRT 8
#  elif SHRT_MIN == (-0x7fffLL-1) && SHRT_MAX == 0x7fffLL
#    define PSNIP_BUILTIN__SIZEOF_SHRT 16
#  elif SHRT_MIN == (-0x7fffffffLL-1) && SHRT_MAX == 0x7fffffffLL
#    define PSNIP_BUILTIN__SIZEOF_SHRT 32
#  elif SHRT_MIN == (-0x7fffffffffffffffLL-1) && SHRT_MAX == 0x7fffffffffffffffLL
#    define PSNIP_BUILTIN__SIZEOF_SHRT 64
#  endif
#endif

#if !defined(PSNIP_BUILTIN__SIZEOF_INT)
#  if   INT_MIN == (-0x7fLL-1) && INT_MAX == 0x7fLL
#    define PSNIP_BUILTIN__SIZEOF_INT 8
#  elif INT_MIN == (-0x7fffLL-1) && INT_MAX == 0x7fffLL
#    define PSNIP_BUILTIN__SIZEOF_INT 16
#  elif INT_MIN == (-0x7fffffffLL-1) && INT_MAX == 0x7fffffffLL
#    define PSNIP_BUILTIN__SIZEOF_INT 32
#  elif INT_MIN == (-0x7fffffffffffffffLL-1) && INT_MAX == 0x7fffffffffffffffLL
#    define PSNIP_BUILTIN__SIZEOF_INT 64
#  endif
#endif

#if !defined(PSNIP_BUILTIN__SIZEOF_LONG)
#  if   LONG_MIN == (-0x7fLL-1) && LONG_MAX == 0x7fLL
#    define PSNIP_BUILTIN__SIZEOF_LONG 8
#  elif LONG_MIN == (-0x7fffLL-1) && LONG_MAX == 0x7fffLL
#    define PSNIP_BUILTIN__SIZEOF_LONG 16
#  elif LONG_MIN == (-0x7fffffffLL-1) && LONG_MAX == 0x7fffffffLL
#    define PSNIP_BUILTIN__SIZEOF_LONG 32
#  elif LONG_MIN == (-0x7fffffffffffffffLL-1) && LONG_MAX == 0x7fffffffffffffffLL
#    define PSNIP_BUILTIN__SIZEOF_LONG 64
#  endif
#endif

#if !defined(PSNIP_BUILTIN__SIZEOF_LLONG)
#  if   LLONG_MIN == (-0x7fLL-1) && LLONG_MAX == 0x7fLL
#    define PSNIP_BUILTIN__SIZEOF_LLONG 8
#  elif LLONG_MIN == (-0x7fffLL-1) && LLONG_MAX == 0x7fffLL
#    define PSNIP_BUILTIN__SIZEOF_LLONG 16
#  elif LLONG_MIN == (-0x7fffffffLL-1) && LLONG_MAX == 0x7fffffffLL
#    define PSNIP_BUILTIN__SIZEOF_LLONG 32
#  elif LLONG_MIN == (-0x7fffffffffffffffLL-1) && LLONG_MAX == 0x7fffffffffffffffLL
#    define PSNIP_BUILTIN__SIZEOF_LLONG 64
#  endif
#endif

#if !defined(PSNIP_BUILTIN_SUFFIX_INT8)
#  if CHAR_BIT == 8 || PSNIP_BUILTIN__SIZEOF_CHAR == 8
#    define PSNIP_BUILTIN_SUFFIX_INT8 PSNIP_BUILTIN__SUFFIX_B
#  elif PSNIP_BUILTIN__SIZEOF_SHRT == 8
#    define PSNIP_BUILTIN_SUFFIX_INT8 PSNIP_BUILTIN__SUFFIX_S
#  elif PSNIP_BUILTIN__SIZEOF_INT == 8
#    define PSNIP_BUILTIN_SUFFIX_INT8 PSNIP_BUILTIN__SUFFIX_
#  elif PSNIP_BUILTIN__SIZEOF_LONG == 8
#    define PSNIP_BUILTIN_SUFFIX_INT8 PSNIP_BUILTIN__SUFFIX_L
#  elif PSNIP_BUILTIN__SIZEOF_LLONG == 8
#    define PSNIP_BUILTIN_SUFFIX_INT8 PSNIP_BUILTIN__SUFFIX_LL
#  endif
#endif

#if !defined(PSNIP_BUILTIN_SUFFIX_INT16)
#  if PSNIP_BUILTIN__SIZEOF_CHAR == 16
#    define PSNIP_BUILTIN_SUFFIX_INT16 PSNIP_BUILTIN__SUFFIX_B
#  elif PSNIP_BUILTIN__SIZEOF_SHRT == 16
#    define PSNIP_BUILTIN_SUFFIX_INT16 PSNIP_BUILTIN__SUFFIX_S
#  elif PSNIP_BUILTIN__SIZEOF_INT == 16
#    define PSNIP_BUILTIN_SUFFIX_INT16 PSNIP_BUILTIN__SUFFIX_
#  elif PSNIP_BUILTIN__SIZEOF_LONG == 16
#    define PSNIP_BUILTIN_SUFFIX_INT16 PSNIP_BUILTIN__SUFFIX_L
#  elif PSNIP_BUILTIN__SIZEOF_LLONG == 16
#    define PSNIP_BUILTIN_SUFFIX_INT16 PSNIP_BUILTIN__SUFFIX_LL
#  endif
#endif

#if !defined(PSNIP_BUILTIN_SUFFIX_INT32)
#  if PSNIP_BUILTIN__SIZEOF_CHAR == 32
#    define PSNIP_BUILTIN_SUFFIX_INT32 PSNIP_BUILTIN__SUFFIX_B
#  elif PSNIP_BUILTIN__SIZEOF_SHRT == 32
#    define PSNIP_BUILTIN_SUFFIX_INT32 PSNIP_BUILTIN__SUFFIX_S
#  elif PSNIP_BUILTIN__SIZEOF_INT == 32
#    define PSNIP_BUILTIN_SUFFIX_INT32 PSNIP_BUILTIN__SUFFIX_
#  elif PSNIP_BUILTIN__SIZEOF_LONG == 32
#    define PSNIP_BUILTIN_SUFFIX_INT32 PSNIP_BUILTIN__SUFFIX_L
#  elif PSNIP_BUILTIN__SIZEOF_LLONG == 32
#    define PSNIP_BUILTIN_SUFFIX_INT32 PSNIP_BUILTIN__SUFFIX_LL
#  endif
#endif

#if !defined(PSNIP_BUILTIN_SUFFIX_INT64)
#  if defined(__APPLE__) && PSNIP_BUILTIN__SIZEOF_LLONG == 64
#    define PSNIP_BUILTIN_SUFFIX_INT64 PSNIP_BUILTIN__SUFFIX_LL
#  elif PSNIP_BUILTIN__SIZEOF_CHAR == 64
#    define PSNIP_BUILTIN_SUFFIX_INT64 PSNIP_BUILTIN__SUFFIX_B
#  elif PSNIP_BUILTIN__SIZEOF_SHRT == 64
#    define PSNIP_BUILTIN_SUFFIX_INT64 PSNIP_BUILTIN__SUFFIX_S
#  elif PSNIP_BUILTIN__SIZEOF_INT == 64
#    define PSNIP_BUILTIN_SUFFIX_INT64 PSNIP_BUILTIN__SUFFIX_
#  elif PSNIP_BUILTIN__SIZEOF_LONG == 64
#    define PSNIP_BUILTIN_SUFFIX_INT64 PSNIP_BUILTIN__SUFFIX_L
#  elif PSNIP_BUILTIN__SIZEOF_LLONG == 64
#    define PSNIP_BUILTIN_SUFFIX_INT64 PSNIP_BUILTIN__SUFFIX_LL
#  endif
#endif

#if defined(PSNIP_BUILTIN_SUFFIX_INT8)
#  if PSNIP_BUILTIN_SUFFIX_INT8 == 1
#    define PSNIP_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##b
#  elif PSNIP_BUILTIN_SUFFIX_INT8 == 2
#    define PSNIP_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##s
#  elif PSNIP_BUILTIN_SUFFIX_INT8 == 3
#    define PSNIP_BUILTIN__VARIANT_INT8(prefix,name)  prefix##_builtin_##name
#    define PSNIP_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name
#  elif PSNIP_BUILTIN_SUFFIX_INT8 == 4
#    define PSNIP_BUILTIN__VARIANT_INT8(prefix,name)  prefix##_builtin_##name##l
#    define PSNIP_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##l
#  elif PSNIP_BUILTIN_SUFFIX_INT8 == 5
#    define PSNIP_BUILTIN__VARIANT_INT8(prefix,name)  prefix##_builtin_##name##ll
#    define PSNIP_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

#if defined(PSNIP_BUILTIN_SUFFIX_INT16)
#  if PSNIP_BUILTIN_SUFFIX_INT16 == 1
#    define PSNIP_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##b
#  elif PSNIP_BUILTIN_SUFFIX_INT16 == 2
#    define PSNIP_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##s
#  elif PSNIP_BUILTIN_SUFFIX_INT16 == 3
#    define PSNIP_BUILTIN__VARIANT_INT16(prefix,name)  prefix##_builtin_##name
#    define PSNIP_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name
#  elif PSNIP_BUILTIN_SUFFIX_INT16 == 4
#    define PSNIP_BUILTIN__VARIANT_INT16(prefix,name)  prefix##_builtin_##name##l
#    define PSNIP_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##l
#  elif PSNIP_BUILTIN_SUFFIX_INT16 == 5
#    define PSNIP_BUILTIN__VARIANT_INT16(prefix,name)  prefix##_builtin_##name##ll
#    define PSNIP_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

#if defined(PSNIP_BUILTIN_SUFFIX_INT32)
#  if PSNIP_BUILTIN_SUFFIX_INT32 == 1
#    define PSNIP_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##b
#  elif PSNIP_BUILTIN_SUFFIX_INT32 == 2
#    define PSNIP_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##s
#  elif PSNIP_BUILTIN_SUFFIX_INT32 == 3
#    define PSNIP_BUILTIN__VARIANT_INT32(prefix,name)  prefix##_builtin_##name
#    define PSNIP_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name
#  elif PSNIP_BUILTIN_SUFFIX_INT32 == 4
#    define PSNIP_BUILTIN__VARIANT_INT32(prefix,name)  prefix##_builtin_##name##l
#    define PSNIP_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##l
#  elif PSNIP_BUILTIN_SUFFIX_INT32 == 5
#    define PSNIP_BUILTIN__VARIANT_INT32(prefix,name)  prefix##_builtin_##name##ll
#    define PSNIP_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

#if defined(PSNIP_BUILTIN_SUFFIX_INT64)
#  if PSNIP_BUILTIN_SUFFIX_INT64 == 1
#    define PSNIP_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##b
#  elif PSNIP_BUILTIN_SUFFIX_INT64 == 2
#    define PSNIP_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##s
#  elif PSNIP_BUILTIN_SUFFIX_INT64 == 3
#    define PSNIP_BUILTIN__VARIANT_INT64(prefix,name)  prefix##_builtin_##name
#    define PSNIP_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name
#  elif PSNIP_BUILTIN_SUFFIX_INT64 == 4
#    define PSNIP_BUILTIN__VARIANT_INT64(prefix,name)  prefix##_builtin_##name##l
#    define PSNIP_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##l
#  elif PSNIP_BUILTIN_SUFFIX_INT64 == 5
#    define PSNIP_BUILTIN__VARIANT_INT64(prefix,name)  prefix##_builtin_##name##ll
#    define PSNIP_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

    /******
     *** GCC-style built-ins
     ******/

     /*** __builtin_ffs ***/

#define PSNIP_BUILTIN__FFS_DEFINE_PORTABLE(f_n, T)	\
  PSNIP_BUILTIN__FUNCTION				\
  int psnip_builtin_##f_n(T x) {			\
    static const char psnip_builtin_ffs_lookup[256] = { \
      0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      8, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1	\
    };							\
							\
    unsigned char t;					\
    size_t s = 0;					\
							\
    while (s < (sizeof(T) * 8)) {			\
      t = (unsigned char) ((x >> s) & 0xff);		\
      if (t)						\
        return psnip_builtin_ffs_lookup[t] + s;		\
							\
      s += 8;						\
    }							\
							\
    return 0;						\
  }

#if PSNIP_BUILTIN_GNU_HAS_BUILTIN(__builtin_ffs, 3, 3)
#  define psnip_builtin_ffs(x)   __builtin_ffs(x)
#  define psnip_builtin_ffsl(x)  __builtin_ffsl(x)
#  define psnip_builtin_ffsll(x) __builtin_ffsll(x)
#  define psnip_builtin_ffs32(x) PSNIP_BUILTIN__VARIANT_INT32(_,ffs)(x)
#  define psnip_builtin_ffs64(x) PSNIP_BUILTIN__VARIANT_INT64(_,ffs)(x)
#else
#  if PSNIP_BUILTIN_MSVC_HAS_INTRIN(_BitScanForward, 14, 0)
PSNIP_BUILTIN__FUNCTION
int psnip_builtin_ffsll(long long v) {
    unsigned long r;
#    if defined(_M_AMD64) || defined(_M_ARM)
    if (_BitScanForward64(&r, (unsigned long long) v)) {
        return (int)(r + 1);
    }
#    else
    if (_BitScanForward(&r, (unsigned long)(v))) {
        return (int)(r + 1);
    }
    else if (_BitScanForward(&r, (unsigned long)(v >> 32))) {
        return (int)(r + 33);
    }
#    endif
    return 0;
}

PSNIP_BUILTIN__FUNCTION
int psnip_builtin_ffsl(long v) {
    unsigned long r;
    if (_BitScanForward(&r, (unsigned long)v)) {
        return (int)(r + 1);
    }
    return 0;
}

PSNIP_BUILTIN__FUNCTION
int psnip_builtin_ffs(int v) {
    return psnip_builtin_ffsl(v);
}
#  else
PSNIP_BUILTIN__FFS_DEFINE_PORTABLE(ffs, int)
PSNIP_BUILTIN__FFS_DEFINE_PORTABLE(ffsl, long)
PSNIP_BUILTIN__FFS_DEFINE_PORTABLE(ffsll, long long)
#  endif
#  if defined(PSNIP_BUILTIN_EMULATE_NATIVE)
#    define __builtin_ffsll(v) psnip_builtin_ffsll(v)
#    define __builtin_ffsl(v)  psnip_builtin_ffsl(v)
#    define __builtin_ffs(v)   psnip_builtin_ffs(v)
#  endif
#endif

#if !defined(psnip_builtin_ffs32)
#  define psnip_builtin_ffs32(x) PSNIP_BUILTIN__VARIANT_INT32(psnip,ffs)(x)
#endif

#if !defined(psnip_builtin_ffs64)
#  define psnip_builtin_ffs64(x) PSNIP_BUILTIN__VARIANT_INT64(psnip,ffs)(x)
#endif

#endif