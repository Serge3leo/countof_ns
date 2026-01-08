// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef TU_STUB_H_8927
#define TU_STUB_H_8927

#define TU_0_FAIL          (1)
#define TU_LE_DESIRED_FAIL (2)
#define TU_GT_DESIRED_FAIL (3)
#define TU_ASSERT_FAIL     (4)

#ifndef TU_COUNTOF_INC
    #define TU_COUNTOF_INC  "countof_ns.h"
#endif
#ifndef TU_COUNTOF_FUNC
    #define TU_COUNTOF_FUNC  countof_ns
#endif
#define TU_COUNTOF(a)  (TU_COUNTOF_FUNC(a))

#include TU_COUNTOF_INC

#ifndef countof_ns
    #define TU_C11_VLA  ("")
#elif _COUNTOF_NS_VLA_UNSUPPORTED
    #if _COUNTOF_NS_WANT_VLA_BUILTIN || _COUNTOF_NS_WANT_VLA_C11
        #error "Wrong VLA config"
    #endif
    #define TU_C11_VLA  (" _COUNTOF_NS_VLA_UNSUPPORTED")
#elif _COUNTOF_NS_WANT_VLA_C11
    #define TU_C11_VLA  (" _COUNTOF_NS_WANT_VLA_C11")
#elif _COUNTOF_NS_WANT_VLA_BUILTIN
    #define TU_C11_VLA  (" _COUNTOF_NS_WANT_VLA_BUILTIN")
#else
    #define TU_C11_VLA  (" _COUNTOF_NS_WANT_VLA_AUTOCONF")
#endif
#ifdef __cplusplus
    #define TU_LANG  ("C++")
    #define TU_LVER  __cplusplus
#else
    #define TU_LANG  ("C")
    #define TU_LVER  __STDC_VERSION__
#endif

#include <stdlib.h>
#include <stdio.h>

#define tu_assert(e)  \
            ((void)((e) ? (void)0 : _tu_assert(#e, __FILE__, __LINE__)))
#define _tu_assert(se, f, l)  \
            ((void)printf("Fail assertion %s:%d: '%s'\n", f, l, se), \
             exit(TU_ASSERT_FAIL))

#if defined(__cplusplus) && __cplusplus < 201103L
    #error "Require C++11 or above"
#endif
#if __cplusplus || __STDC_VERSION__ >= 202311L
    #define tu_static_assert(e)  static_assert((e), #e)
#elif __STDC_VERSION__ >= 201112L
    #define tu_static_assert(e)  _Static_assert((e), #e)
#else
    #define tu_static_assert(e)  tu_assert(e)
#endif

// Report on the test source

#define _TU_STR1(S)  #S
#define _TU_STR(S)  _TU_STR1(S)
#define TU_REPORT()  do { \
            printf("%s:%d:%s:%s for %s:%s()%s, %s %s\n", \
                    __FILE__, __LINE__, TU_UNIT_INC, _TU_STR(TU_UNIT), \
                    TU_COUNTOF_INC, _TU_STR(TU_COUNTOF_FUNC), \
                    TU_C11_VLA, TU_LANG, _TU_STR(TU_LVER)); \
        } while(0)

// Negative tests

#define _TU_TODO_MAGIC (0x390da063)

#define TU_FAIL(method, desired, computed)  do { \
            volatile size_t _tuf_d_ = (desired); \
            volatile size_t _tuf_c_ = (computed); \
            if(_TU_TODO_MAGIC != _tuf_c_) { \
                printf("Fail %zu desired=%zu %s(%s, ...) ", \
                       _tuf_c_, _tuf_d_, (method), (#desired)); \
                TU_REPORT(); \
                if (0 == _tuf_c_) { \
                    exit(TU_0_FAIL); \
                } else if (_tuf_c_ <= _tuf_d_) { \
                    exit(TU_LE_DESIRED_FAIL); \
                } \
            } \
        } while(0)

// Positive tests

#define _TU_ASSERT_AND_RETURN(method, desired, computed)  do { \
            volatile size_t _tuar_d_ = (desired); \
            volatile size_t _tuar_c_ = (computed); \
            if (_tuar_d_ == _tuar_c_) { \
                /* clang/icx */ \
                /* For UB (divide zero, etc) won't choose this branch. */ \
                printf("Ok %zu %s(%s=%zu, ...) ", \
                       _tuar_c_, (method), (#desired), _tuar_d_); \
                TU_REPORT(); \
                exit(EXIT_SUCCESS); \
            } \
            TU_FAIL((method), (desired), _tuar_c_); \
            return _TU_TODO_MAGIC; \
        } while(0)

#define TU_ASSERT_AND_RETURN(desired, computed) \
            _TU_ASSERT_AND_RETURN("TU_ASSERT_AND_RETURN", (desired), (computed))

#define TU_STATIC_ASSERT_AND_RETURN(desired, computed)  do { \
            tu_static_assert((desired) == (computed)); \
            _TU_ASSERT_AND_RETURN("TU_STATIC_ASSERT_AND_RETURN", \
                                  (desired), (computed)); \
        } while(0)


#include TU_UNIT_INC

#endif  // TU_STUB_H_8927
