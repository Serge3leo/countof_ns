// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

//#define EXAMPLE_FAIL (1)  // 1 - on T *
                            // 2 - on function arg
                            // 3 - on T (*)[N]
                            // 4 - on T (*)[0]  (ZLA)
                            // 5 - on T (*)[n]  (VLA)
#if !defined(HAVE_VLA_CXX) && !defined(_MSC_VER)
    // To disable VLA tests for non MSVC, use: -DHAVE_VLA_CXX=0
    #define HAVE_VLA_CXX  (1)
#endif
#if !defined(HAVE_ZLA) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
    // To enable ZLA tests for SunPro, use: -features=zla -DHAVE_ZLA
    #define HAVE_ZLA  (1)
#endif

#if HAVE_VLA_CXX && \
    !defined(_COUNTOF_NS_WANT_VLA_C11) && !defined(_COUNTOF_NS_WANT_VLA_BUILTIN)
    #define _COUNTOF_NS_WANT_VLA_BUILTIN  (1)  // TODO: remove
#endif
#include "countof_ns.h"

#include <stdio.h>  // clang-14 conflict with own libc++

#if HAVE_VLA_CXX
    #include <assert.h>
#endif
#if __cplusplus >= 201703L || __cpp_lib_nonmember_container_access >= 201411L
    #include <vector>

    #define std_size_compare(e)  static_assert(e)
    #define std_size(a)  (std::size(a))
#else
    #define std_size_compare(e)
    #define std_size(a)  (size_t(-2025))
#endif
#if __cplusplus > 201400L
    #define var_func_constexpr constexpr
#else
    #define var_func_constexpr
#endif

template<size_t N>
var_func_constexpr int sum1(const int (&a)[N]) {
    #if __cplusplus > 202300L
        std_size_compare(N == std::size(a));
        static_assert(N == countof_ns(a), "sum1: N == countof_ns(a)");
    #endif
    int s = 0;
    for (size_t i = 0; i < countof_ns(a); i++) {
        s += a[i];
    }
    return s;
}

template<size_t N, size_t M>
var_func_constexpr int sum2(const int (&a)[N][M]) {
    #if __cplusplus > 202300L
        std_size_compare(N == std::size(a));
        static_assert(N == countof_ns(a), "sum2: N == countof_ns(a)");
        std_size_compare(M == std::size(*a));
        static_assert(M == countof_ns(*a), "sum2: M == countof_ns(*a)");
    #endif
    int s = 0;
    for (size_t i = 0; i < countof_ns(a); i++) {
        for (size_t j = 0; j < countof_ns(a[0]); j++) {
            s += a[i][j];
        }
    }
    return s;
}

#if EXAMPLE_FAIL == 2
    int bad1(size_t n, const int a[]) {
        (void)std_size(a);
        (void)countof_ns(a);
        return 1;
    }
#endif

int main(void) {
    constexpr int a1[1917] = { 25, 10 };
    constexpr int a2[25][10] = { { 1917 }, { 25, 10 } };

    std_size_compare(1917 == std::size(a1));
    static_assert(1917 == countof_ns(a1), "1917 == countof_ns(a1)");
    std_size_compare(25 == std::size(a2));
    static_assert(25 == countof_ns(a2), "25 == countof_ns(a2)");
    std_size_compare(10 == std::size(a2[0]));
    static_assert(10 == countof_ns(a2[0]), "10 == countof_ns(a2[0])");

    #if __cplusplus > 201400L
        static_assert(35 == sum1(a1), "35 == sum1(a1)");
        static_assert(1952 == sum2(a2), "1952 == sum2(a2)");
    #else
        assert(35 == sum1(a1));
        assert(1952 == sum2(a2));
    #endif
    printf("sum1() & sum2() - Ok\n");

    #if EXAMPLE_FAIL == 1
        auto *p1 = &a1[0];

        (void)std_size(p1);
        (void)countof_ns(p1);
        printf("auto *p1 = &a1[0] - fail\n");
    #endif

    #if EXAMPLE_FAIL == 2
        (void)bad1(1917, a1);
        printf("not pure array function argument - Fail\n");
    #endif

    #if EXAMPLE_FAIL == 3
        auto *p2 = &a2[0];

        (void)std_size(p2);
        (void)countof_ns(p2);
        printf("auto *p2 = &a2[0] - fail\n");
    #endif

    #if HAVE_ZLA
        #if !__SUNPRO_CC
            int zn0[1917][0];
            int z00[0][0];

            static_assert(0 == sizeof(zn0), "0 == sizeof(zn0)");
            std_size_compare(1917 == std::size(zn0));
            static_assert(1917 == countof_ns(zn0),
                          "1917 == countof_ns(zn0)");

            static_assert(0 == sizeof(z00), "0 == sizeof(z00)");
            // expected fail: std_size_compare(0 == std::size(z00));
            static_assert(0  == countof_ns(z00), "0 == countof_ns(z00)");
        #endif

        int z[0];
        int z0n[0][10];

        static_assert(0 == sizeof(z), "0 == sizeof(z)");
        // expected fail: std_size_compare(0 == std::size(z00));
        static_assert(0  == countof_ns(z), "0 == countof_ns(z)");

        static_assert(0 == sizeof(z0n), "0 == sizeof(z0n)");
        static_assert(10*sizeof(z0n[0][0]) == sizeof(z0n[0]),
                      "10*sizeof(z0n[0][0]) == sizeof(z0n[0])");
        // expected fail: std_size_compare(0 == std::size(z0n));
        std_size_compare(10 == std::size(z0n[0]));
        static_assert(0  == countof_ns(z0n), "0 == countof_ns(z0n)");
        static_assert(10  == countof_ns(z0n[0]), "10 == countof_ns(z0n[0])");

        printf("ZLA - Ok\n");

        #if EXAMPLE_FAIL == 4
            auto *zp = &z;

            (void)countof_ns(zp);
            printf("auto *zp = &z - Fail\n");
        #endif
    #endif

    #if HAVE_VLA_CXX
        size_t n0 = 0;
        for (size_t n = 0; n < 25; n++) {
            int vn0[n][n0];  // For C VLA - formally UB, but C++ extension
            int v00[n0][n0];
            int v0n[n0][n];

            assert(0 == sizeof(vn0));
            // WARNING, for C2y: assert(n == countof(vn0))
            // But countof_ns(vn0), can't resolve the uncertain zero-by-zero
            assert(0 == countof_ns(vn0));

            assert(0 == sizeof(v00));
            assert(0  == countof_ns(v00));

            assert(0 == sizeof(v0n));
            assert(0  == countof_ns(v0n));

            #if EXAMPLE_FAIL == 5
                auto *vp = &vn0[0];

                // Compile-time constraint violation
                (void)countof_ns(vp);

                printf("auto *vp = &vn0[0] - Fail\n");
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
    #if HAVE_VLA_CXX
        printf(", HAVE_VLA_CXX");
    #endif
    #if HAVE_ZLA
        printf(", HAVE_ZLA");
    #endif
    printf(", __cplusplus=%ld\n", __cplusplus);
}
