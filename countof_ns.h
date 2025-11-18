// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2024-07-30 23:09:46 - Создан.
// 2025-11-13 16:30:46 - Убран конфликты с C2y, MS, VLA
// 2025-11-16 15:58:13 - Пример
// 2025-11-18 04:07:37 - Поддержка C99/C11 вычитания указателей
//
// Реализация макроса `countof()` черновика C2y для C23/C++11
//
// Для стандартных массивов C/C++11 результат идентичен `countof()`. В
// противном случае, если аргумент не является массивом, возникает ошибка
// компиляции, аналогично `countof()`.
//
// Если аргумент является расширенным zero-length массивом, содержит такие
// массивы или содержит структуры нулевой длины (clang/gcc), то:
//
//     - C++11 версия макроса, выдаёт идентичный результат;
//
//     - C версия, если число элементов такого массива равно 0, выдаёт 0, в
//     противном случае возникает ошибка компиляции.
//
//
// Требования:
//
//     - C23 или C++11;
//
//     - С11 c расширением `__typeof__()` - clang 4, gcc 5, MSVC 17.9, но это
//     неточно (TODO);
//
//     - C99 c расширением `__typeof__()` - clang 4, gcc 5, MSVC 17.9, но это
//     неточно (TODO);
//
//
// До первого включения можно определить следующие макросы значением 1:
//
// `_COUNTOF_NS_WANT_STDC` - не использовать расширение `__typeof__()`;
//
// `_COUNTOF_NS_WANT_C11_PTR_SUB` - для проверки полных типов и вызова ошибки
// компиляции использовать вычитание указателей, согласно `6.5.6 Additive
// operators` (C11/23).
//
// `_COUNTOF_NS_WANT_C99_PTR_SUB` - для вызова ошибки компиляции использовать
// тип массива отрицательной длины, согласно `6.7.5.2 Array declarators` (C99,
// 6.7.6.2 С11/С23), поскольку до C99 включительно операция вычитания
// указателей требовала указатели на совместимые типы (так же, как операция
// равенства). И, в этом случае, проверка полных типов выполняется функцией
// `_Generic()`, которая не поддерживает VLA.
//
// Для MSVC, по умолчанию, используется флаг `_COUNTOF_NS_WANT_C99_PTR_SUB`.
// Поскольку, при вычитании указателей, он выдаёт только Compiler Warning
// (level 1) C4047 'operator' : 'identifier1' differs in levels of indirection
// from 'identifier2'. Однако, ввиду того, что MSVC не имеет поддержки VLA, это
// не накладывает никаких реальных ограничений.
//
// Данный заголовочный файл определяет `_COUNTOF_NS_VLS_UNSUPPORTED` в случае,
// если не обеспечивается поддержка VLA.
//

#ifndef COUNTOF_NS_H_6951
#define COUNTOF_NS_H_6951

#include <stddef.h>

#if !__cplusplus
    #if __STDC_VERSION__ >= 202311L
        #define _countof_ns_typeof(t)  typeof(t)
    #elif (!_COUNTOF_NS_WANT_STDC && \
           (__clang_major__ >= 4 || __GNUC__ >= 5 || _MSC_VER >= 1939))  // TODO
        #define _countof_ns_typeof(t)  __typeof__(t)
    #else
        #error "typeof(t) don't detected, need C23 or clang/gcc/MSVC/... extension"
    #endif
    #define _countof_ns_unsafe(a)  (sizeof(*(a)) ? sizeof(a)/sizeof(*(a)) : 0)
    #if _COUNTOF_NS_WANT_C99_PTR_SUB || (!_COUNTOF_NS_WANT_C11_PTR_SUB && \
        (_MSC_VER || __STDC_VERSION__ < 201112L))
        #define _countof_ns_must_be(a) ((size_t)!sizeof(char[_Generic(&(a), \
                    _countof_ns_typeof(*(a)) (*)[_countof_ns_unsafe(a)]: 1, \
                    default: -1)]))
        #define _COUNTOF_NS_VLS_UNSUPPORTED  (1)
    #else
        #define _countof_ns_must_be(a)  ((size_t)( \
                    (_countof_ns_typeof(a) **)&(a) - \
                    (_countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)])&(a)))
    #endif
    #define countof_ns(a)  (_countof_ns_unsafe(a) + _countof_ns_must_be(a))
#else
    template<size_t A, size_t E, class T, size_t N>
    constexpr static size_t _countof_ns_aux(T (&)[N]) { return N; }
    template<size_t A, size_t E, class T>
    constexpr static size_t _countof_ns_aux(T (&)) {
        static_assert(0 == A, "Argument zero-length array");
        return 0;
    }
    #define countof_ns(a) _countof_ns_aux<sizeof(a), sizeof(*(a))>(a)
#endif

#endif // COUNTOF_NS_H_6951
