<!-- vim:set sw=4 ts=8 fileencoding=utf8:
 SPDX-License-Identifier: BSD-2-Clause
 SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
 -->

# Русский вариант документации [`countof_ns.h`](../include/countof_ns.h)

# Реализация макроса `countof()` проекта C2y для C23/C++11

Макрос `countof_ns(array)` возвращает количество элементов в своем операнде.
Количество элементов определяется типом операнда. Результатом является целое
число. Если количество элементов массива является переменным, операнд
вычисляется, в противном случае операнд не вычисляется, и выражение является
целым константным выражением.

Для стандартных массивов C/C++11 результат идентичен `countof()`.  В противном
случае, если аргумент не является массивом, возникает ошибка компиляции, т.е.
поведение полностью аналогично `countof()`.

Если аргумент является расширенным zero-length массивом (ZLA), или содержит
такие массивы, или содержит элементы нулевого размера, то:

- Для фиксированных массивов C++11 выдаёт идентичный `countof()` результат;

- Для фиксированных массивов С, в случае `sizeof(*array) == 0`, если число
  элементов 0, то будет возвращён 0, в противном случае возникает ошибка
  компиляции (поведение зависит от возможностей компилятора, на некоторых
  компиляторах может всегда возвращаться 0);

- Для массивов переменной длины (VLA) стандарта С или расширения С++, в случае
  `sizeof(*array) == 0`, всегда возвращает 0.

## Требования

- C23 или C++11;
- С99 c расширениями `__typeof__()` и другими (Clang, GNU gcc, Intel (classic
  icc), IntelLLVM (icx), LCC (MCST Elbrus), MSVC (Visual Studio 2022), NVHPC
  (NVIDIA HPC Compiler), Pelles C, PGI (The Portland Group(?)), SunPro (Oracle
  Developer Studio)).

## Использование

До первого включения можно определить следующие макросы в значение 1:

- `_COUNTOF_NS_WANT_VLA_BUILTIN` - использовать встроенные функции
  (`__builtin_types_compatible_p()` для C или `__builtin_constant_p()` и
  `__is_same()` для C++ и др.). Используется по умолчанию, если нет определений
  `__STDC_NO_VLA__`, `_MSC_VER`, `__POCC__` или `_COUNTOF_NS_WANT_STDC`;

- `_COUNTOF_NS_WANT_VLA_C11` - обеспечить поддержку VLA стандартными
  возможностями C11/C23, требует соответствия компилятора `6.5.6 Additive
  operators` C11 и выше, для всех типов аргументов. Используется по умолчанию
  если определён символ `__POCC__` (Pelles C);

- `_COUNTOF_NS_REFUSE_VLA` - нет поддержки VLA (аргументы VLA вызывают ошибку
  компиляции). Используется по умолчанию, если определены символы
  `__STDC_NO_VLA__` или `_MSC_VER` (Visual Studio);

- `_COUNTOF_NS_BROKEN_TYPEOF` - C расширение `__typeof__()` удаляет
  квалификаторы, т.е. функционирует аналогично `typeof_unqual()` С23. Для
  компиляторов: классический Intel (icc), LCC (lcc) и NVHPC(pgcc), указывать не
  требуется, учитывается автоматически;

- `_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P` - функция
  `__builtin_types_compatible_p()` не проверяет полное совпадение VLA массивов:
  `static_assert(!__builtin_types_compatible_p(int (*)[d1], int (*)[d1]))`. Для
  компиляторов: классический Intel (icc), LCC (lcc) и NVHPC(pgcc), указывать не
  требуется, учитывается автоматически;

- `_COUNTOF_NS_WANT_STDC` - не использовать расширения: `__typeof__()` и
  другие;

### Сравнение опций поддержки VLA

- Если приоритет - минимизация ошибок компиляции на различных компиляторах, то
  при потребности в VLA, следует предпочесть `_COUNTOF_NS_WANT_VLA_C11`;

- Если приоритет - корректность контроля, то предпочтительнее
  `_COUNTOF_NS_WANT_VLA_BUILTIN`, т.к. отсутствует зависимость контроля
  ограничений от ключей компилятора (но, к сожалению, проблемные компиляторы не
  поддерживают `__has_builtin()`).

Так же макрос `_COUNTOF_NS_WANT_VLA_BUILTIN` позволяет документировать
настройки компилятора:

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

Маловероятно, что кому-нибудь потребуется `_COUNTOF_NS_WANT_VLA_BUILTIN` для
MSVC, но для тестирования, это тоже возможно:

```
    > cl /std:clatest /wd4116 /we4047 ^
         /FI_countof_ns_ptr_compatible_type_msvc.h ^
         /D_COUNTOF_NS_WANT_VLA_BUILTIN ...
```

### Статус поддержки VLA

Данный заголовочный файл определяет:

- `_COUNTOF_NS_VLA_UNSUPPORTED` в случае, если не обеспечивается поддержка VLA;

- `_COUNTOF_NS_USE_GENERIC`,  если используется `_Generic()`
  (`_COUNTOF_NS_VLA_UNSUPPORTED`);

- `_COUNTOF_NS_USE_TEMPLATE`,  если используются стандартные шаблоны C++
  (`_COUNTOF_NS_VLA_UNSUPPORTED`);

- `_COUNTOF_NS_USE_SUBTRACTION`,  если для поддержки VLA используется вычитание
  указателей по C11/C23;

- `_COUNTOF_NS_USE_BUILTIN`, если для поддержки VLA используется встроенные
  функции и т.п.

# Особенности реализации

```
// Обход неопределённости деления ноль-на-ноль и предупреждений
#define _countof_ns_unsafe(a)  (0 == sizeof(*(a)) ? 0 \
            : sizeof(a)/( sizeof(*(a)) ? sizeof(*(a)) : (size_t)-1 ))
```
При использовании простого выражения: `(0 == sizeof(*(a)) ? 0 :
sizeof(a)/sizeof(*(a)))`, некоторые компиляторы могут порождать ложные
предупреждения о возможности деления на 0.

```
            // Ограничения на `a` - массив содержащий `_countof_ns_unsafe(a)`
            // элементов (для VLA, число элементов не ограничивается).
            //
            // Ограничение идентично ограничению C11 на вычитание указателей.
            // (если !_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P).  См. ниже.
        #define _countof_ns_must_array(a)  \
                (0*sizeof(struct { int _countof_ns; _countof_ns_assert( \
                    _countof_ns_ptr_compatible_type( \
                        (_countof_ns_typeof(a) **)&(a), \
                        _countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)] \
                    ), "Must be array"); }))
```
```
            // Ограничения на `a` - массив, содержащий `_countof_ns_unsafe(a)`
            // элементов (для VLA, число элементов не ограничивается).
            //
            // Предупреждение: только в случае расширенных C массивов. которые
            // содержат или могут содержать элементы нулевого размера
            // (многомерные ZLA, и т.д.), происходит операция вычитания
            // указателя на входной массив и указателя на массив с нулевым
            // числом элементов. Согласно C11/C23 это неопределённое
            // поведение, если в массиве не нулевое число элементов. Однако,
            // в общепринятых расширениях C для VLA, эти указатели совместимы
            // и операция допустима (во время компиляции).
            //
            // В результате:
            //     static_assert(sizeof(T0) == 0);
            //     T0 a[0];
            //     static_assert(0 == countof_ns(a));  // Constraints OK - "is array"
            //     T0 b[1];
            //     (void)countof_ns(b);  // Constraints FAIL - "is not array"
            //     size_t u = ...;
            //     T0 c[u];
            //     assert(0 == countof_ns(c))  // Constraints OK - "is array",
            //                                 // at compile time, for any `u`
        #define _countof_ns_must_array(a)  (0*sizeof( \
                (_countof_ns_typeof(a) **)&(a) - \
                (_countof_ns_typeof(*(a))(**)[_countof_ns_unsafe(a)])&(a)))
```
```
            // Ограничения на `a` - массив фиксированого размера, содержащий
            // `_countof_ns_unsafe(a)` элементов (не является модифисированым
            // типом, т.е. не VLA, не содержит VLA и т.д.).
        #define _countof_ns_must_array(a)  (_Generic( \
                    (_countof_ns_typeof(a) *)&(a), \
                    _countof_ns_typeof(*(a))(*)[_countof_ns_unsafe(a)]: 0))
```
```
    #if _COUNTOF_NS_REFUSE_VLA || _MSC_VER
            // _MSC_VER единственный компилятор без поддержки C++ VLA
            // расширений.
        #define _COUNTOF_NS_USE_TEMPLATE  (1)
```
```
        // Подсчёт для фиксированного массива, случай: стандартного массива C++
    template<size_t A, size_t E, class T, size_t N>
    constexpr static size_t _countof_ns_aux(const T (&)[N]) noexcept {
        return N;
    }
```
```
        // Подсчёт для фиксированного массива, случай: расширения ZLA
    template<size_t A, size_t E, class T>
    constexpr static size_t _countof_ns_aux(const T (&)) noexcept {
        static_assert(0 == A, "Argument must be zero-length array");
        return 0;
    }
```
```
        #warning "Нет корректной реализации на чистом C++ (ждём C++26?)"
```
```
            // Внутреняя VLA заглушка, вызывается только в случае
            // HAVE_STRANGE_BUILTIN_CONSTANT_P_CXX (Intel, IntelLLVM)
        template<size_t A, size_t E>
        constexpr static size_t _countof_ns_aux(...) noexcept {
            return size_t(-1917); // Значение, которое крайне маловероятно
        }
```
```
            // Для VLA, ограничиваем неконстантный аргумент - константой 0
        #define _countof_ns_2cexp(a, v)  (__builtin_constant_p(sizeof(a)) \
                                         ? (v) : 0)
```
```
            // Для VLA, ограничиваем аргументы шаблона - константой 0
        #define _countof_ns_fix(a)  (_countof_ns_aux<\
                             _countof_ns_2cexp((a), sizeof(a)), \
                             _countof_ns_2cexp((a), sizeof(*(a)))>(a))
```
```
        #if 0   // !HAVE_STRANGE_BUILTIN_CONSTANT_P_CXX
                // Clang/GNU/LCC/NHPC (pgcc Nvidia HPC) - работвет замечательно
                //
                // Если sizeof(a) - константа времени компиляции, то
                // подсчитываем фиксированый массив, в противном случае
                // подсчитываем VLA.
            #define countof_ns(a)  (!__builtin_constant_p(sizeof(a)) \
                                    ? _vla_countof_ns(a) \
                                    : _countof_ns_fix(a))
        #else   // HAVE_STRANGE_BUILTIN_CONSTANT_P_CXX
                // Но компиляторы Intel and IntelLLVM иногда получают:
                // __builtin_constant_p(sizeof(VLA)) == 1
                //
                // Смотри: tests/autoconf/have_strange_builtin_constant_p_cxx.cpp
            #define countof_ns(a)  (!__builtin_constant_p(sizeof(a)) \
                                    ? _vla_countof_ns(a) \
                                    : size_t(-1917) == _countof_ns_fix(a) \
                                            ? _vla_countof_ns(a) \
                                            : _countof_ns_fix(a))
        #endif
```
```
    #else  // _COUNTOF_NS_USE_BUILTIN && __SUNPRO_CC
           // Расширенные шаблоны SunPro. SunPro запрещает массивы содержащие
           // объекты нулевой длины, в частности многомерные ZLA.
```
