// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2024-07-30 23:09:46 - Создан.
// 2025-11-13 16:30:46 - Убран конфликты с C2y, MS, VLA
// 2025-11-16 15:58:13 - Пример
// 2025-11-18 04:07:37 - Поддержка C99/C11 вычитания указателей
//
//
// Реализация макроса `countof()` черновика C2y для C23/C++11
//
// Для стандартных массивов C/C++11 результат идентичен `countof()`. В
// противном случае, если аргумент не является массивом, возникает ошибка
// компиляции, т.е. полностью аналогично `countof()`.
//
// Если аргумент является расширенным zero-length массивом, или содержит такие
// массивы, или содержит элементы нулевого размера, то:
//
//     - Для массивов C++11 выдаёт идентичный `countof()` результат;
//
//     - Для фиксированных массивов С, в случае `sizeof(array) == 0`, если
//       число элементов 0, то будет возвращён 0, в противном случае возникает
//       ошибка компиляции;
//
//     - Для VLA массивов C, в случае `sizeof(array) == 0`, всегда возвращает 0
//       (в общепринятых расширениях языка C полные типы VLA `T [0]` и `T [n]`
//       оказываются совместимыми).
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
// До первого включения можно определить следующие макросы:
//
// TODO
// `_COUNTOF_NS_WANT_VLA_EXTC11` - обеспечить поддержку VLA (если не определён
//     `__STDC_NO_VLA__`), без этого флага аргумент VLA будет вызывать ошибку
//     компиляции;
//
//     ПРЕДУПРЕЖДЕНИЕ: Поддержка VLA требует соответствия компилятора `6.5.6
//     Additive operators` C11 и выше для всех типов аргументов. А так же
//     общепринятого расширения языка C в части 6.7.6.2 Array declarators (6)
//     [C11] для VLA.
//
// TODO
// `_COUNTOF_NS_WANT_VLA_BUILTIN` - использовать сравнительно широко
//     распространённую встроенную функцию `__builtin_types_compatible_p()`;
//
//         TODO: Дублировать ветку `_Generic()`, для того, что бы показать
//         сообщение о возможностях _COUNTOF_NS_WANT_VLA_EXTC11 или
//         _COUNTOF_NS_WANT_VLA_BUILTIN.
//
// `_COUNTOF_NS_WANT_STDC` - не использовать расширения: `__typeof__()` и
//     другие;
//
// Данный заголовочный файл определяет `_COUNTOF_NS_VLA_UNSUPPORTED` в случае,
// если не обеспечивается поддержка VLA.

#ifndef COUNTOF_NS_H_6951
#define COUNTOF_NS_H_6951

#include <stddef.h>

#if !__cplusplus
    #if __STDC_VERSION__ >= 202311L
        #define _countof_ns_typeof(t)  typeof(t)
    #elif !_COUNTOF_NS_WANT_STDC
        #define _countof_ns_typeof(t)  __typeof__(t)
    #else
        #error "With _COUNTOF_NS_WANT_STDC required C23 typeof(t)"
    #endif
    #define _countof_ns_unsafe(a)  (!sizeof(*(a)) ? 0 \
                : sizeof(a)/( sizeof(*(a)) ? sizeof(*(a)) : (size_t)-1 ))
    #if __STDC_NO_VLA__ || !_COUNTOF_NS_WANT_C11_VLA
        #define _COUNTOF_NS_VLA_UNSUPPORTED  (1)
        // Использование традиционного `sizeof(struct{int:(-1)})` невозможно
        // ввиду MSVC Level 1 warning C4116: unnamed type definition in
        // parentheses.
        //
        // Использование ошибки компиляции при размере массива -1 полностью
        // безопасно, `_Generic()` является константным выражением, т.к.
        // ограничен типами постоянного размера и выбирает одну из двух
        // констант -1 или 1. Поэтому ошибка компиляции будет выдана вне
        // зависимости от поддержки VLA компилятором.

        #define _countof_ns_must_be(a) ((size_t)!sizeof(char[_Generic(&(a), \
                    _countof_ns_typeof(*(a)) (*)[_countof_ns_unsafe(a)]: 1, \
                    default: -1)]))
    #else
        #define _countof_ns_must_be(a)  (0*sizeof( \
                    (_countof_ns_typeof(a) **)&(a) - \
                    (_countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)])&(a)))
    #endif
    #define countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_be(a))
#else
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
    #define countof_ns(a) _countof_ns_aux<sizeof(a), sizeof(*(a))>(a)
#endif

#endif // COUNTOF_NS_H_6951
