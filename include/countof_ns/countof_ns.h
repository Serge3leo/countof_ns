// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
//
//
// Implementing the macro `countof()' C2y draft for C23/C++14
// ==========================================================
//
// The macro `countof_ns(array)` returns the number of elements in its operand.
// The number of elements is determined by the type of the operand. The result
// is an integer. For variable-length arrays (VLA), the operand is evaluated,
// otherwise the operand is not evaluated, and the result is an integer
// constant expression.
//
// For standard C/C++14 arrays, the result is identical to `countof()`.
// Otherwise, if the argument is not an array, a compilation error occurs,
// i.e., the behavior is completely identical to `countof()`.
//
// If the argument is an extended zero-length array (ZLA), or contains such
// arrays, or contains zero-size elements, then:
//
// - For fixed arrays, C++14 produces an identical result to `countof()`;
//
// - For fixed arrays C, in the case of `sizeof(*array) == 0`, if the number If
//   there are 0 elements, then 0 will be returned, otherwise an error occurs.
//   compilation (the behavior depends on the capabilities of the compiler, on
//   some compilers can always return 0);
//
// - For VLA, in the case of `sizeof(*array) == 0`, always returns 0.
//
// Warning: In the case of multidimensional VLAs, the number of argument
// evaluations is compiler dependent and different from `countof()`. To check,
// you can use third-party analyzers, for example, together with the Clang
// compiler you can use:
//
// $ clang-tidy -config="{
//     Checks: bugprone-assert-side-effect,
//     CheckOptions: {
//         bugprone-assert-side-effect.AssertMacros:
//             'assert,NSAssert,NSCAssert,sizeof,countof,countof_ns'
//     }
// }" ...
//
//
// Requirements
// ============
//
// - C23 or C++14;
//
// - C99 for Clang (clang), GNU (gcc), classic Intel (icc), IntelLLVM (icx),
//   LCC (MCST Elbrus), MSVC (Visual Studio 2022), NVHPC (NVIDIA HPC Compiler),
//   Pelles C, PGI (The Portland Group(?)), SunPro (Oracle Developer Studio),
//   XL (IBM XL C/C++ for AIX), XLClang (IBM Clang-based XL).
//
//
// Usage
// =====
//
//    #include "countof_ns/countof_ns.h"
//
// Before the first include, if necessary, you can define the following macros
// to the value 1:
//
// - `_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P` - the
//   `__builtin_types_compatible_p()` function does not check for full VLA
//   matching of arrays: `static_assert(!__builtin_types_compatible_p(int
//   (*)[d1], int (*)[d1]))`. For compilers: classic Intel (icc), LCC (lcc),
//   and NVHPC(pgcc), this does not need to be specified; it is taken into
//   account automatically;
//
// - `_COUNTOF_NS_BROKEN_TYPEOF` - The C extension `__typeof__()` removes
//   qualifiers, i.e., functions similarly to `typeof_unqual()` in C23. For
//   compilers: classic Intel (icc), LCC (lcc), and NVHPC (pgcc), no need to
//   specify, automatically taken into account;
//
// - `_COUNTOF_NS_REFUSE_VLA` - no VLA support (VLA arguments cause a compile
//   error). This is used by default if the symbols `__STDC_NO_VLA__` or
//   `_MSC_VER` are defined (MS Visual Studio 2022 and later);
//
// - `_COUNTOF_NS_WANT_KR` - use the K&R idiom without using `__typeof__()',
//   `typeof()' etc. For C, it is used by default if `_MSC_VER < 1939` (MS
//   Visual Studio 2019 and earlier);
//
//   WARNING: The GNU (gcc) warnings `-Wsizeof-pointer-div` and
//            `-Wsizeof-array-argument` are disabled for system headers
//            (located in `/usr/include` or `/usr/local/include`, depending on
//            the system). The `countof_ns.h` header should be placed outside
//            of them, or refer to it directly, for example with the option
//            `--include`. Clang (clang) and IntelLLVM (icx) work fine.
//
// - `_COUNTOF_NS_WANT_STDC` - do not use extensions: `__typeof__()` and
//   others;
//
// - `_COUNTOF_NS_WANT_VLA_BUILTIN` - use built-in functions
//   (`__builtin_types_compatible_p()` for C or `__is_same()` for C++ and
//   others). This is used by default if `__STDC_NO_VLA__`, `_MSC_VER`,
//   `__POCC__`, or `_COUNTOF_NS_WANT_STDC` are not defined;
//
// - `_COUNTOF_NS_WANT_VLA_C11` - to provide VLA support with standard C11/C23
//   features, requires compiler compliance with `6.5.6 Additive operators` C11
//   and higher, for all types of arguments. This is used by default if the
//   symbol `__POCC__` is defined (Pelles C).
//
// Comparison of VLA support options
// ---------------------------------
//
// - If the priority is minimizing compilation errors on various compilers,
//   then if VLA is required, `_COUNTOF_NS_WANT_VLA_C11` should be preferred;
//
// - If the priority is correctness of the constraint check, then
//   `_COUNTOF_NS_WANT_VLA_BUILTIN` is preferable, since constraint checks do
//   not depend on compiler switches (but, unfortunately, problematic compilers
//   do not support `__has_builtin()`).
//
// The `_COUNTOF_NS_WANT_VLA_BUILTIN` macro also allows you to document
// compiler settings:
//
// $ icc -diag-error=1121
//     '-D_countof_ns_must_compatible(p,t1,t2)=(0*sizeof((t1)(p)-(t2)(p)))'
//     -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
// $ pgcc --diag_error=nonstandard_ptr_minus_ptr
//     '-D_countof_ns_must_compatible(p,t1,t2)=(0*sizeof((t1)(p)-(t2)(p)))'
//     -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
// $ suncc -errwarn=E_BAD_POINTER_SUBTRACTION
//     '-D_countof_ns_must_compatible(p,t1,t2)=(0*sizeof((t1)(p)-(t2)(p)))'
//     -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
// $ xlc -qlanglvl=extc1x -qhaltonmsg=1506-068
//       -qinclude=_countof_ns_must_compatible_xl.h
//       -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
// It is unlikely that anyone will need `_COUNTOF_NS_WANT_VLA_BUILTIN` for
// MSVC, but for testing, it's also possible:
//
// > cl /std:clatest /wd4116 /we4047 /we4048 ^
//      /FI_countof_ns_must_compatible_msvc.h ^
//      /D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
// - Alternatively, the following keys can be used for Clang, GNU, and
//   INTELLVM: `-D_COUNTOF_NS_WANT_KR -Werror=sizeof-pointer-div
//   -Werror=sizeof-array-decay -Werror=sizeof-array-argument'.
//
//
// VLA Support Status
// ==================
//
// This header file defines:
//
// - `_COUNTOF_NS_USE_BUILTIN`, if built-in functions are used to support VLAs,
//   etc.
//
// - `_COUNTOF_NS_USE_GENERIC`, if `_Generic()` is used
//   (`_COUNTOF_NS_VLA_UNSUPPORTED`);
//
// - `_COUNTOF_NS_USE_SUBTRACTION`, if subtraction of pointers by C11/C23 is
//   used to support VLAs;
//
// - `_COUNTOF_NS_USE_TEMPLATE`, if standard C++ templates are used
//   (`_COUNTOF_NS_VLA_UNSUPPORTED`);
//
// - `_COUNTOF_NS_VLA_UNSUPPORTED` in case VLA support is not provided;
//
//
// Disclaimer
// ==========
//
// Sorry for my best English. Alas, this file is actually a yandex translation
// of [countof_ns.h.ru.md](../countof_ns.h.ru.md) with minimal editorial
// changes.

#ifndef COUNTOF_NS_H_6951
#define COUNTOF_NS_H_6951
#define _COUNTOF_NS  (202601L)

#include <stddef.h>

    // Avoid uncertain zero-by-zero divide
#if __SUNPRO_C
        // - SunPro ZLA extension limited: can't sizeof((ZLA)[0]);
        // - SunPro incomplete implementation Conditionals with Omitted (?:);
        // - C++ containers implement only `[]` operator;
    #define _countof_ns_unsafe(a)  \
                (0 == sizeof(*(a)) ? 0 : sizeof(a)/sizeof(*(a)))
#elif __NVCOMPILER
        // Probably NVHPC (pgcc) bug (HAVE_BROKEN_VLA et all)
    #define _countof_ns_unsafe(a)  (0 == sizeof((a)[0]) ? 0 \
              : sizeof(a)/(sizeof((a)[0]) ? sizeof((a)[0]) : 2*sizeof(void *)))
    #define _countof_ns_not_kr_idiom_  (1)
#elif !__GNUC__
    #define _countof_ns_unsafe(a)  \
                (0 == sizeof((a)[0]) ? 0 : sizeof(a)/sizeof((a)[0]))
#else
        // Assume: __GNUC__ => has Conditionals with Omitted (?:)
        //         sizeof((a)[0]) == 0 => sizeof(a) == 0 or sizeof(void *)
    #define _countof_ns_unsafe(a)  \
                        (sizeof(a)/( sizeof((a)[0]) ?: 2*sizeof(void *) ))
    #define _countof_ns_not_kr_idiom_  (1)
#endif
#if _COUNTOF_NS_WANT_KR || (_MSC_VER < 1939 && _MSC_VER && !__cplusplus)
    #define _COUNTOF_NS_USE_KR (1)
    #if _countof_ns_not_kr_idiom_
            // For compilers, analyzers and warnings (as errors)
        #define _countof_ns_must_array(a)  (0*sizeof(sizeof(a)/sizeof((a)[0])))
    #else
        #define _countof_ns_must_array(a)  (0)
    #endif
    #define countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_array(a))
#elif !__cplusplus
    #if _COUNTOF_NS_WANT_VLA_C11 || _COUNTOF_NS_WANT_STDC
        #define _COUNTOF_NS_USE_SUBTRACTION  (1)
    #elif _COUNTOF_NS_WANT_VLA_BUILTIN
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #elif _COUNTOF_NS_REFUSE_VLA || __STDC_NO_VLA__
        #define _COUNTOF_NS_USE_GENERIC  (1)
    #elif __IBMC__ || __SUNPRO_C
        #ifdef _countof_ns_must_compatible
            #define _COUNTOF_NS_USE_BUILTIN  (1)
        #else
            #define _COUNTOF_NS_USE_GENERIC  (1)
            #warning "For SunPro/IBM XL, you must define either _COUNTOF_NS_REFUSE_VLA or _countof_ns_must_compatible()"
        #endif
    #elif __POCC__
        #define _COUNTOF_NS_USE_SUBTRACTION  (1)
    #else
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #endif
    #if __STDC_VERSION__ >= 202311L
        #define _countof_ns_assert  static_assert
        #define _countof_ns_typeof(t)  typeof(t)
    #elif !_COUNTOF_NS_WANT_STDC
        #define _countof_ns_assert  _Static_assert
        #if !defined(_countof_ns_must_compatible) && \
            (_COUNTOF_NS_BROKEN_TYPEOF || _COUNTOF_NS_USE_BUILTIN)
            #define _countof_ns_typeof(t)  const volatile __typeof__(t)
        #else
            #define _countof_ns_typeof(t)  __typeof__(t)
        #endif
    #else
        #error "With _COUNTOF_NS_WANT_STDC required C23 typeof(t)"
    #endif
    #if _COUNTOF_NS_USE_BUILTIN || _COUNTOF_NS_USE_SUBTRACTION
        #if !_COUNTOF_NS_USE_BUILTIN
            #define _countof_ns_must(c)  (c)
            #define _countof_ns_must_compatible(p, t1, t2) \
                                        (0*sizeof((t1)(p) - (t2)(p)))
        #elif !defined(_countof_ns_must_compatible)
            #if __ORANGEC__
                #define _countof_ns_must_compatible(p, t1, t2) \
                                               (!__is_same(t1, t2))
                #define _countof_ns_must(c)  \
                                (0*sizeof(struct{unsigned foo:(!(c));}))
            #elif defined(__has_builtin)
                #if __has_builtin(__builtin_types_compatible_p) && \
                    !__NVCOMPILER && !__LCC__ && \
                    !_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P || \
                    __ibmxl__
                    #define _countof_ns_must_compatible(p, t1, t2) \
                                    (!__builtin_types_compatible_p(t1, t2))
                #endif
            #endif
            #ifndef _countof_ns_must_compatible
                    // Workaround, implement is_pointer_v<decltype(**(ppa))>
                #define _countof_ns_must_compatible(ppa, t1, t2)  \
                                (__builtin_types_compatible_p( \
                                    _countof_ns_typeof(&*(**(ppa))), \
                                    _countof_ns_typeof(**(ppa))))
            #endif
        #endif
        #ifndef _countof_ns_must_compatible
            #error "Not __builtin_types_compatible_p() or _countof_ns_must_compatible()"
        #endif
        #ifndef _countof_ns_must
            #define _countof_ns_must(c)  (0*sizeof(struct { int _countof_ns; \
                            _countof_ns_assert(!(c), "Must be array"); }))
        #endif
            // Constraints `a` is array and have `_countof_ns_unsafe(a)`
            // elements (for VLA, number elements is unconstrained).
            //
            // Warning: only in the case C extensions of arrays that contain or
            // may contain zero-size elements (multidimensional ZLAs, etc.),
            // the operation subtracting the pointer to the input array and the
            // pointer to the array with zero elements will be performed.  For
            // C11/C23, this is undefined behavior, if array have non zero
            // zero-size elements.  However, a common C language extension for
            // VLAs is that these pointer types are compatible, and the
            // operation is valid (at compile time).
            //
            // As result:
            //     static_assert(sizeof(T0) == 0);
            //     T0 a[0];  // Constraints OK - "is array"
            //     T0 b[1];  // Constraints FAIL - "is not array"
            //     size_t u = ...;
            //     T0 c[u];  // Constraints OK - "is array", at compile time,
            //               // for any `u`
        #define _countof_ns_must_array(a) \
                    _countof_ns_must(_countof_ns_must_compatible( \
                        (_countof_ns_typeof(a) **)&(a), \
                        _countof_ns_typeof(a) **, \
                        _countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)]))
    #else
        #if __STDC_VERSION__ < 202601L
            #define _COUNTOF_NS_VLA_UNSUPPORTED  (1)
        #endif
            // Constraints `a` is fixed array and have `_countof_ns_unsafe(a)`
            // elements (don't have variably modified type, i.e. not VLA, not
            // contains VLA etc).
        #define _countof_ns_must_array(a)  (_Generic( \
                    (_countof_ns_typeof(a) *)&(a), \
                    _countof_ns_typeof(*(a))(*)[_countof_ns_unsafe(a)]: 0))
    #endif
    #define countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_array(a))
#else
    #include <type_traits>
    namespace _countof_ns_ {

    #if _COUNTOF_NS_REFUSE_VLA || _MSC_VER
            // _MSC_VER is the only compiler without support for the C++
            // VLA extension.
        #define _COUNTOF_NS_USE_TEMPLATE  (1)
    #else
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #endif

        // Unchecked size stub, only for compilation success
    constexpr size_t unthinkable = 1917;
    constexpr size_t bias = 1;
    class no_t { char no_[1]; };
    class yes_t { long long yes_[2]; };
    static_assert(sizeof(no_t) != sizeof(yes_t), "Internal error");
        // T is container (has `size()` member)
    template <class T> struct has_size {
        template <class C> static yes_t test_(decltype(&C::size));
        template <class> static no_t test_(...);
        static constexpr bool value = sizeof(test_<T>(0)) == sizeof(yes_t);
    };
        // T is ZLA
    template<class T>
    struct is_zla : std::integral_constant<bool, 0 == sizeof(T) &&
                                0 == std::extent<T>::value &&
                                !std::is_class<T>::value &&
                                !std::is_function<T>::value &&
                                !std::is_scalar<T>::value &&
                                !std::is_union<T>::value &&
                                !std::is_void<T>::value> {};
        // Count of fixed array, standard C++
    template<class T, size_t N> static char (*match(const T (&)[N]))[bias + N];
        // Count of ZLA
    template <class T, typename std::enable_if<
                                is_zla<T>::value, int>::type = 0>
    static char (*match(const T&))[bias + 0];
        // Container (stub)
    template <class C, typename std::enable_if<
                                has_size<C>::value, int>::type = 0>
    static char (*match(const C&))[unthinkable];
        // Count of container
    template <class C, typename std::enable_if<
                                has_size<C>::value, int>::type = 0>
    constexpr static auto cnt_size(const C &c) noexcept(noexcept(c.size())) {
        return c.size();
    }
    constexpr static auto cnt_size(...) noexcept { return unthinkable; }

    #if _COUNTOF_NS_USE_TEMPLATE
            // C++ with ZLA extension only
        #define _COUNTOF_NS_VLA_UNSUPPORTED  (1)
        #define countof_ns(a)  (_countof_ns_::has_size<decltype(a)>::value \
                                ? _countof_ns_::cnt_size(a) \
                                : sizeof(*_countof_ns_::match(a)) - \
                                  _countof_ns_::bias)
    #elif _COUNTOF_NS_USE_BUILTIN // && !__SUNPRO_CC
            // C++ with VLA extension
        template<bool IsArray>
        constexpr static size_t zero_assert() noexcept {
            static_assert(IsArray, "Must be VLA");
            return 0;
        }
            // Argument type is not variably modified (VM) type
        #if !__SUNPRO_CC
                // Number compilers HAVE_HIDDEN_IS_SAME_CXX
            #define _countof_ns_must_vla(a)  (_countof_ns_::zero_assert< \
                                !__is_same(decltype(&(a)[0]), decltype(a))>())
            template<class T>
            static yes_t match_not_vmt(const T&);
            // !match_not_vmt => VLA, pointer to VLA or fixed array of VLA
        #else
            #define _countof_ns_must_vla(a)  (_countof_ns_::zero_assert< \
                                !std::is_pointer<decltype(a)>::value>())
            template <class T, typename std::enable_if<
                                    !std::is_array<T>::value ||
                                    0 < std::extent<T>::value, int>::type = 0>
            static yes_t match_not_vmt(const T&);
            // !match_not_vmt => VLA or ZLA
        #endif
        static no_t match_not_vmt(...);
            // Count of VLA
        #define _countof_ns_vla(a)  \
                        (_countof_ns_unsafe(a) + _countof_ns_must_vla(a))
            // Argument is container
        template <class C, typename std::enable_if<
                                    has_size<C>::value, int>::type = 0>
        static yes_t match_cnt(const C&);
        static no_t match_cnt(...);
            // Count of fixed array (or ZLA)
        template <class T>
        static auto match(yes_t not_vmt, const T& a) -> decltype(match(a));
            // VLA match stub
        static char (*match(no_t not_vmt, ...))[unthinkable];
        #define countof_ns(a)  (sizeof(_countof_ns_::match_not_vmt(a)) == \
                                sizeof(_countof_ns_::no_t) \
                                ? _countof_ns_vla(a) \
                                : sizeof(_countof_ns_::match_cnt(a)) == \
                                  sizeof(_countof_ns_::yes_t) \
                                    ? _countof_ns_::cnt_size(a) \
                                    : sizeof(*_countof_ns_::match( \
                                        _countof_ns_::match_not_vmt(a), \
                                        (a))) - \
                                      _countof_ns_::bias)
    #endif
    }  // of namespace
#endif

#endif // COUNTOF_NS_H_6951
