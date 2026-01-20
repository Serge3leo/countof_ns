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
// $ suncc -errwarn=E_BAD_POINTER_SUBTRACTION
//   #-D'_countof_ns_compatible_type(p,t1,t2)=(0==0*sizeof((t1)(p)-(t2)(p)))'
//    -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))'
//    -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
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
#if __SUNPRO_C
        // - SunPro ZLA extension limited: can't sizeof((ZLA)[0]);
        // - SunPro incomplete implementation Conditionals with Omitted (?:);
        // - C++ containers implement only `[]` operator;
    #define _countof_ns_unsafe(a)  \
                (0 == sizeof(*(a)) ? 0 : sizeof(a)/sizeof(*(a)))
#elif !__GNUC__ || __NVCOMPILER
        // NVHPC (pgcc) incomplete implementation (?:);
    #define _countof_ns_unsafe(a)  \
                (0 == sizeof((a)[0]) ? 0 : sizeof(a)/sizeof((a)[0]))
#else
        // Assume: __GNUC__ => has Conditionals with Omitted (?:)
        //         sizeof((a)[0]) == 0 => sizeof(a) == 0
    #define _countof_ns_unsafe(a)  (sizeof(a)/(sizeof((a)[0]) ?: 1))
    #define _cntfn_not_kr_idiom_  (1)
#endif

#if !__cplusplus
    #if _COUNTOF_NS_WANT_KR || (_MSC_VER && _MSC_VER < 1939)
        #define _COUNTOF_NS_USE_KR (1)
    #elif _COUNTOF_NS_WANT_VLA_C11 || _COUNTOF_NS_WANT_STDC
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
    #elif !_COUNTOF_NS_USE_KR
        #error "With _COUNTOF_NS_WANT_STDC required C23 typeof(t)"
    #endif
    #if _COUNTOF_NS_USE_KR
#if 1
        #if _cntfn_not_kr_idiom_
            #define _countof_ns_must_array(a)  \
                                (0*sizeof(sizeof(a)/sizeof((a)[0])))
        #else
            #define _countof_ns_must_array(a)  (0)
        #endif
#elif 0
        #if !__LCC__ && !_MSC_VER
                // __LCC__ - SIGFPE by div zero
                // _MSC_VER - don't have __builtin_constant_p()
            #define _countof_ns_must_array(a)  ((void)( \
                            __builtin_constant_p(sizeof(*(a))) \
                                ? ((void)(1/sizeof(*(a))), 0) \
                                : 0), 0)
        #else
            #define _countof_ns_must_array(a)  (0)
        #endif
#elif 1
        // clang 15 or above
        // gcc 10 or above
        // icx, pgcc -???
        //__attribute__ ((warning ("Zero size array element")))
        // [[deprecated]]
        // [[gnu::warning("Zero size array element")]]
        __attribute__ ((warning ("Zero size array element")))
        static size_t _countof_ns_element_zero();
        static size_t _countof_ns_element_zero() {
            return 0;
        }
        #define _countof_ns_must_array(a)  ( \
                __builtin_constant_p(sizeof(*(a))) && 0 == sizeof(*(a)) \
                    ? _countof_ns_element_zero() : 0)
#else
        #define _countof_ns_must_array(a)  (__builtin_choose_expr( \
             (__builtin_constant_p(sizeof(*(a))) ? 0 == sizeof(*(a)) : 0), \
                        ((void)printf("%d: %s\n", __LINE__, #a), 0), \
                        0))
                        //
                        // ((void)_countof_ns_zerro_divide(a, #a), 0), \
                        //
                        //
#endif
    #elif _COUNTOF_NS_USE_BUILTIN || _COUNTOF_NS_USE_SUBTRACTION
        #if !_COUNTOF_NS_USE_BUILTIN  // TODO XXX
            #define _countof_ns_ptr_compatible_type(ppa, type) \
                            (0 == 0*sizeof((ppa) - (type)(ppa)))
            // TODO: _countof_ns_must_compatible(p, t1, t2)
            // C11:  (0*sizeof((t1)(p) - (t2)(p)))
            // Norm: _countof_ns_must(__builtin_types_compatible_p(t1, t2))
            // Arnd: _countof_ns_must!__builtin_types_compatible_p(
            //                                  typeof(p), typeof(&*(p)))
        #elif !defined(_countof_ns_ptr_compatible_type)
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
    #if !_COUNTOF_NS_USE_KR && !__LCC__ && !__SUNPRO_C && !__INTEL_COMPILER
        #define _countof_ns_typ2arr(a)  (*(_countof_ns_typeof(a) *)(void *)8192)
    #else
        #define _countof_ns_typ2arr(a)  a
    #endif
    #define _countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_array(a))
    #define countof_ns(a)  (_countof_ns(_countof_ns_typ2arr(a)))
#else
    #include <type_traits>
    namespace _countof_ns_ {

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

        // Unchecked size stub, only for compilation success
    constexpr size_t unthinkable = 1917;
    constexpr char no = false;
    constexpr long long yes = true;
    static_assert(sizeof(no) != sizeof(yes),
                  "TODO XXX: sizeof(no) != sizeof(yes)");
        // T is container (have `size()` member)
    template <class T> struct has_size {
        template <class C> static decltype(yes) test_(decltype(&C::size));
        template <class> static decltype(no) test_(...);
        static constexpr bool value = sizeof(test_<T>(0)) == sizeof(yes);
    };
        // T is ZLA
#if 1 // TODO XXX
    template<class T>
    struct is_zla : std::integral_constant<bool, 0 == sizeof(T) &&
                                0 == std::extent<T>::value &&
                                !std::is_class<T>::value &&
                                !std::is_function<T>::value &&
                                !std::is_scalar<T>::value &&
                                !std::is_union<T>::value &&
                                !std::is_void<T>::value> {};
#else
    template<class T>
    class is_zla {
        constexpr static size_t bias_ = 3;
        template<class C> static auto size_(C &c) ->
                                        char(*)[sizeof(*c) + bias_];
        static char *size_(...);
        struct indr_t_ {};
        struct subs_t_ {};
        static_assert(!std::is_same<indr_t_, subs_t_>::value,
                      "TODO XXX: !std::is_same<indr_t_, subs_t_>::value");
        template<class C> static auto indr_(C &c) -> decltype(**c);
        static indr_t_ indr_(...);
        template<class C> static auto subs_(C &c) -> decltype((*c)[0]);
        static subs_t_ subs_(...);
        constexpr static typename std::remove_reference<T>::type *pt_ = 0;
     public:
        static constexpr bool value = (0 == std::extent<T>::value &&
            !std::is_class<T>::value &&
            !std::is_function<T>::value &&
            !std::is_scalar<T>::value &&
            !std::is_union<T>::value &&
            !std::is_void<T>::value &&
            std::is_same<decltype(indr_(pt_)), decltype(subs_(pt_))>::value &&
            sizeof(*size_(pt_)) == 0 + bias_);
    };
#endif
        // Count of fixed array, standard C++
    template<class T, size_t N> static char (*match(const T (&)[N]))[N];
        // Count of fixed ZLA
    template <class T, typename std::enable_if<
                                is_zla<T>::value, int>::type = 0>
    static char (*match(const T&))[0];
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
        #define _countof_ns(a)  (_countof_ns_::has_size<decltype(a)>::value \
                                 ? _countof_ns_::cnt_size(a) \
                                 : sizeof(*_countof_ns_::match(a)))
    #elif _COUNTOF_NS_USE_STUB
        #warning "There is no correct implementation in pure C++ (wait C++26?)"
        #define _countof_ns(a)  (_countof_ns_unsafe(a))
    #elif _COUNTOF_NS_USE_BUILTIN // && !__SUNPRO_CC
            // C++ with VLA extension
            // Argument may be VLA or not
        #if !__SUNPRO_CC  // Number compilers HAVE_HIDDEN_IS_SAME_CXX
            #define _cntfn_must_vla(a)  (_countof_ns_::zero_assert< \
                            !__is_same(decltype(&(a)[0]), decltype(a))>())
            template<class T>
            constexpr static auto match_not_vla(const T&) { return yes; }
        #else
            #define _cntfn_must_vla(a)  (_countof_ns_::zero_assert< \
                      !std::is_same<decltype(&(a)[0]), decltype(a)>::value>())
            template <class T, typename std::enable_if<
                                    !std::is_array<T>::value ||
                                    0 < std::extent<T>::value, int>::type = 0>
            constexpr static auto match_not_vla(const T&) { return yes; }
        #endif
        constexpr static auto match_not_vla(...) { return no; }
            // Count of VLA
        template<bool IsArray>
        constexpr static size_t zero_assert() noexcept {
            static_assert(IsArray, "Must be array");
            return 0;
        }
        #define _cntfn_vla(a)  (_countof_ns_unsafe(a) + _cntfn_must_vla(a))
            // Argument is container
        template <class C, typename std::enable_if<
                                    has_size<C>::value, int>::type = 0>
        constexpr static auto match_cnt(const C&) { return yes; }
        constexpr static auto match_cnt(...) { return no; }
            // Count of fixed array (or ZLA)
        template <class T>
        constexpr static auto stub_match(const T& a) -> decltype(match(a));
        constexpr static char (*stub_match(...))[unthinkable];
        #define _countof_ns(a)  (sizeof(_countof_ns_::match_not_vla(a)) == \
                                 sizeof(_countof_ns_::no) \
                                 ? _cntfn_vla(a) \
                                 : sizeof(_countof_ns_::match_cnt(a)) == \
                                   sizeof(_countof_ns_::yes) \
                                        ? _countof_ns_::cnt_size(a) \
                                        : sizeof(*_countof_ns_::stub_match(a)))
    #endif
    #define _countof_ns_typ2arr(a)  a  // magic, don't parentheses
    #define countof_ns(a)  (_countof_ns(_countof_ns_typ2arr(a)))

    }  // of namespace
#endif

#endif // COUNTOF_NS_H_6951
