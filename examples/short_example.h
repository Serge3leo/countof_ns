// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// The short first part of the example (aka "Hello world"). For examples of
// zero-length arrays, variable-length arrays, `const`, `volatile`, etc., see
// the second part: long_example.h.
//
// `short_example.h` compilation options:
//
// EXAMPLE_FAIL - Include examples for compilation errors with erroneous
//                parameters.
//
// Example:
//  $ clang -I../include short_example.c && ./a.out  # Success
//  $ clang -I../include -DEXAMPLE_FAIL short_example.c  # Fail with errors
//
//  $ clang++ -std=c++11 -I../include short_example_cxx.cpp  # C++ examples
//
//  ...\examples>cl /std:clatest /I..\include short_example.c
//  ...\examples>cl /std:clatest /I..\include /DEXAMPLE_FAIL short_example.c
//  ...\examples>cl /Zc:__cplusplus /I..\include short_example_cxx.cpp

#include "countof_ns.h"

#include <assert.h>
#include <stdio.h>

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

static void short_example(void) {
    size_t fail = 0;
    int a1[42] = { 0 };
    int a2[56][23] = { { 0 } };
    int (*p1)[23] = &a2[0];
    int *p2 = a1;
    int **p3 = &p2;
    int i = 0;

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
        int a3[2] = { 0 };
        struct {
            // int bits: sizeof(a3);  // OK
            // int bits: std::size(a3);  // Bug in clang++/icx // score 2:4
             int bits: countof_ns(a3);  // Bug in clang++/icx TODO XXX
        } s = { 0 };
        (void)a3[s.bits];
    #endif

    #ifdef EXAMPLE_FAIL
                            #pragma message ("Must error below @{")
        fail += countof_ns(p1);
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(p2);
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(i);
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(NULL);
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(0);
                            #pragma message ("}@ Must error above")
    #endif

    printf("%s: Ok (fail=%zu)\n", __func__, fail);
    (void)p1[i]; (void)p3[i]; (void)c[i];
}
