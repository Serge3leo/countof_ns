// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

//#define EXAMPLE_FAIL (6)  // 1 - on T *
                            // 2 - on not pure array function argument
                            // 3 - on T (*)[N]
                            // 4 - on T (*)[0]  (ZLA)
                            // 5 - on T (*)[n]  (VLA)
                            // 6 - resolve the uncertain zero-by-zero for ZLA
#if !defined(HAVE_ZLA) && !defined(_MSC_VER) && !defined(__POCC__) && \
    !defined(__SUNPRO_C) && !defined(__IBMC__)
    // To enable ZLA for SunPro, use: -features=zla -DHAVE_ZLA=1
    #define HAVE_ZLA  (1)
#endif
#if !defined(HAVE_BROKEN_VLA) && (defined(__NVCOMPILER) || defined(__POCC__))
    // NVHPC (pgcc) have broken VLA implementation
    // See: no_have_broken_vla.c and have_vla.h
    #define HAVE_BROKEN_VLA  (1)
#endif

#include "countof_ns.h"

#include <assert.h>
#include <stdio.h>

#ifdef __has_include
    #if __has_include(<stdcountof.h>)
        #include <stdcountof.h>

        #define countof_compare(e)  assert(e)
        #define stdc_countof(a)  (countof(a))
    #endif
#endif
#ifndef stdc_countof
    #define countof_compare(e)
    #define stdc_countof(a)  ((size_t)-2025)
#endif
#if __STDC_VERSION__ >= 202311L
    #define g_static_assert(e)  static_assert((e), #e)
#elif __STDC_VERSION__ >= 201112L
    #define g_static_assert(e)  _Static_assert((e), #e)
#else
    #define g_static_assert(e)  assert(e)
#endif

#if !__STDC_NO_VLA__ && !HAVE_BROKEN_VLA
    int sum1(size_t n, int (*a)[n]) {
        assert(n*sizeof((*a)[0]) == sizeof(*a));
        countof_compare(n == countof(*a));
        assert(n == countof_ns(*a));

        int s = 0;
        for (size_t i = 0; i < countof_ns(*a); i++) {
            s += (*a)[i];
        }
        return s;
    }
    int sum2(size_t n, size_t m, int (*a)[n][m]) {
        assert(n*sizeof((*a)[0]) == sizeof(*a));
        assert(n*m*sizeof((*a)[0][0]) == sizeof(*a));
        countof_compare(n == countof(*a));
        // Current countof_ns(vn0), can't resolve the uncertain zero-by-zero
        if (m) {
            assert(n == countof_ns(*a));
        } else {
            assert(0 == countof_ns(*a));
        }
        assert(m*sizeof((*a)[0][0]) == sizeof((*a)[0]));
        countof_compare(m == countof((*a)[0]));
        assert(m == countof_ns((*a)[0]));

        int s = 0;
        for (size_t i = 0; i < countof_ns((*a)); i++) {
            for (size_t j = 0; j < countof_ns((*a)[0]); j++) {
                s += (*a)[i][j];
            }
        }
        return s;
    }
#endif
#if EXAMPLE_FAIL == 2
    int bad1(size_t n, const int a[static n]) {  // Not VLA
        (void)stdc_countof(a);
        (void)countof_ns(a);
        return 1;
    }
#endif

int fa1[1917];
int fb1[countof_ns(fa1)];

int main(void) {
    assert(1917 == countof_ns(fb1));

    int a1[1917] = { 25, 10 };
    int a2[25][10] = { { 1917 }, { 25, 10 } };

    countof_compare(1917 == countof(a1));
    g_static_assert(1917 == countof_ns(a1));
    countof_compare(25 == countof(a2));
    g_static_assert(25 == countof_ns(a2));
    countof_compare(10 == countof(a2[0]));
    g_static_assert(10 == countof_ns(a2[0]));
    #if !__STDC_NO_VLA__ && !HAVE_BROKEN_VLA
        assert(35 == sum1(1917, &a1));
        assert(1952 == sum2(25, 10, &a2));
        printf("sum1() & sum2() - Ok\n");
    #else
        printf("sum1() & sum2() - Skip\n");
        (void)a1; (void)a2;
    #endif

    #if EXAMPLE_FAIL == 1
        int *p1 = &a1[0];
        (void)p1;
        (void)stdc_countof(p1);
        (void)countof_ns(p1);
        printf("int * - Fail\n");
    #endif

    #if EXAMPLE_FAIL == 2
        (void)bad1(1917, a1);
        printf("not pure array function argument - Fail\n");
    #endif

    #if EXAMPLE_FAIL == 3
        int (*p2)[10] = &a2[0];
        (void)p2;
        (void)stdc_countof(p2);
        (void)countof_ns(p2);
        printf("int (*)[N] - Fail\n");
    #endif

    #if HAVE_ZLA
        int zn0[1917][0];
        int z00[0][0];
        int z0n[0][10];

        g_static_assert(0 == sizeof(zn0));
        countof_compare(1917 == countof(zn0));
        // Current countof_ns(zn0), can't resolve the uncertain zero-by-zero.
        #if EXAMPLE_FAIL == 6
            #if !__LCC__ && !__NVCOMPILER && !__INTEL_COMPILER
                // if !HAVE_BROKEN_BUILTIN_TYPES_COMPATIBLE_P
                // For ZLA case, fail if resolve not true
                (void)countof_ns(zn0);
            #else
                // if HAVE_BROKEN_BUILTIN_TYPES_COMPATIBLE_P
                // return 0 as VLA case
                g_static_assert(!(0 == countof_ns(zn0)));
            #endif
            printf("resolve the uncertain zero-by-zero for ZLA - Fail\n");
        #endif

        g_static_assert(0 == sizeof(z00));
        countof_compare(0 == countof(z00));
        g_static_assert(0 == countof_ns(z00));

        g_static_assert(0 == sizeof(z0n));
        g_static_assert(10*sizeof(z0n[0][0]) == sizeof(z0n[0]));
        countof_compare(0 == countof(z0n));
        countof_compare(10 == countof(z0n[0]));
        g_static_assert(0 == countof_ns(z0n));
        g_static_assert(10 == countof_ns(z0n[0]));

        printf("ZLA - Ok\n");

        #if EXAMPLE_FAIL == 4
            int (*zp)[0] = &zn0[0];
            (void)zp;
            (void)countof_ns(zp);
            printf("int (*)[0] - Fail\n");
        #endif
    #endif

    #if !__STDC_NO_VLA__
        size_t n0 = 0;
        for (size_t n = 0; n < 25; n++) {
            int v[n];

            assert(n*sizeof(v[0]) == sizeof(v));
            countof_compare(n == countof(v));
            assert(n == countof_ns(v));

            int vn0[n][n0];  // Formally UB, but common C extensions...?
            int v00[n0][n0];
            int v0n[n0][n];

            assert(0 == sizeof(vn0));
            countof_compare(n == countof(vn0));
            // Current countof_ns(vn0), can't resolve the uncertain
            // zero-by-zero
            assert(0 == countof_ns(vn0));

            assert(0 == sizeof(v00));
            countof_compare(0  == countof(v00));
            assert(0  == countof_ns(v00));

            assert(0 == sizeof(v0n));
            countof_compare(0  == countof(v0n));
            assert(0  == countof_ns(v0n));

            #if EXAMPLE_FAIL == 5
                int (*vp)[n] = &vn0[0];
                (void)vp;
                (void)countof_ns(vp);  // Compile-time constraint violation
                printf("int (*)[n] - Fail\n");
            #endif
        }
        printf("VLA - Ok\n");
    #endif
    #ifndef EXAMPLE_FAIL
        printf("Ok");
    #else
        printf("Fail, EXAMPLE_FAIL=%d", EXAMPLE_FAIL);
    #endif
    #if _COUNTOF_NS_VLA_UNSUPPORTED
        printf(", _COUNTOF_NS_VLA_UNSUPPORTED");
    #endif
    #if HAVE_ZLA
        printf(", HAVE_ZLA");
    #endif
    #if HAVE_BROKEN_VLA
        printf(", HAVE_BROKEN_VLA");
    #endif
    #if __STDC_NO_VLA__
        printf(", __STDC_NO_VLA__");
    #endif
    printf(", __STDC_VERSION__=%ld\n", __STDC_VERSION__);
}
