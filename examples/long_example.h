// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// The second part of the example depends on the capabilities of the compiler:
//   1. Zero-length arrays and/or arrays of zero-size objects - zla_example();
//   2. Variable-length arrays - vla_example();
//   3. Qualifiers `const` and `volatile` - cv_example();
//   4. Comparison with `countof()` (draft C2y) - countof_example();
//   5. Comparison with `std::size()` (C++17) - std_size_example().
//
// `long_example.h` compilation options:
//
// EXAMPLE_FAIL - Include examples for compilation errors with erroneous
//                parameters;
//
// EXAMPLE_VLA_C11_ENABLE - Variable-length arrays (VLA) C examples;
//
// EXAMPLE_VLA_BUILTIN_ENABLE - Variable-length arrays (VLA) C/C++ examples;
//
// HAVE_ZLA - The compiler supports the C/C++ extension for
//                           zero-length arrays (clang, gcc, intel, nvidia...);
//
// HAVE_ALONE_FLEXIBLE_ARRAY - The compiler supports the C/C++ extension for
//                             structures with only Flexible Array Members or
//                             unions unions with Flexible Array Members
//                             (clang, gcc, modern intel);
//
// HAVE_COUNTOF - The compiler supports the C2y `countof()` (clang 21,
//                IntelLLVM 2025.3, GNU gcc 16);
//
// HAVE_EMPTY_INITIALIZER - The compiler supports the C/C++ extension for empty
//                          initializer;
//
// HAVE_EMPTY_STRUCTURE - The compiler supports the C/C++ extension for
//                        Structures with No Members. The structure has size
//                        zero. In C++, empty structures are part of the
//                        language. C++ treats empty structures as if they had
//                        a single member of type char;
//
// __STDC_NO_VLA__ - The C compiler don't supports optional variable-length arrays
//            (MSVC).
//
// HAVE_BROKEN_VLA - VLA implementation don't support two-dimensional
//                   VLA. See "tests/autoconf/have_vla.h". Check:
//
//     ${CC} tests/autoconf/no_have_broken_vla.c && ./a.out
//
// HAVE_VLA_CXX - The compiler supports the C++ extension variable-length
//                arrays (clang, gcc, intel, nvidia, PGI...);
//
// HAVE_BROKEN_VLA_CXX - C++ VLA extension don't support two-dimensional
//                       VLA. "tests/autoconf/have_vla.h". Check:
//
//     ${CXX} -std=c++14 -DHAVE_VLA_CXX \
//            tests/autoconf/no_have_broken_vla_cxx.cpp && ./a.out
//

#if ENABLE_VLA_C11_EXAMPLE || ENABLE_VLA_BUILTIN_EXAMPLE
        // TODO, need change, but... By default, `countof_ns()` causes
        // compilation errors if the argument is a VLA array. This is often the
        // most expected behavior, see the README for details.

    #if !__cplusplus && !__STDC_NO_VLA__ && !defined(HAVE_BROKEN_VLA)
        #define HAVE_VLA  (1)
    #endif
    #if defined(HAVE_VLA_CXX) && defined(HAVE_BROKEN_VLA_CXX)
        #undef HAVE_VLA_CXX
    #endif
    #if !defined(HAVE_VLA) && !defined(HAVE_VLA_CXX)
        #pragma message ("VLA support don't detected, may be broken " \
                         "(as some `pgcc`)")
    #endif
    #ifdef countof_ns
        #pragma message ("_COUNTOF_NS_WANT_VLA_* don't not affected. " \
                         "Probably the above was #include \"countof_ns.h\"")
    #endif
    #if ENABLE_VLA_C11_EXAMPLE
        #define _COUNTOF_NS_WANT_VLA_C11  (1)
    #else
        #define _COUNTOF_NS_WANT_VLA_BUILTIN  (1)
    #endif
#endif

#include "countof_ns.h"
#include "short_example.h"

static void zla_example(void) {
#ifdef HAVE_ZLA
    size_t fail = 0;
    int z1[0];
    int z2[0][0];
    int z3[5][0];
    int z4[0][5];
    int z5[5][countof_ns(z4)][5];

    example_assert(0 == sizeof(z1) && 0 == sizeof(z2) && 0 == sizeof(z3) &&
                   0 == sizeof(z4) && 0 == sizeof(z5));

    example_assert(0 == countof_ns(z1));
    example_assert(0 == countof_ns(z2));  // 0, if complete object type is T[0]
    example_assert(0 == countof_ns(z4));
    example_assert(0 == countof_ns(z2[0]));  // z2[0] - don't evaluated
    example_assert(0 == countof_ns(z3[0]));
    example_assert(5 == countof_ns(z4[0]));
    example_assert(0 == countof_ns(z5[0]));
    example_assert(5 == countof_ns(z5[0][0]));
    #if __cplusplus
        example_assert(5 == countof_ns(z3));
        example_assert(5 == countof_ns(z5));
    #elif EXAMPLE_FAIL && defined(__NVCOMPILER)
        #warning "TODO for pgcc (aka nvc) 25.9 & HAVE_ZLA"
    #elif EXAMPLE_FAIL
                            #pragma message ("Must error below @{")
        fail += countof_ns(z3);  // compile error, if complete object type not
                                 // T[0], to avoid zero-by-zero indeterminate
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(z5);
                            #pragma message ("}@ Must error above")
    #endif
    #if defined(EXAMPLE_FAIL)
                            #pragma message ("Must error below @{")
        fail += countof_ns((int *)z1);
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(&z1[0]);
                            #pragma message ("}@ Must error above")
    #endif

    #ifdef HAVE_ALONE_FLEXIBLE_ARRAY
        printf("HAVE_ALONE_FLEXIBLE_ARRAY\n");
        struct {
            int flex[];
        }       f1[0], f2[5];

        example_assert(0 == sizeof(f1) && 0 == sizeof(f2));

        example_assert(0 == countof_ns(f1));
        #ifdef __cplusplus
            example_assert(5 == countof_ns(f2));
        #elif defined(EXAMPLE_FAIL)
                            #pragma message ("Must error below @{")
            fail += countof_ns(f2);
                            #pragma message ("}@ Must error above")
        #endif

        union {
            char flex_c[];
            int flex_i[];
        }       u1[0], u2[5];

        example_assert(0 == sizeof(u1) && 0 == sizeof(u2));

        example_assert(0 == countof_ns(u1));
        #ifdef __cplusplus
            example_assert(5 == countof_ns(u2));
        #elif defined(EXAMPLE_FAIL)
                            #pragma message ("Must error below @{")
            fail += countof_ns(u2);
                            #pragma message ("}@ Must error above")
        #endif
    #endif

    #ifdef HAVE_EMPTY_INITIALIZER
        #if ((4 < __GNUC__ && __GNUC__ < 11) || \
             defined(__GNUC_WIDE_EXECUTION_CHARSET_NAME)) && \
            !defined(__cplusplus)
            // TODO: How can you distinguish genuine gcc from counterfeit?
            #pragma message ("TODO for gcc & HAVE_EMPTY_INITIALIZER")
        #else
            printf("HAVE_EMPTY_INITIALIZER\n");
            int e1[] = {};
            int e2[0][countof_ns(e1)];
            int e3[5][countof_ns(e2)];
            #if __cplusplus
                decltype(e1) e4[1917];
            #else
                _countof_ns_typeof(e1) e4[1917];
            #endif

            example_assert(0 == sizeof(e1) && 0 == sizeof(e2) &&
                           0 == sizeof(e3) && 0 == sizeof(e4));

            example_assert(0 == countof_ns(e1));
            example_assert(0 == countof_ns(e2));
            #ifdef __cplusplus
                example_assert(5 == countof_ns(e3));
                example_assert(1917 == countof_ns(e4));
            #elif defined(EXAMPLE_FAIL)
                            #pragma message ("Must error below @{")
                fail += countof_ns(e3);
                            #pragma message ("}@ Must error above & below @{")
                fail += countof_ns(e4);
                            #pragma message ("}@ Must error above")
            #endif
            (void)e1;
        #endif
    #endif

    #if defined(HAVE_EMPTY_STRUCTURE) && !defined(__cplusplus)
        printf("HAVE_EMPTY_STRUCTURE\n");
        struct {
        }       s1[0], s2[5];

        example_assert(0 == sizeof(s1) && 0 == sizeof(s2));

        example_assert(0 == countof_ns(s1));
        #ifdef EXAMPLE_FAIL
                            #pragma message ("Must error below @{")
            fail += countof_ns(s2);
                            #pragma message ("}@ Must error above")
        #endif
    #endif

    printf("%s: Ok (fail=%zu)\n", __func__, fail);
#endif
}

static void vla_example() {
#if (HAVE_VLA || HAVE_VLA_CXX) && !_COUNTOF_NS_VLA_UNSUPPORTED
    size_t fail = 0;
    size_t ba = 42;
    size_t fa = 56;
    size_t tt = 23;
    size_t xx = 12;
    size_t xy = 3;
    size_t xz = 2;
    int a1[ba];
    int a2[fa][tt];
    int *p2 = a1;
    int **p3 = &p2;

    assert(ba*sizeof(a1[0]) == sizeof(a1));
    assert(ba == countof_ns(a1));

    assert(fa*sizeof(a2[0]) == sizeof(a2));
    assert(fa*tt*sizeof(a2[0][0]) == sizeof(a2));
    assert(fa == countof_ns(a2));

    assert(tt*sizeof(a2[0][0]) == sizeof(a2[0]));
    assert(tt == countof_ns(a2[0]));

    int c[countof_ns(a2[0])];
    assert(tt == countof_ns(c));

    assert(xx == countof_ns(*(int (*)[xx])&p2));
    assert(xy == countof_ns(*(int (*)[xy])p3));
    assert(xz == countof_ns(*(int(*)[xz][xz])&p3));

    #if !__cplusplus
        int (*p1)[tt] = &a2[0];
        assert(tt == countof_ns(*p1));
        (void)p1;
    #endif
    (void)p3;
    #ifdef EXAMPLE_FAIL
                            #pragma message ("Must error below @{")
        example_assert(ba == countof_ns(a1));
                            #pragma message ("}@ Must error above & below @{")
        int a3[z] = { 0 };
        struct {
            int bits: countof_ns(a3);
        } s = { 0 };
        (void)a3[s.bits];
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(p1);
                            #pragma message ("}@ Must error above & below @{")
        fail += countof_ns(p2);
                            #pragma message ("}@ Must error above")
    #endif

    #ifdef HAVE_ZLA
        size_t n = 0;
        size_t f = 5;
        int vz1[n][n];
        int vz2[countof_ns(vz1)][f];
        int vz3[countof_ns(vz2[0])][n];

        assert(0 == sizeof(vz1) && 0 == sizeof(vz2) && 0 == sizeof(vz3));

        assert(0 == countof_ns(vz1));  // 0, if complete object type is T[0]
        assert(0 == countof_ns(vz2));
        #if __APPLE__ && ((4 < __GNUC__ && __GNUC__ < 11) || \
             defined(__GNUC_WIDE_EXECUTION_CHARSET_NAME)) && \
            !defined(__cplusplus)
            // TODO: How can you distinguish genuine gcc from counterfeit?
            #pragma message ("TODO for macOS gcc & HAVE_EMPTY_INITIALIZER")
        #else
#if !(__APPLE__ && defined(__clang__) && __clang_major__ <= 15)
        assert(0 == countof_ns(vz3));  // But all VLA T[x] have compatible
                                       // complete types. We can't avoid
                                       // zero-by-zero indeterminate for VLA.
#endif
        #endif
        assert(0 == countof_ns(vz1[0]));
        assert(5 == countof_ns(vz2[0]));
        assert(0 == countof_ns(vz3[0]));
    #endif

    printf("%s: Ok (fail=%zu)\n", __func__, fail);
#endif
}

static void cv_example() {
    const int a1[1] = { 0 };
    volatile int a2[2][countof_ns(a1)];
    const volatile int a3[3][countof_ns(a2)][countof_ns(a2[0])] = {{{ 0 }}};

    example_assert(1 == countof_ns(a1));
    example_assert(2 == countof_ns(a2));
    example_assert(3 == countof_ns(a3));
    example_assert(1 == countof_ns(a2[0]));
    example_assert(2 == countof_ns(a3[0]));
    example_assert(1 == countof_ns(a3[0][0]));

    printf("%s: Ok\n", __func__);
    (void)a1;
}

#ifdef __has_include
    #if __has_include(<stdcountof.h>)
        #include <stdcountof.h>
    #endif
#endif

static void countof_example() {
#if defined(HAVE_COUNTOF) && !defined(__cplusplus)
    int a1[1];
    int a9[9];

    static_assert(_Generic(__typeof__(countof(a1)),
                           __typeof__(countof_ns(a1)) : 1, default : 0));
    static_assert(countof(a1) == countof_ns(a1));
    static_assert(countof(a9) == countof_ns(a9));

    #ifdef HAVE_ZLA
        int z1[0][0];
        int z2[countof(z1)][5];
        int z3[countof(z2[0])][0];

        static_assert(countof(z1) == countof_ns(z1));
        static_assert(countof(z2) == countof_ns(z2));
        static_assert(countof(z3) == 5);  // But countof_ns() - compile error

        #if (HAVE_VLA || HAVE_VLA_CXX) && !_COUNTOF_NS_VLA_UNSUPPORTED
            const size_t n = 0;
            const size_t f = 5;
            int vz1[n][n];
            int vz2[countof(vz1)][f];
            int vz3[countof(vz2[0])][n];

            assert(countof(vz1) == countof_ns(vz1));
            assert(countof(vz2) == countof_ns(vz2));
            assert(countof(vz3) == 5);
            assert(0 == countof_ns(vz3));  // zero-by-zero indeterminate for VLA
        #endif
    #endif
    #if (HAVE_VLA || HAVE_VLA_CXX) && !_COUNTOF_NS_VLA_UNSUPPORTED
        const size_t d = 2;
        int v[d];

        static_assert(_Generic(__typeof__(countof_ns(v)),
                      __typeof__(countof_ns(v)) : 1, default : 0));
        assert(countof(v) == countof_ns(v));
    #endif

    printf("%s: Ok\n", __func__);
#endif
}

#ifdef __cplusplus
    // TODO #include <typeinfo>
    #include <vector>
#endif

static void std_size_example() {
#if __cplusplus >= 201703L || __cpp_lib_nonmember_container_access >= 201411L
    size_t fail = 0;
    int a1[1];
    int a9[9];

    static_assert(std::size(a1) == countof_ns(a1));
    static_assert(std::size(a9) == countof_ns(a9));

#if 0
    #if (4 < __GNUC__ && __GNUC__ <= 11) || \
        (__APPLE__ && defined(__clang__) && __clang_major__ <= 15)
        #pragma message ("TODO for old gcc & typeid()..")
    #else
        static_assert(typeid(std::size(a1)) == typeid(countof_ns(a9)));
        constexpr auto std_size_1 = std::size<int, 1>;
        constexpr auto std_size_2 = std::size<int, 2>;

        static_assert(typeid(std_size_1) != typeid(std_size_2));
        #if _COUNTOF_NS_VLA_UNSUPPORTED && !__NVCOMPILER
                // TODO: C++ VLA enabled version
            constexpr auto countof_ns_1 = _countof_ns_aux<sizeof(int),
                                                          sizeof(int[1]),
                                                          int, 1>;
            static_assert(typeid(std_size_1) == typeid(countof_ns_1));
        #endif
    #endif
#endif
    #ifdef HAVE_ZLA
        int z1[5][0];
        int z2[0][5];
        int z3[0][0]; (void)z3;

        static_assert(std::size(z1) == countof_ns(z1));
        static_assert(std::size(z2[0]) == countof_ns(z2[0]));

        #if defined(EXAMPLE_FAIL)
                            #pragma message ("Must error below @{")
            fail += std::size(z2);
                            #pragma message ("}@ Must error above & below @{")
            fail += std::size(z3);
                            #pragma message ("}@ Must error above")
        #endif
    #endif

    printf("%s: Ok (fail=%zu)\n", __func__, fail);
#endif
}

static void long_example() {
    short_example();
    zla_example();
    vla_example();
    cv_example();
    countof_example();
    std_size_example();
    #ifdef __clang_major__
        printf("__clang_major__.__clang_minor__ %d.%d ",
                __clang_major__, __clang_minor__);
    #endif
    #ifdef __GNUC__
        printf("__GNUC__.__GNUC_MINOR__ %d.%d ",
                __GNUC__, __GNUC_MINOR__);
    #endif
    #ifdef __LCC__
        printf("__LCC__.__LCC_MINOR__ %d.%d ",
                __LCC__, __LCC_MINOR__);
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
    #ifdef __SUNPRO_CC  // Oracle Developer Studio
        printf("__SUNPRO_CC 0x%x ", __SUNPRO_CC);
    #endif
    #ifdef __NVCC__  // NVIDIA HPC SDK
        printf("__CUDACC_VER_MAJOR__.__CUDACC_VER_MINOR__ %d.%d ",
                __CUDACC_VER_MAJOR__, __CUDACC_VER_MINOR__);
    #endif
    #ifdef __NVCOMPILER
        printf("__NVCOMPILER_MAJOR__.__NVCOMPILER_MINOR__ %d.%d ",
                __NVCOMPILER_MAJOR__, __NVCOMPILER_MINOR__);
    #endif
    #ifdef __INTEL_COMPILER
        printf("__INTEL_COMPILER %d ", __INTEL_COMPILER);
    #endif
    #ifdef __INTEL_LLVM_COMPILER
        printf("__INTEL_LLVM_COMPILER ", __INTEL_LLVM_COMPILER);
    #endif
    #ifdef __cplusplus
        #ifdef __cpp_lib_nonmember_container_access
            printf("__cpp_lib_nonmember_container_access %ld ",
                  (long)__cpp_lib_nonmember_container_access);
        #endif
        printf("__cplusplus %ld\n", __cplusplus);
    #else
        #if _COUNTOF_NS_VLA_UNSUPPORTED
            printf("_COUNTOF_NS_VLA_UNSUPPORTED ");
        #endif
        #ifdef __STDC_NO_VLA__
            printf("__STDC_NO_VLA__ %d ", __STDC_NO_VLA__);
        #endif
        printf("__STDC_VERSION__ %ld\n", __STDC_VERSION__);
    #endif
}
