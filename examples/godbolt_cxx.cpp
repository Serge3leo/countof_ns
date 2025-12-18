// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

//#define EXAMPLE_FAIL (1)  // 1 - on T *, 2 - on function arg, 3 - on T (*)[]
//#define HAVE_VLA (1)
//#define HAVE_ZLA (1)

#include "countof_ns.h"

#include <iosteram>
#include <vector>

#if __cplusplus >= 201703L || __cpp_lib_nonmember_container_access >= 201411L
    #define std_size_compare(e)  static_assert(e)
    #define std_size(a)  (std::size(a))
#else
    #define std_size_compare(e)
    #define std_size(a)  (size_t(0))
#endif

template<size_t N>
int sum1(int (&a)[N]) {
    static_assert(N == countof_ns(a), "sum1: N == countof_ns(a)");
    std_size_compare(N == std::size(a));
    int s = 0;
    for (size_t i = 0; i < countof_ns(a); i++) {
        s += a[i];
    }
    return s;
}

template<size_t N, size_t M>
void sum2(int (&a)[N][M]) {
    static_assert(N == countof_ns(a), "sum2: N == countof_ns(a)");
    std_size_compare(N == std::size(a));
    static_assert(M == countof_ns(*a), "sum2: M == countof_ns(*a)");
    std_size_compare(M == std::size(*a));
    int s = 0;
    for (size_t i = 0; i < countof_ns(a); i++) {
        for (size_t j = 0; i < countof_ns(a[0]); j++) {
            s += a[i][j];
        }
    }
    return s;
}

template<size_t N>
void bad1(int a[N]) {
    (void)countof_ns(a);
    (void)std_size(a);
}




int main() {

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
