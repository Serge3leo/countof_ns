// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
//
//
// Implementing the macro `countof()' C2y draft for C23/C++11
// ==========================================================
//
// The macro `countof_ns(array)` returns the number of elements in its operand
// (array or array type). The number of elements is determined by the type of
// the operand. The result is an integer. If the number of elements is
// variable, the operand is evaluated, otherwise the operand is not evaluated,
// and the result is an integer constant expression.
//
// For standard C/C++11 arrays, the result is identical to `countof()`.
// Otherwise, if the argument is not an array, a compilation error occurs,
// i.e., the behavior is completely identical to `countof()`.
//
// If the argument is an extended zero-length array (ZLA), or contains such
// arrays, or contains zero-size elements, then:
//
// - For fixed arrays, C++11 produces an identical result to `countof()`;
//
// - For fixed arrays C, in the case of `sizeof(*array) == 0`, if the number If
//   there are 0 elements, then 0 will be returned, otherwise an error occurs.
//   compilation (the behavior depends on the capabilities of the compiler, on
//   some compilers can always return 0);
//
// - For variable-length arrays (VLA) of the C standard or the C++ extension,
//   in the case of `sizeof(*array) == 0`, always returns 0.
//
//
// Requirements
// ============
//
// - C23 or C++11;
//
// - C99 with extensions `__typeof__()` and others (Clang, GNU gcc, Intel
//   (classic icc), IntelLLVM (icx), LCC (MCST Elbrus), MSVC (Visual Studio
//   2022), NVHPC (NVIDIA HPC Compiler), Pelles C, PGI (The Portland Group(?)),
//   SunPro (Oracle Developer Studio)).
//
//
// Usage
// =====
//
// Before the first include, you can define the following macros to the value 1:
//
// - `_COUNTOF_NS_WANT_VLA_BUILTIN` - use built-in functions
//   (`__builtin_types_compatible_p()` for C or `__builtin_constant_p()` and
//   `__is_same()` for C++ and others). This is used by default if
//   `__STDC_NO_VLA__`, `_MSC_VER`, `__POCC__`, or `_COUNTOF_NS_WANT_STDC` are
//   not defined;
//
// - `_COUNTOF_NS_WANT_VLA_C11` - to provide VLA support with standard C11/C23
//   features, requires compiler compliance with `6.5.6 Additive operators` C11
//   and higher, for all types of arguments. This is used by default if the
//   symbol `__POCC__` (Pelles C) is defined.
//
// - `_COUNTOF_NS_REFUSE_VLA` - no VLA support (VLA arguments cause a compile
//   error). This is used by default if the symbols `__STDC_NO_VLA__` or
//   `_MSC_VER` (Visual Studio) are defined.
//
// - `_COUNTOF_NS_BROKEN_TYPEOF` - The C extension `__typeof__()` removes
//   qualifiers, i.e., functions similarly to `typeof_unqual()` in C23. For
//   compilers: classic Intel (icc), LCC (lcc), and NVHPC (pgcc), no need to
//   specify, automatically taken into account;
//
// - `_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P` - the
//   `__builtin_types_compatible_p()` function does not check for full VLA
//   matching of arrays: `static_assert(!__builtin_types_compatible_p(int
//   (*)[d1], int (*)[d1]))`. For compilers: classic Intel (icc), LCC (lcc),
//   and NVHPC(pgcc), this does not need to be specified; it is taken into
//   account automatically;
//
// - `_COUNTOF_NS_WANT_STDC` - do not use extensions: `__typeof__()` and
//   others;
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
//     $ icc -diag-error=1121
//         -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))'
//         -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//     $ pgcc --diag_error=nonstandard_ptr_minus_ptr
//         -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))'
//         -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//     $ suncc -errwarn=E_BAD_POINTER_SUBTRACTION
//         -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))'
//         -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//     It is unlikely that anyone will need `_COUNTOF_NS_WANT_VLA_BUILTIN` for
//     MSVC, but for testing, it's also possible:
//
//     > cl /std:clatest /wd4116 /we4047 ^
//          /FI_countof_ns_ptr_compatible_type_msvc.h ^
//          /D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//
// VLA Support Status
// ==================
//
// This header file defines:
//
// - `_COUNTOF_NS_VLA_UNSUPPORTED` in case VLA support is not provided;
//
// - `_COUNTOF_NS_USE_GENERIC`, if `_Generic()` is used
//   (`_COUNTOF_NS_VLA_UNSUPPORTED`);
//
// - `_COUNTOF_NS_USE_TEMPLATE`, if standard C++ templates are used
//   (`_COUNTOF_NS_VLA_UNSUPPORTED`);
//
// - `_COUNTOF_NS_USE_SUBTRACTION`, if subtraction of pointers by C11/C23 is
//   used to support VLAs;
//
// - `_COUNTOF_NS_USE_BUILTIN`, if built-in functions are used to support VLAs,
//   etc.
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
#define _COUNTOF_NS  (202512L)

#include <stddef.h>

    // Avoid uncertain zero-by-zero divide
#if __NVCOMPILER
        // Probably NVHPC (pgcc) bug (HAVE_BROKEN_VLA et all?)
    #define _countof_ns_unsafe(a)  (0 == sizeof((a)[0]) ? 0 \
                : sizeof(a)/( sizeof((a)[0]) ? sizeof((a)[0]) : (size_t)-1 ))
#elif __SUNPRO_C
        // TODO: C/C++ or SunPro/others... ?
        // - SunPro ZLA extension limited: can't sizeof((ZLA)[0]);
        // - C++ containers implement only `[]` operator;
    #define _countof_ns_unsafe(a)  \
                (0 == sizeof(*(a)) ? 0 : sizeof(a)/sizeof(*(a)))
#else
        // Strange zero division warning from Clang (clang) and InteLLVM (icx),
        // only if it is accompanied by any other compilation errors
    #define _countof_ns_unsafe(a)  \
                (0 == sizeof((a)[0]) ? 0 : sizeof(a)/sizeof((a)[0]))
#endif

#if !__cplusplus
    #if _COUNTOF_NS_WANT_VLA_C11 || _COUNTOF_NS_WANT_STDC
        #define _COUNTOF_NS_USE_SUBTRACTION  (1)
    #elif _COUNTOF_NS_WANT_VLA_BUILTIN
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #elif _COUNTOF_NS_REFUSE_VLA || __STDC_NO_VLA__
        #define _COUNTOF_NS_USE_GENERIC  (1)
    #elif __SUNPRO_C || __IBMC__
        #ifdef _countof_ns_ptr_compatible_type
            #define _COUNTOF_NS_USE_BUILTIN  (1)
        #else
            #define _COUNTOF_NS_USE_GENERIC  (1)
            #warning "For SunPro/IBM XL, you must define either _COUNTOF_NS_REFUSE_VLA or _countof_ns_ptr_compatible_type(p, type)"
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
        #if !defined(_countof_ns_ptr_compatible_type) && \
            (_COUNTOF_NS_BROKEN_TYPEOF || _COUNTOF_NS_USE_BUILTIN)
            #define _countof_ns_typeof(t)  const volatile __typeof__(t)
        #else
            #define _countof_ns_typeof(t)  __typeof__(t)
        #endif
    #else
        #error "With _COUNTOF_NS_WANT_STDC required C23 typeof(t)"
    #endif
    #if _COUNTOF_NS_USE_BUILTIN
        #if !defined(_countof_ns_ptr_compatible_type)
            #if defined(__has_builtin)
                #if __has_builtin(__builtin_types_compatible_p) && \
                    !__NVCOMPILER && !__LCC__ && \
                    !_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P || \
                    __ibmxl__
                    #define _countof_ns_ptr_compatible_type(p, type)  \
                                __builtin_types_compatible_p( \
                                    _countof_ns_typeof(p), type)
                #endif
            #endif
            #if !defined(_countof_ns_ptr_compatible_type)
                #define _countof_ns_ptr_compatible_type(ppa, type)  \
                                (!__builtin_types_compatible_p( \
                                    _countof_ns_typeof(&*(**(ppa))), \
                                    _countof_ns_typeof(**(ppa))))
            #endif
        #endif
        #if !defined(_countof_ns_ptr_compatible_type)
            #error "Not __builtin_types_compatible_p() or _countof_ns_ptr_compatible_type()"
        #endif

            // Constraints `a` is array and have `_countof_ns_unsafe(a)`
            // elements (for VLA, number elements is unconstrained).
            //
            // Constraints identically C11 constraints of pointer
            // subtraction.  See below.
        #define _countof_ns_must_array(a)  \
                (0*sizeof(struct { int _countof_ns; _countof_ns_assert( \
                    _countof_ns_ptr_compatible_type( \
                        (_countof_ns_typeof(a) **)&(a), \
                        _countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)] \
                    ), "Must be array"); }))
    #elif _COUNTOF_NS_USE_SUBTRACTION
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
        #define _countof_ns_must_array(a)  (0*sizeof( \
                (_countof_ns_typeof(a) **)&(a) - \
                (_countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)])&(a)))
    #else
        #define _COUNTOF_NS_VLA_UNSUPPORTED  (1)
            // Constraints `a` is fixed array and have `_countof_ns_unsafe(a)`
            // elements (don't have variably modified type, i.e. not VLA, not
            // contains VLA etc).
        #define _countof_ns_must_array(a)  (_Generic( \
                    (_countof_ns_typeof(a) *)&(a), \
                    _countof_ns_typeof(*(a))(*)[_countof_ns_unsafe(a)]: 0))
    #endif
    #if !__LCC__ && !__SUNPRO_C && !__INTEL_COMPILER
        #define _countof_ns_typ2arr(a)  (*(_countof_ns_typeof(a) *)(void *)8192)
    #else
        #define _countof_ns_typ2arr(a)  (a)
    #endif
    #define _countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_array(a))
    #define countof_ns(a)  (_countof_ns(_countof_ns_typ2arr(a)))
#else
    #if _COUNTOF_NS_REFUSE_VLA || _MSC_VER
            // _MSC_VER is the only compiler without support for the C++
            // VLA extension.
        #define _COUNTOF_NS_USE_TEMPLATE  (1)
    #elif _COUNTOF_NS_WANT_VLA_CXX
        #define _COUNTOF_NS_USE_STUB  (1)
    #elif _COUNTOF_NS_WANT_VLA_BUILTIN
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #else
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #endif

        // Count of fixed size arrays, case: standard C++ array
    template<size_t A, size_t E, class T, size_t N>
    constexpr static size_t _countof_ns_aux(const T (&)[N]) noexcept {
        return N;
    }
        // Count of fixed size arrays, cases: container or C++ extensions ZLA
    template<size_t A, size_t E, class C>
    constexpr static size_t _countof_ns_aux(const C &c) {
        if constexpr (A > 0) {  // TODO: C++ 17, convert to class
            return c.size();
        }
        return 0;
    }

    template<bool Assert>
    constexpr static size_t _countof_ns_0_if_assert() noexcept {
        static_assert(Assert, "Must be array");
        return 0;
    }

    #if _COUNTOF_NS_USE_TEMPLATE
            // C++ with ZLA extension only
        #define _COUNTOF_NS_VLA_UNSUPPORTED  (1)
        #define _countof_ns(a)  (_countof_ns_aux<sizeof(a), sizeof((a)[0])>(a))
    #elif _COUNTOF_NS_USE_STUB
        #warning "There is no correct implementation in pure C++ (wait C++26?)"
        #define _countof_ns(a)  (_countof_ns_unsafe(a))
    #elif _COUNTOF_NS_USE_BUILTIN && !__SUNPRO_CC
            // C++ with VLA extension
        #if __LCC__
            // #define _countof_ns_must_array(a)  (_countof_ns_0_if_assert<
            //                 !__is_same(__typeof__(&*(a)), __typeof__(a))>())
            #define _countof_ns_must_array(a)  (_countof_ns_0_if_assert< \
                            !__is_same_as(decltype(&(a)[0]), decltype(a))>())
            #define _vla_countof_ns(a)  (_countof_ns_unsafe(a))
        #else
            #define _countof_ns_must_array(a)  (_countof_ns_0_if_assert< \
                                !__is_same(decltype(&(a)[0]), decltype(a))>())
            #define _vla_countof_ns(a)  (_countof_ns_unsafe(a) + \
                                         _countof_ns_must_array(a))
        #endif

            // Internal VLA stub, called only for case
            // HAVE_STRANGE_BUILTIN_CONSTANT_P_CXX
        template<size_t A, size_t E>
        constexpr static size_t _countof_ns_aux(...) noexcept {
            return size_t(-1917); // A value that is incredible to encounter
        }

            // For VLA clip not constexpr argument to constexpr 0
        #define _countof_ns_2cexp(a, v)  (__builtin_constant_p(sizeof(a)) \
                                         ? (v) : 0)

            // For VLA clip template arguments to constexpr 0
        #define _countof_ns_fix(a)  (_countof_ns_aux<\
                             _countof_ns_2cexp((a), sizeof(a)), \
                             _countof_ns_2cexp((a), sizeof((a)[0]))>(a))

        #if 0   // !HAVE_STRANGE_BUILTIN_CONSTANT_P_CXX
                // Clang/GNU/LCC/NHPC (pgcc Nvidia HPC) - work fine
                //
                // If sizeof(a) - compile-time constant, then count fixed size
                // array , otherwise count VLA.
            #define _countof_ns(a)  (!__builtin_constant_p(sizeof(a)) \
                                     ? _vla_countof_ns(a) \
                                     : _countof_ns_fix(a))
        #else   // HAVE_STRANGE_BUILTIN_CONSTANT_P_CXX
                // But Intel and IntelLLVM compilers sometimes considers:
                // __builtin_constant_p(sizeof(VLA)) == 1
                //
                // See: tests/autoconf/have_strange_builtin_constant_p_cxx.cpp
            #define _countof_ns(a)  (!__builtin_constant_p(sizeof(a)) \
                                     ? _vla_countof_ns(a) \
                                     : size_t(-1917) == _countof_ns_fix(a) \
                                            ? _vla_countof_ns(a) \
                                            : _countof_ns_fix(a))
        #endif
    #else  // _COUNTOF_NS_USE_BUILTIN && __SUNPRO_CC
           // Advanced SunPro templates. SunPro C++ prohibits arrays containing
           // zero-length objects, particularly multidimensional ZLAs.
        #include <type_traits>

        #define _countof_ns_must_array(a)  (_countof_ns_0_if_assert< \
                    !std::is_same<decltype(&(a)[0]), decltype(a)>::value>())

        template <
            typename C,
            typename std::enable_if<std::is_class<C>::value, bool>::type = true>
        constexpr static auto _countof_ns_csize(const C& c) { return c.size(); }
        constexpr static size_t _countof_ns_csize(...) { return 0; }

        #define _countof_ns(a)  (std::is_class<decltype(a)>::value \
                    ? _countof_ns_csize(a) \
                    : _countof_ns_unsafe(a) + _countof_ns_must_array(a))
    #endif
    #define _countof_ns_typ2arr(a)  a  // __SUNPRO_CC magic, don't parentheses
    #define countof_ns(a)  (_countof_ns(_countof_ns_typ2arr(a)))
#endif

#endif // COUNTOF_NS_H_6951
