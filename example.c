// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2025-11-16 23:01:48 - Создан.
// 2025-11-18 04:07:37 - Поддержка C99/C11 вычитания указателей
//

#if !DISABLE_VLA_EXAMPLE && !__SUNPRO_C
    #define _COUNTOF_NS_WANT_VLA  (1)
#endif
#include "countof_ns.h"

#include <assert.h>
#include <stdio.h>
#if defined(__has_include)
    #if __has_include(<stdcountof.h>) && !__cplusplus
        #include <stdcountof.h>
    #endif
#endif
#if __cplusplus >= 201703L || __cpp_lib_nonmember_container_access >= 201411L
    #include <vector>
    #define std_size(a)  (std::size(a))
#endif
#if defined(__cplusplus) && __cplusplus < 201103L
    #error "Require C++11 or above"
#endif
#if __cplusplus || __STDC_VERSION__ >= 202311L
    #define example_assert(...)  static_assert(__VA_ARGS__)
    #if __POCC__
        #define example_thread_local
    #else
        #define example_thread_local  thread_local
    #endif
#elif __STDC_VERSION__ >= 201112L
    #define example_assert(...)  _Static_assert(__VA_ARGS__)
    #define example_thread_local  _Thread_local
#else
    #define example_assert(e, s)  assert((e) && (s))
    #define example_thread_local
#endif
#if __clang__ || __GNUC__ || __NVCC__
    #define MIN_DIM  (0)
#else
    #define MIN_DIM  (1)
#endif
#if __clang_major__ >= 19 || __GNUC__ >= 15
    #define HAVE_FLEXIBLE_ONLY (1)
#endif

int main(void) {
    const int a1[42] = { 0 };
    volatile int a2[42][56];
    static example_thread_local int a3[42][56][23];
    int min1[MIN_DIM];
    int min2[MIN_DIM][42];
    int min3[MIN_DIM][MIN_DIM];
    int min4[42][MIN_DIM][56];
    struct {
        int mins[MIN_DIM];
    } s;
    #if HAVE_FLEXIBLE_ONLY
        struct {
            int flex[];
        } flexsmin[MIN_DIM];
        struct {
            int flex[];
        } flexs42[42];
    #endif

    size_t res = countof_ns(a1);
    // Negative examples
    #if 0
        const int *p1 = a1;
        const int **pp1 = &p1;
        volatile int (*p2)[56] = a2;
        int (*p3)[56][23] = a3;
        res += countof_ns(p1);
        res += countof_ns(pp1);
        res += countof_ns(p2);
        res += countof_ns(p3);
        res += countof_ns(&a1);
        res += countof_ns(a1[0]);
        res += countof_ns(&a1[0]);
        res += countof_ns(&min1);
        res += countof_ns(&min3[0]);
        res += countof_ns(s);
        #if HAVE_FLEXIBLE_ONLY
            res += countof_ns(flexs42[0]);
        #endif
        // Need DISABLE_VLA_EXAMPLE
        #if _COUNTOF_NS_VLA_UNSUPPORTED && !__STDC_NO_VLA__ && !__cplusplus
            for(size_t n = MIN_DIM; n < 4; n++) {
                int vla[n];
                res += countof_ns(vla);
            }
        #endif
    #endif

    // Positive examples
    example_assert(countof_ns(a1) == 42, "a1");
    example_assert(countof_ns(a2) == 42, "a2");
    example_assert(countof_ns(a2[0]) == 56, "a2[0]");
    example_assert(countof_ns(a3) == 42, "a3");
    example_assert(countof_ns(a3[0]) == 56, "a3[0]");
    example_assert(countof_ns(a3[0][0]) == 23, "a3[0][0]");
    example_assert(countof_ns(min1) == MIN_DIM, "min1");
    example_assert(countof_ns(min2) == MIN_DIM, "min2");
    example_assert(countof_ns(min2[0]) == 42, "min2[0]");
    example_assert(countof_ns(min3) == MIN_DIM, "min3");
    example_assert(countof_ns(min3[0]) == MIN_DIM, "min3[0]");
    #if __cplusplus || MIN_DIM > 0
        example_assert(countof_ns(min4) == 42, "min4");
    #endif
    example_assert(countof_ns(min4[0]) == MIN_DIM, "min4[0]");
    example_assert(countof_ns(min4[0][0]) == 56, "min4[0][0]");
    example_assert(countof_ns(s.mins) == MIN_DIM, "s.mins");
    #if HAVE_FLEXIBLE_ONLY
        example_assert(countof_ns(flexsmin) == MIN_DIM, "flexsmin");
        (void)flexsmin;
        #if __cplusplus
            example_assert(countof_ns(flexs42) == 42, "flexs42");
        #endif
        (void)flexs42;
    #endif

    #ifdef countof
        printf("Comparison with C2y countof()\n");
        example_assert(sizeof(min4[0]) == 0 &&
                      countof(min4) == 42, "C2y countof(min4)");
        #if HAVE_FLEXIBLE_ONLY
            example_assert(countof(flexs42) == 42, "C2y countof(flexs42)");
        #endif
    #endif

    #if !_COUNTOF_NS_VLA_UNSUPPORTED
        for(size_t n = MIN_DIM; n < 4; n++) {
            int vla[n];
            assert(countof_ns(vla) == n);
            for(size_t m = MIN_DIM; m < 4; m++) {
                int vlm[n][m];
                assert(countof_ns(vlm) == (m > 0 ? n : 0));
                assert(countof_ns(vlm[0]) == m);
                #ifdef countof
                    assert(countof(vlm) == n);
                    assert(countof(vlm[0]) == m);
                #endif
                for(size_t l = MIN_DIM; l < 4; l++) {
                    int vlt[n][m][l];
                    assert(countof_ns(vlt) ==
                           (m > 0 && l > 0 ? n : 0));
                    assert(countof_ns(vlt[0]) ==
                           (l > 0 ? m : 0));
                    assert(countof_ns(vlt[0][0]) == l);
                    #ifdef countof
                        assert(countof(vlt) == n);
                        assert(countof(vlt[0]) == m);
                        assert(countof(vlt[0][0]) == l);
                    #endif
                }
            }
        }
    #elif _COUNTOF_NS_WANT_VLA && !__STDC_NO_VLA__ && !__cplusplus
        #error "VLA detection error, probably bug in \"countof_ns.h\""
    #endif

    #ifdef std_size
        printf("Comparison with std::size()\n");
        static_assert(std_size(a1) == 42);
        static_assert(std_size(a2) == 42);
        static_assert(std_size(a2[0]) == 56);
        static_assert(std_size(a3) == 42);
        static_assert(std_size(a3[0]) == 56);
        static_assert(std_size(a3[0][0]) == 23);
        static_assert(std_size(min4) == 42);
        static_assert(std_size(min4[0][0]) == 56);
        #if HAVE_FLEXIBLE_ONLY
            printf("Comparison with std::size() "
                    "and flexible array member only\n");
            static_assert(std_size(flexs42) == 42);
        #endif
        #if MIN_DIM > 0
            static_assert(std_size(min1) == MIN_DIM);
            static_assert(std_size(min2) == MIN_DIM);
            static_assert(std_size(min3) == MIN_DIM);
            static_assert(std_size(min3[0]) == MIN_DIM);
            static_assert(std_size(min4[0]) == MIN_DIM);
            static_assert(std_size(s.mins) == MIN_DIM);
            #if HAVE_FLEXIBLE_ONLY
                static_assert(std_size(flexsmin) == MIN_DIM);
            #endif
        #endif
    #endif
    #ifdef __clang_major__
        printf("__clang_major__.__clang_minor__ %d.%d ",
                __clang_major__, __clang_minor__);
    #endif
    #ifdef __GNUC__
        printf("__GNUC__.__GNUC_MINOR__ %d.%d ",
                __GNUC__, __GNUC_MINOR__);
    #endif
    #ifdef _MSC_VER  // Visual Studio 2022/2026
        printf("_MSC_VER %d ", _MSC_VER);
    #endif
    #ifdef __POCC__  // Pelles C
        printf("__POCC__ %d ", __POCC__);
    #endif
    #ifdef __SUNPRO_C  // Oracle Developer Studio
        printf("__SUNPRO_C 0x%x ", __SUNPRO_C);
    #endif
    #ifdef __NVCC__  // NVIDIA HPC SDK
        printf("__CUDACC_VER_MAJOR__.__CUDACC_VER_MINOR__ %d.%d ",
                __CUDACC_VER_MAJOR__, __CUDACC_VER_MINOR__);
    #endif
    #if __cplusplus
        #ifdef __cpp_lib_nonmember_container_access
            printf("__cpp_lib_nonmember_container_access %ld ",
                   __cpp_lib_nonmember_container_access);
        #endif
        printf("__cplusplus %ld\n", __cplusplus);
    #else
        #ifdef __STDC_NO_VLA__
            printf("__STDC_NO_VLA__ %d ", __STDC_NO_VLA__);
        #endif
        printf("__STDC_VERSION__ %ld\n", __STDC_VERSION__);
    #endif
    (void)res;
    return 0;
}
