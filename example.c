// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2025-11-16 23:01:48 - Создан.
// 2025-11-18 04:07:37 - Поддержка C99/C11 вычитания указателей
//

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
#if !__cplusplus && !defined(static_assert)
    #define static_assert(e, s)  assert((e) && (s))
#endif
#if !__cplusplus && __STDC_VERSION__ < 202311L && !defined(thread_local)
    #define thread_local _Thread_local
#endif
#if __clang__ || __GNUC__
    #define MIN_DIM  (0)
#else
    #define MIN_DIM  (1)
#endif
#if __clang_major__ >= 19 || __GNUC__ >= 15
    #define HAVE_FLEXIBLE_ONLY (1)
#endif

int main(void) {
    const int a1[42] = {};
    volatile int a2[42][56];
    static thread_local int a3[42][56][23];
    int min1[MIN_DIM] = {};
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
    // res += countof_ns(&a1);
    // res += countof_ns(a1[0]);
    // res += countof_ns(&a1[0]);
    // res += countof_ns(&min1);
    // res += countof_ns(&min3[0]);
    // res += countof_ns(s);
    #if HAVE_FLEXIBLE_ONLY
        // res += countof_ns(flexs42[0]);
    #endif

    // Positive examples
    static_assert(countof_ns(a1) == 42, "a1");
    static_assert(countof_ns(a2) == 42, "a2");
    static_assert(countof_ns(a2[0]) == 56, "a2[0]");
    static_assert(countof_ns(a3) == 42, "a3");
    static_assert(countof_ns(a3[0]) == 56, "a3[0]");
    static_assert(countof_ns(a3[0][0]) == 23, "a3[0][0]");
    static_assert(countof_ns(min1) == MIN_DIM, "min1");
    static_assert(countof_ns(min2) == MIN_DIM, "min2");
    static_assert(countof_ns(min2[0]) == 42, "min2[0]");
    static_assert(countof_ns(min3) == MIN_DIM, "min3");
    static_assert(countof_ns(min3[0]) == MIN_DIM, "min3[0]");
    #if __cplusplus || MIN_DIM > 0
        static_assert(countof_ns(min4) == 42, "min4");
    #endif
    static_assert(countof_ns(min4[0]) == MIN_DIM, "min4[0]");
    static_assert(countof_ns(min4[0][0]) == 56, "min4[0][0]");
    static_assert(countof_ns(s.mins) == MIN_DIM, "s.mins");
    #if HAVE_FLEXIBLE_ONLY
        static_assert(countof_ns(flexsmin) == MIN_DIM, "flexsmin");
        (void)flexsmin;
        #if __cplusplus
            static_assert(countof_ns(flexs42) == 42, "flexs42");
        #endif
        (void)flexs42;
    #endif

    #ifdef countof
        printf("Comparison with C2y countof()\n");
        static_assert(sizeof(min4[0]) == 0 &&
                      countof(min4) == 42, "C2y countof(min4)");
        #if HAVE_FLEXIBLE_ONLY
            static_assert(countof(flexs42) == 42, "C2y countof(flexs42)");
        #endif
    #endif

    #if !__STDC_NO_VLA__ && !__cplusplus && !_COUNTOF_NS_VLS_UNSUPPORTED
        for(size_t n = MIN_DIM; n < 4; n++) {
            int vla[n];
            assert(countof_ns(vla) == n);
            for(size_t m = MIN_DIM; m < 4; m++) {
                int vlm[n][m];
                assert(m <= 0 || countof_ns(vlm) == n);
                assert(countof_ns(vlm[0]) == m);
                #ifdef countof
                    assert(countof(vlm) == n);
                    assert(countof(vlm[0]) == m);
                #endif
                for(size_t l = MIN_DIM; l < 4; l++) {
                    int vlt[n][m][l];
                    assert(m <= 0 || l <= 0 ||
                           countof_ns(vlt) == n);
                    assert(l <= 0 ||
                           countof_ns(vlt[0]) == m);
                    assert(countof_ns(vlt[0][0]) == l);
                    #ifdef countof
                        assert(countof(vlt) == n);
                        assert(countof(vlt[0]) == m);
                        assert(countof(vlt[0][0]) == l);
                    #endif
                }
            }
        }
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
    #ifdef __cplusplus
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
