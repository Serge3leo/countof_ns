<!-- vim:set sw=4 ts=8 fileencoding=utf8:
 SPDX-License-Identifier: BSD-2-Clause
 SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
 -->

# Русский вариант документации [`countof_ns.h`](../include/countof_ns.h)

# Реализация макроса `countof()` проекта C2y для C23/C++14

Макрос `countof_ns(array)` возвращает количество элементов в своем операнде (массив или тип массива). Количество элементов определяется типом операнда. Результатом является целое число. Для массивов переменной длины (VLA) операнд вычисляется, в противном случае операнд не вычисляется, и результат является целым константным выражением.

Для стандартных массивов C/C++14 результат идентичен `countof()`.  В противном случае, если аргумент не является массивом, возникает ошибка компиляции, т.е. поведение полностью аналогично `countof()`.

Если аргумент является расширенным zero-length массивом (ZLA), или содержит
такие массивы, или содержит элементы нулевого размера, то:

- Для фиксированных массивов C++14 выдаёт идентичный `countof()` результат;

- Для фиксированных массивов С, в случае `sizeof(*array) == 0`, если число
  элементов 0, то будет возвращён 0, в противном случае возникает ошибка
  компиляции (поведение зависит от возможностей компилятора, на некоторых
  компиляторах может всегда возвращаться 0);

- Для VLA, в случае `sizeof(*array) == 0`, всегда возвращает 0.

Предупреждение: в случае многомерных VLA число вычислений аргумента зависит от компилятора и отличается от `countof()`. Для проверки можно использовать сторонние анализаторы, к примеру, совместно с компилятором Clang можно использовать:
```sh
$ clang-tidy -config="{
    Checks: bugprone-assert-side-effect,
    CheckOptions: {
        bugprone-assert-side-effect.AssertMacros: 'assert,NSAssert,NSCAssert,sizeof,countof,countof_ns'
    }
}" ...
```
# Требования

- С23;
- C++14, для расширений VLA требуется встроенная функция `__is_same()`;
- С11, требуется `__typeof__()` и, возможно, `__builtin_types_compatible_p()`;
- Clang (clang), GNU (gcc), классический Intel (icc), IntelLLVM (icx), LCC (MCST Elbrus), MSVC (Visual Studio), NVHPC (NVIDIA HPC Compiler), Pelles C, PGI (The Portland Group(?)), SunPro (Oracle Developer Studio), XL (IBM® XL C/C++ for AIX®), XLClang (IBM Clang-based XL).

# Использование
```c
#include "countof_ns/countof_ns.h"
```

До первого включения, при необходимости, можно определить следующие макросы в значение 1:

- `_COUNTOF_NS_BROKEN_BUILTIN_TYPES_COMPATIBLE_P` - функция
  `__builtin_types_compatible_p()` не проверяет полное совпадение VLA массивов:
  `static_assert(!__builtin_types_compatible_p(int (*)[d1], int (*)[d1]))`. Для компиляторов: классический Intel (icc), LCC (lcc) и NVHPC(pgcc), указывать не требуется, учитывается автоматически;

- `_COUNTOF_NS_BROKEN_TYPEOF` - C расширение `__typeof__()` удаляет
  квалификаторы, т.е. функционирует аналогично `typeof_unqual()` С23. Для
  компиляторов: классический Intel (icc), LCC (lcc) и NVHPC(pgcc), указывать не
  требуется, учитывается автоматически;

- `_COUNTOF_NS_REFUSE_VLA` - нет поддержки VLA (аргументы VLA вызывают ошибку компиляции). Используется по умолчанию, если определены символы
  `__STDC_NO_VLA__` или `_MSC_VER` (MS Visual Studio 2019 и позднее);

- `_COUNTOF_NS_WANT_KR` - использовать идиому K&R без использования `__typeof__()`, `typeof()` и т.п. Для C, используется по умолчанию если `_MSC_VER < 1939` (MS Visual Studio 2017 и ранее);

- `_COUNTOF_NS_WANT_STDC` - не использовать расширения: `__typeof__()` и
  другие;

- `_COUNTOF_NS_WANT_VLA_BUILTIN` - использовать встроенные функции
  (`__builtin_types_compatible_p()` для C или `__is_same()` для C++ и др.). Используется по умолчанию, если нет определений
  `__STDC_NO_VLA__`, `_MSC_VER`, `__POCC__` или `_COUNTOF_NS_WANT_STDC`;

- `_COUNTOF_NS_WANT_VLA_C11` - обеспечить поддержку VLA стандартными
  возможностями C11/C23, требует соответствия компилятора `6.5.6 Additive operators` C11 и выше, для всех типов аргументов. Для C, используется по умолчанию если определён символ `__POCC__` (Pelles C).

## Сравнение опций поддержки VLA

- Если приоритет - минимизация ошибок компиляции на различных компиляторах, то при потребности в VLA, следует предпочесть `_COUNTOF_NS_WANT_VLA_C11`;

- Если приоритет - корректность контроля, то предпочтительнее
  `_COUNTOF_NS_WANT_VLA_BUILTIN`, т.к. отсутствует зависимость контроля
  ограничений от ключей компилятора (но, к сожалению, проблемные компиляторы не поддерживают `__has_builtin()`).

Так же макрос `_COUNTOF_NS_WANT_VLA_BUILTIN` позволяет документировать
настройки компилятора:

```
$ icc -diag-error=1121 \
    '-D_countof_ns_must_compatible(p,t1,t2)=(0*sizeof((t1)(p)-(t2)(p)))' \
    -D_COUNTOF_NS_WANT_VLA_BUILTIN ...

$ pgcc --diag_error=nonstandard_ptr_minus_ptr \
    '-D_countof_ns_must_compatible(p,t1,t2)=(0*sizeof((t1)(p)-(t2)(p)))' \
    -D_COUNTOF_NS_WANT_VLA_BUILTIN ...

$ suncc -errwarn=E_BAD_POINTER_SUBTRACTION \
	'-D_countof_ns_must_compatible(p,t1,t2)=(0*sizeof((t1)(p)-(t2)(p)))' \
	-D_COUNTOF_NS_WANT_VLA_BUILTIN ...

$ xlc -qlanglvl=extc1x -qhaltonmsg=1506-068 \
      -qinclude=_countof_ns_must_compatible_xl.h
      -D_COUNTOF_NS_WANT_VLA_BUILTIN ...
```

Маловероятно, что кому-нибудь потребуется `_COUNTOF_NS_WANT_VLA_BUILTIN` для
MSVC, но для тестирования, это тоже возможно:

```
> cl /std:clatest /wd4116 /we4047 /we4048 ^
	 /FI_countof_ns_must_compatible_msvc.h ^
	 /D_COUNTOF_NS_WANT_VLA_BUILTIN ...
```

- В качестве альтернативы, для  Сlang, GNU и IntelLLVM возможно использование ключей: `-D_COUNTOF_NS_WANT_KR -Werror=sizeof-pointer-div -Werror=sizeof-array-decay -Werror=sizeof-array-argument`.
## Статус поддержки VLA

Данный заголовочный файл определяет:

- `_COUNTOF_NS_USE_BUILTIN`, если для поддержки VLA используется встроенные
  функции и т.п.

- `_COUNTOF_NS_USE_GENERIC`,  если используется `_Generic()`
  (`_COUNTOF_NS_VLA_UNSUPPORTED`);

- `_COUNTOF_NS_USE_SUBTRACTION`,  если для поддержки VLA используется вычитание указателей по C11;

- `_COUNTOF_NS_USE_TEMPLATE`,  если используются стандартные шаблоны C++
  (`_COUNTOF_NS_VLA_UNSUPPORTED`);

- `_COUNTOF_NS_VLA_UNSUPPORTED`, в случае, если не обеспечивается поддержка VLA;
