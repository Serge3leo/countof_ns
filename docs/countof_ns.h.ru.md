[//]: # (vim:set sw=4 ts=8 fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя])
[//]: # (SPDX-License-Identifier: BSD-2-Clause)
[//]: # (SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru))
[//]: # (История:)
[//]: # (2025-12-18 13:42:32 - Создан.)

# Русский вариант документации [`countof_ns.h`](../include/countof_ns.h)


# Реализация макроса `countof()` черновика C2y для C23/C++11

Для стандартных массивов C/C++11 результат идентичен `countof()`.  В
противном случае, если аргумент не является массивом, возникает ошибка
компиляции, т.е. полностью аналогично `countof()`.

Если аргумент является расширенным zero-length массивом, или содержит такие
массивы, или содержит элементы нулевого размера, то:

- Для массивов C++11 выдаёт идентичный `countof()` результат;

- Для фиксированных массивов С, в случае `sizeof(*array) == 0`, если
  число элементов 0, то будет возвращён 0, в противном случае возникает
  ошибка компиляции;

- Для VLA массивов C, в случае `sizeof(*array) == 0`, всегда возвращает 0
  (в общепринятых расширениях языка C для массивов содержащих элементы
  нулевого размера, полные типы VLA `T0 [0]` и `T0 [u]` оказываются
  совместимыми, поэтому неопределённость ноль-на-ноль всегда разрешатся
  в пользу 0).


## Требования

- C23 или C++11;
- С99 c расширениями `__typeof__()` и другими (clang, gcc, Intel, MSVC,
  Sun C, PGI Compilers, ...);

## Использование

До первого включения можно определить следующие макросы в значение 1:
- `_COUNTOF_NS_WANT_VLA_C11` - обеспечить поддержку VLA (если не определён
  `__STDC_NO_VLA__`). Без этих определений аргумент VLA будет вызывать
  ошибку компиляции;

    ПРЕДУПРЕЖДЕНИЕ: Поддержка VLA требует соответствия компилятора `6.5.6
    Additive operators` C11 и выше для всех типов аргументов.  Не смотря на
    то, что в C11 ограничения на вычитание указателей ужесточились,
    некоторые компиляторы (старые Intel, PGI и Sun) продолжают их
    реализовывать такими же образом, как и операции сравнения указателей.

- `_COUNTOF_NS_WANT_VLA_BUILTIN` - использовать сравнительно широко
  распространённую встроенную функцию `__builtin_types_compatible_p()`, в
  случае если нет определений `__STDC_NO_VLA__` или
  `_COUNTOF_NS_WANT_STDC`;

    Обычно, компиляторы, которые поддерживают
    `__builtin_types_compatible_p()`, так же соответствуют C11, в части
    ограничений на вычитание указателей, но могут быть и иные соображения.
    К примеру, его возможно использовать для гарантированных настроек старых
    добрых компиляторов:

```
    $ icc -diag-error=1121 \
          -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))' \
          -D_COUNTOF_NS_WANT_VLA_BUILTIN ...

    $ pgcc --diag_error=nonstandard_ptr_minus_ptr \
           -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))' \
           -D_COUNTOF_NS_WANT_VLA_BUILTIN ...

    $ suncc -errwarn=E_BAD_POINTER_SUBTRACTION \
            -D'_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))' \
            -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
```
   Маловероятно, что кому-нибудь потребуется `_COUNTOF_NS_WANT_VLA_BUILTIN`
   для MSVC, но это тоже возможно:

```
    > cl /std:clatest /wd4116 /we4047 ^
         /FI_countof_ns_ptr_compatible_type_msvc.h ^
        /D_COUNTOF_NS_WANT_VLA_BUILTIN ...
```

- `_COUNTOF_NS_BROKEN_TYPEOF_WORKAROUND` - у некоторых компиляторов (старый
  Intel(icc) и NVHPC(pgcc)) функция `__typeof__()` работает как C23
  `typeof_unqual()`, что приводит к отказам компиляции в случае
  квалификаторов.  Негативным следствием определения этого флага являются
  ложные предупреждения MSVC(cl) и др., а так же ошибки компиляции
  SunPro(suncc).

- `_COUNTOF_NS_WANT_STDC` - не использовать расширения: `__typeof__()` и
  другие;


### Сравнение опций поддержки VLA

- Если приоритет - минимизация ошибок компиляции на различных
  компиляторах, то при наличии VLA следует предпочесть
  `_COUNTOF_NS_WANT_VLA_C11`;

- Если приоритет - корректность контроля, то предпочтительнее
  `_COUNTOF_NS_WANT_VLA_BUILTIN` (к сожалению, проблемные компиляторы не
  поддерживают `__has_builtin()`).

Данный заголовочный файл определяет `_COUNTOF_NS_VLA_UNSUPPORTED` в случае,
если не обеспечивается поддержка VLA.

# Особенности реализации

```
// Avoid uncertain zero-by-zero divide and warnings
```
```
                // Simplest C23 way
                // But partial failure for old Intel (icc) and NVHPC(pgcc)
```
```
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
```
```
            // Constraints `a` is array and have `_countof_ns_unsafe(a)`
            // elements (for VLA, number elements is unconstrained).
            //
            // Constraints identically C11 constraints of pointer
            // subtraction.  See below.
```
```
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
```
```
            // Constraints `a` is fixed array and have `_countof_ns_unsafe(a)`
            // elements (don't have variably modified type, i.e. not VLA, not
            // contains VLA etc).
```
        // Count of fixed size arrays, case: standard C++ array
```
```
        // Count of fixed size arrays, case: extensions ZLA C++ array
```
```
            // TODO XXX Intel 2021 - HAVE_HIDDEN_IS_SAME_CXX, but SunPro - not
            // C++ with VLA extension
```
```
#if 0
            // Clang/GNU/NHPC (pgcc Nvidia HPC) - work fine

            // Internal VLA stub
        template<size_t A, size_t E>
        constexpr static size_t _countof_ns_aux(...) noexcept {
            return 0;
        }

            // For VLA clip not constexpr argument to constexpr 0
        #define _countof_ns_2cexp(a, v)  (__builtin_constant_p(sizeof(a)) \
                                         ? (v) : 0)

            // If sizeof(a) - compile-time constant, then count fixed size
            // array , otherwise count VLA.
        #define countof_ns(a)  (__builtin_constant_p(sizeof(a)) \
                  ? _countof_ns_aux<_countof_ns_2cexp((a), sizeof(a)), \
                                    _countof_ns_2cexp((a), sizeof(*(a)))>(a) \
                  : _vla_countof_ns(a))
```
```
            // But IntelLLVM sometimes considers:
            // __builtin_constant_p(sizeof(VLA)) == 1

            // Internal VLA stub
```
```
            // For VLA clip not constexpr argument to constexpr 0
```
```
        #warning "There is no correct implementation in pure C++ (wait C++26?)"
```
