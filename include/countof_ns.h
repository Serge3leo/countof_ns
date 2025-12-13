// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2024-07-30 23:09:46 - Создан.
// 2025-11-13 16:30:46 - Убран конфликты с C2y, MS, VLA
// 2025-11-16 15:58:13 - Пример
// 2025-11-18 04:07:37 - Поддержка C99/C11 вычитания указателей
// 2025-12-04 20:05:35 - Новый стабильный интерфейс
//
//
// Реализация макроса `countof()` черновика C2y для C23/C++11
//
// Для стандартных массивов C/C++11 результат идентичен `countof()`.  В
// противном случае, если аргумент не является массивом, возникает ошибка
// компиляции, т.е. полностью аналогично `countof()`.
//
// Если аргумент является расширенным zero-length массивом, или содержит такие
// массивы, или содержит элементы нулевого размера, то:
//
//     - Для массивов C++11 выдаёт идентичный `countof()` результат;
//
//     - Для фиксированных массивов С, в случае `sizeof(*array) == 0`, если
//       число элементов 0, то будет возвращён 0, в противном случае возникает
//       ошибка компиляции;
//
//     - Для VLA массивов C, в случае `sizeof(*array) == 0`, всегда возвращает 0
//       (в общепринятых расширениях языка C для массивов содержащих элементы
//       нулевого размера, полные типы VLA `T0 [0]` и `T0 [u]` оказываются
//       совместимыми, поэтому неопределённость ноль-на-ноль всегда разрешатся
//       в пользу 0).
//
//
// Требования:
//
//     - C23 или C++11;
//
//     - С99 c расширениями `__typeof__()` и другими (clang, gcc, Intel, MSVC,
//       Sun C, PGI Compilers, ...);
//
//
// До первого включения можно определить следующие макросы в значение 1:
//
// `_COUNTOF_NS_WANT_VLA_C11` - обеспечить поддержку VLA (если не определён
//     `__STDC_NO_VLA__`). Без этих определений аргумент VLA будет вызывать
//     ошибку компиляции;
//
//     ПРЕДУПРЕЖДЕНИЕ: Поддержка VLA требует соответствия компилятора `6.5.6
//     Additive operators` C11 и выше для всех типов аргументов.  Не смотря на
//     то, что в C11 ограничения на вычитание указателей ужесточились,
//     некоторые компиляторы (старые Intel, PGI и Sun) продолжают их
//     реализовывать такими же образом, как и операции сравнения указателей.
//
// `_COUNTOF_NS_WANT_VLA_BUILTIN` - использовать сравнительно широко
//     распространённую встроенную функцию `__builtin_types_compatible_p()`, в
//     случае если нет определений `__STDC_NO_VLA__` или
//     `_COUNTOF_NS_WANT_STDC`;
//
//     Обычно, компиляторы, которые поддерживают
//     `__builtin_types_compatible_p()`, так же соответствуют C11, в части
//     ограничений на вычитание указателей, но могут быть и иные соображения.
//     К примеру, его возможно использовать для гарантированных настроек старых
//     добрых компиляторов:
//
//     $ icc -diag-error=1121 \
//         -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))' \
//         -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//     $ pgcc --diag_error=nonstandard_ptr_minus_ptr \
//         -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))' \
//         -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//     $ suncc -errwarn=E_BAD_POINTER_SUBTRACTION \
//         -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))' \
//         -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
//     Маловероятно, что кому-нибудь потребуется `_COUNTOF_NS_WANT_VLA_BUILTIN`
//     для MSVC, но это тоже возможно:
//
//     > cl /std:clatest /wd4116 /we4047 ^
//          /FI_countof_ns_ptr_compatible_type_msvc.h ^
//          /D_COUNTOF_NS_WANT_VLA_BUILTIN ...
//
// `_COUNTOF_NS_BROKEN_TYPEOF_WORKAROUND` - у некоторых компиляторов (старый
//     Intel(icc) и NVHPC(pgcc)) функция `__typeof__()` работает как C23
//     `typeof_unqual()`, что приводит к отказам компиляции в случае
//     квалификаторов.  Негативным следствием определения этого флага являются
//     ложные предупреждения MSVC(cl) и др., а так же ошибки компиляции
//     SunPro(suncc).
//
// `_COUNTOF_NS_WANT_STDC` - не использовать расширения: `__typeof__()` и
//     другие;
//
//
// Сравнение опций поддержки VLA:
//     - Если приоритет - минимизация ошибок компиляции на различных
//       компиляторах, то при наличии VLA следует предпочесть
//       `_COUNTOF_NS_WANT_VLA_C11`;
//
//     - Если приоритет - корректность контроля, то предпочтительнее
//       `_COUNTOF_NS_WANT_VLA_BUILTIN` (к сожалению, проблемные компиляторы не
//       поддерживают `__has_builtin()`).
//
//
// Данный заголовочный файл определяет `_COUNTOF_NS_VLA_UNSUPPORTED` в случае,
// если не обеспечивается поддержка VLA.

#ifndef COUNTOF_NS_H_6951
#define COUNTOF_NS_H_6951
#define _COUNTOF_NS  (202512L)

#include <stddef.h>

    // Avoid uncertain zero-by-zero divide and warnings
#define _countof_ns_unsafe(a)  (0 == sizeof(*(a)) ? 0 \
            : sizeof(a)/( sizeof(*(a)) ? sizeof(*(a)) : (size_t)-1 ))

#if !__cplusplus
#if 1
    #if _COUNTOF_NS_REFUSE_VLA || __STDC_NO_VLA__
        #define _COUNTOF_NS_USE_GENERIC  (1)
    #elif _COUNTOF_NS_WANT_VLA_C11 || _COUNTOF_NS_WANT_STDC
        #define _COUNTOF_NS_USE_SUBTRACTION  (1)
    #elif _COUNTOF_NS_WANT_VLA_BUILTIN || \
          defined(_countof_ns_ptr_compatible_type)
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #elif __SUNPRO_C
        #warning "For SunPro, you must define either _COUNTOF_NS_REFUSE_VLA or _countof_ns_ptr_compatible_type(p, type)"
        #define _COUNTOF_NS_USE_GENERIC  (1)
    #else
        // TODO: __POCC__  // Pelles Cq
        #define _COUNTOF_NS_USE_BUILTIN  (1)
    #endif
    #if __STDC_VERSION__ >= 202311L
        #define _countof_ns_assert  static_assert
        #define _countof_ns_typeof(t)  typeof(t)
    #elif !_COUNTOF_NS_WANT_STDC
        #define _countof_ns_assert  _Static_assert
        #if _COUNTOF_NS_BROKEN_TYPEOF_WORKAROUND || _COUNTOF_NS_USE_BUILTIN
            #define _countof_ns_typeof(t)  const volatile __typeof__(t)
        #else
            #define _countof_ns_typeof(t)  __typeof__(t)
        #endif
    #else
        #error "With _COUNTOF_NS_WANT_STDC required C23 typeof(t)"
    #endif
    #if _COUNTOF_NS_USE_BUILTIN
        #if !defined(_countof_ns_ptr_compatible_type) && \
            defined(__has_builtin)
            #if __has_builtin(__builtin_types_compatible_p)
#if 1
                #define _countof_ns_ptr_compatible_type(p, type)  \
                            __builtin_types_compatible_p( \
                                _countof_ns_typeof(p), type)
#else
            #define _countof_ns_ptr_compatible_type(ppa, type)  \
                            (!__builtin_types_compatible_p( \
                                _countof_ns_typeof(&*(**(ppa))), \
                                _countof_ns_typeof(**(ppa))))
#endif
            #endif
        #endif
        #if !defined(_countof_ns_ptr_compatible_type) && \
            defined(__GNUC__) && __GNUC__ < 10
            // Old gcc and compatibles have restrict
            // __builtin_types_compatible_p()
            #define _countof_ns_ptr_compatible_type(ppa, type)  \
                            (!__builtin_types_compatible_p(
                                _countof_ns_typeof(&*(**(ppa))),
                                _countof_ns_typeof(**(ppa))))
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
#else
    #if __STDC_VERSION__ >= 202311L
        #define _countof_ns_assert  static_assert
        #define _countof_ns_typeof(t)  typeof(t)
    #elif !_COUNTOF_NS_WANT_STDC
        #define _countof_ns_assert  _Static_assert
        #if !_COUNTOF_NS_BROKEN_TYPEOF_WORKAROUND
                // Simplest C23 way
                // But partial failure for old Intel (icc) and NVHPC(pgcc)
            #define _countof_ns_typeof(t)  __typeof__(t)
        #else
                // In case the pre C23 language extension is like this:
                // `__typeof__(x) === typeof_unqual(x)` (old Intel(icc) and
                // NVHPC(pgcc)), we dummy add "all" qualifiers.
                //
                // But full failure for SunPro (suncc)
                // And MSVC, warning C4114: same type qualifier used more than
                // once
                //
                // In this case, it would be better to use __TYPEOF_UNQUAL__,
                // but there is no function with this name for Intel(icc),
                // NVHPC(pgcc) and SunPro(suncc)
            #define _countof_ns_typeof(t)  const volatile __typeof__(t)
        #endif
    #else
        #error "With _COUNTOF_NS_WANT_STDC required C23 typeof(t)"
    #endif

    #if _COUNTOF_NS_WANT_VLA_BUILTIN && !__STDC_NO_VLA__ && \
        !_COUNTOF_NS_WANT_STDC
        #if !defined(_countof_ns_ptr_compatible_type) && defined(__has_builtin)
            #if __has_builtin(__builtin_types_compatible_p)
                #define _countof_ns_ptr_compatible_type(p, type) \
                            __builtin_types_compatible_p( \
                                _countof_ns_typeof(p), type)
            #endif
        #endif
        #if !defined(_countof_ns_ptr_compatible_type)
            #error "Not __builtin_types_compatible_p() or" \
                   " _countof_ns_ptr_compatible_type() for" \
                   "_COUNTOF_NS_WANT_VLA_BUILTIN"
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
    #elif _COUNTOF_NS_WANT_VLA_C11 && !__STDC_NO_VLA__
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
#endif
    #define countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_array(a))
#elif _COUNTOF_NS_WANT_VLA_BUILTIN
        // TODO XXX Intel 2021 - HAVE_HIDDEN_IS_SAME_CXX, but SunPro - not

        // C++ with VLA or ZLA extension
    template<bool cnd>
    constexpr size_t _countof_ns_0_if_assert() noexcept {
        static_assert(cnd, "Must be array");
        return 0;
    }
    #define _countof_ns_must_array(a)  (_countof_ns_0_if_assert< \
                                        !__is_same(decltype(&*(a)), decltype(a))>())
    #define countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_array(a))
#else
        // C++ with ZLA extension
    #define _COUNTOF_NS_VLA_UNSUPPORTED  (1)
    template<size_t A, size_t E, class T, size_t N>
    constexpr static size_t _countof_ns_aux(const T (&)[N]) noexcept {
        return N;
    }
    template<size_t A, size_t E, class T>
    constexpr static size_t _countof_ns_aux(const T (&)) noexcept {
        static_assert(0 == A, "Argument must be zero-length array");
        return 0;
    }
    #define countof_ns(a)  (_countof_ns_aux<sizeof(a), sizeof(*(a))>(a))
#endif

#endif // COUNTOF_NS_H_6951
