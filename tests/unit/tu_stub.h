// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef TU_STUB_H_8927
#define TU_STUB_H_8927

#ifndef TU_COUNTOF_INC
    #define TU_COUNTOF_INC  "countof_ns.h"
#endif
#ifndef TU_COUNTOF_FUNC
    #define TU_COUNTOF_FUNC  countof_ns
#endif
#define TU_COUNTOF(a)  (TU_COUNTOF_FUNC(a))

#include TU_COUNTOF_INC

#if _COUNTOF_NS_VLA_UNSUPPORTED || !defined(countof_ns)
    #if _COUNTOF_NS_WANT_C11_VLA && !__cplusplus
        #error "Wrong autoconf of VLA"
    #endif
    #define TU_C11_VLA  ("")
#else
    #define TU_C11_VLA  (" with C11&VLA")
#endif
#ifdef __cplusplus
    #define TU_LANG  ("C++")
    #define TU_LVER  __cplusplus
#else
    #define TU_LANG  ("C")
    #define TU_LVER  __STDC_VERSION__
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__cplusplus) && __cplusplus < 201103L
    #error "Require C++11 or above"
#endif
#if __cplusplus || __STDC_VERSION__ >= 202311L
    #define tu_static_assert(e)  static_assert((e), #e)
#elif __STDC_VERSION__ >= 201112L
    #define tu_static_assert(e)  _Static_assert((e), #e)
#else
    #define tu_static_assert(e)  assert(e)
#endif

                            // TODO: Strange: do{ } while(0)
#define TU_STATIC_ASSERT_AND_RETURN(a, b)  \
            tu_static_assert((a) == (b)); \
            return (a) - (b)

#define TU_ASSERT_AND_RETURN(a, b)  \
            if ((a) != (b)) { \
                printf("Fail %zu TU_ASSERT_AND_RETURN(%s, %s) [%zu, %zu]\n", \
                       (size_t)(b), (#a), (#b), (size_t)(a), (size_t)(b)); \
                exit(EXIT_FAILURE); \
            } \
            return (a) - (b)

#define _TU_STR1(S)  #S
#define _TU_STR(S)  _TU_STR1(S)
#define TU_REPORT()  do { \
            printf("%s:%s:%s: OK(%zu), for %s:%s()%s, %s %s\n", \
                    __FILE__, TU_UNIT_INC, _TU_STR(TU_UNIT), \
                    TU_UNIT(), \
                    TU_COUNTOF_INC, _TU_STR(TU_COUNTOF_FUNC), \
                    TU_C11_VLA, TU_LANG, _TU_STR(TU_LVER)); \
        } while(0)

#include TU_UNIT_INC

#endif  // TU_STUB_H_8927
