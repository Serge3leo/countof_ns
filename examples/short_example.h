// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "countof_ns.h"

#include <assert.h>
#include <stdio.h>
#ifdef __cplusplus
    #include <vector>
#endif

#if defined(__cplusplus) && __cplusplus < 201103L
    #error "Require C++11 or above"
#endif
#if __cplusplus || __STDC_VERSION__ >= 202311L
    #define example_assert(e)  static_assert((e), #e)
#elif __STDC_VERSION__ >= 201112L
    #define example_assert(e)  _Static_assert((e), #e)
#else
    #define example_assert(e)  assert(e)
#endif

static void shr_example(void) {
    int a1[42] = { 0 };
    int a2[56][23] = { { 0 } };
    int (*p1)[23] = &a2[0];
    int *p2 = a1;
    int **p3 = &p2;
    int i = 0;
    size_t fail = 0;

    example_assert(42 == countof_ns(a1));
    example_assert(56 == countof_ns(a2));
    example_assert(23 == countof_ns(a2[0]));
    int c[countof_ns(a2[0])] = { 0 };
    example_assert(23 == countof_ns(c));
    #ifndef __cplusplus
        example_assert(23 == countof_ns(*p1));
        example_assert(12 == countof_ns(*(int (*)[12])&p2));
        example_assert(3 == countof_ns(*(int (*)[3])p3));
        example_assert(2 == countof_ns(*(int(*)[2][2])&p3));
    #endif

    #ifdef EXAMPLE_FAIL
                            #warning "Must error below @{"
        fail += countof_ns(p1);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(p2);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(i);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(NULL);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(0);
                            #warning "}@ Must error above"
    #endif
    printf("shr_example: Ok (fail=%zu)\n", fail);
    (void)p1[i]; (void)p3[i]; (void)c[i];
}

static void zla_example(void) {
#if HAVE_ZERO_LENGTH_ARRAYS
    int a1[42] = { 0 };
    int a2[56][23] = { { 0 } };
    int (*p1)[23] = &a2[0];
    int *p2 = a1;
    int **p3 = &p2;
    int i = 0;
    size_t fail = 0;

    example_assert(42 == countof_ns(a1));
    example_assert(56 == countof_ns(a2));
    example_assert(23 == countof_ns(a2[0]));
    #ifndef __cplusplus
        example_assert(23 == countof_ns(*p1));
        example_assert(12 == countof_ns(*(int (*)[12])&p2));
        example_assert(3 == countof_ns(*(int (*)[3])p3));
        example_assert(2 == countof_ns(*(int(*)[2][2])&p3));
    #endif

    #ifdef EXAMPLE_FAIL
                            #warning "Must error below @{"
        fail += countof_ns(p1);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(p2);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(i);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(NULL);
                            #warning "}@ Must error above/below @{"
        fail += countof_ns(0);
                            #warning "}@ Must error above"
    #endif
    printf("zal_example: Ok (fail=%zu)\n", fail);
    (void)p1[i]; (void)p3[i]; //(void)c[i];
#endif
}

static int short_example(void) {
    shr_example();
    zla_example();
    return 0;
}
