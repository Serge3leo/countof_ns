[![CMake on multiple platforms](https://github.com/Serge3leo/countof_ns/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Serge3leo/countof_ns/actions/workflows/cmake-multi-platform.yml)

# Число элементов массива, C23/C++14 переносимая реализация

Макрос `countof_ns(array)` возвращает количество элементов в своем операнде.
Количество элементов определяется типом операнда. Результатом является целое
число. Для массивов переменной длины (VLA) операнд вычисляется, в противном случае операнд не вычисляется, и результат является целым константным выражением.

`countof_ns()` это переносимая реализация оператора `_Countf` (макроса
`countof`), определённого в проекте стандарта C2y с небольшими синтаксическими и семантическим отличиями.

Он реализован средствами стандартов C23/C++14 или с использованием расширений стандартов C11/C++14, которые поддерживаются большинством компиляторов. Неполный список совместимых компиляторов: Clang (clang), GNU (gcc), классический Intel (icc), IntelLLVM (icx), LCC (MCST Elbrus), MSVC (Visual Studio), NVHPC (NVIDIA HPC Compiler), Pelles C, PGI (The Portland Group(?)), SunPro (Oracle Developer Studio), XL (IBM® XL C/C++ for AIX®), XLClang (IBM Clang-based XL).

## Содержание

- [Установка](#установка)
- [Использование](#использование)
- [Участие](#участие)
- [Оговорка](#оговорка)
- [Лицензия](#лицензия)

## Установка

Реализация макроса `countof_ns()` состоит из одного, не имеющего зависимостей,
файла [`include/countof_ns/countof_ns.h`](include/countof_ns/countof_ns.h).  Этот файл можно просто
скопировать в необходимое место на путях поиска заголовочных файлов.

Как альтернативный вариант, можно в вашем cmake проекте использовать
`FetchContent`:

```
include(FetchContent)
FetchContent_Declare(
    CountofNS
    GIT_REPOSITORY https://github.com/Serge3leo/countof_ns.git
    GIT_TAG  e6b8a03f5c62a0866dda910379240f18d9082e0f # v0.6.0-pre-examples
)
FetchContent_MakeAvailable(CountofNS)
```

Пример использования FetchContent в проекте cmake смотрите:
[`examples/cmake_fetch_content/CMakeLists.txt`](examples/cmake_fetch_content/CMakeLists.txt).

## Примеры и тесты

В основном этот проект состоит из примеров использования и тестов. Используется система сборки `cmake`, тесты `ctest`, на `GitHub Actions` тесты запускаются с генераторами `Unix Makefiles` (`gmake`) и `Visual Studio 17 2022` (`MSBuild`).

Для ограниченного и упрощённого запуска сборки примеров и тестов:
- [`./examples-build.sh`](./examples-build.sh) - в интерфейсе командной строки FreeBSD/Linux/macOS;
- [`examples-build.bat`](./examples-build.bat) - в интерфейсе CMD Windows.

Для `./examples-build.sh` в опциональных аргументах могут быть заданы команды
компиляторов (для некоторых компиляторов, может потребоваться предварительно установить переменные окружения). Каталог сборки:
`build/<платформа>[_имя_компилятора]`.

А `examples-build.bat` проще всего запускать из Native Tools Command Prompt for
VS. Каталог сборки: `build/<версия VS>`.  После сборки тестов и первоначального
запуска тестов, в этом каталоге остаётся файл решения `countof_ns.sln` в
котором определены "глобальные" цели `ALL_BUILD` и `RUN_TESTS`.  Модифицировать и повторно запускать можно уже из среды `Visual Studio`.

Предупреждение. У проекта `cmake`/`ctest` по умолчанию, есть особенность,
необходимо сначала собрать `ALL_BUILD` и только потом запускать тесты сборкой
`RUN_TESTS`

### Примерные системные требования к тестам

- cmake 3.25 или выше;
- FreeBSD 14 или выше;
- Linux Debian 12 (Bookworm) или выше;
- Windows 10, Visual Studio 2022 (MSVC 19.44) или выше.

### Простой пример

Исходный код [`examples/examples/short_example.h`](examples/short_example.h).
Переменная `cmake` - `COUNTOF_NS_SHORT_EXAMPLE`, включена по умолчанию. Сборка
и запуск:

```
$ ./examples-build.sh

> examples-build.bat
```

| Цель                           | Комментарий                                                                                                                 |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------------- |
| `short_example`                | Файл короткого примера.                                                                                                    |
| `short_example.build_fail`     | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции. |
| `short_example_cxx`            | Файл короткого примера, собранного C++                                                                                     |
| `short_example_cxx.build_fail` | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции. |
### Сложный пример

Исходный код [`examples/examples/long_example.h`](examples/long_example.h).
Переменная `cmake` - `COUNTOF_NS_EXAMPLES`, по умолчанию. Сборка и запуск:

```
$ ./examples-build.sh -- -DCOUNTOF_NS_EXAMPLES=ON

> examples-build.bat -- "-DCOUNTOF_NS_EXAMPLES=ON"
```

| Цель                               | Комментарий                                                                                                                     |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------- |
| `long_example`                     | Файл короткого примера.                                                                                                         |
| `long_example.build_fail`          | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции.      |
| `long_example_с11`                 | Опциональный файл короткого примера, собранного с ключом EXAMPLE_VLA_C11_ENABLE, если компилятор поддерживает VLA.              |
| `long_example_c11.build_fail`      | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции.      |
| `long_example_bltn`                | Опциональный файл короткого примера, собранного с ключом EXAMPLE_VLA_BUILTIN_ENABLE, если компилятор поддерживает VLA.          |
| `long_example_bltn.build_fail`     | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции.      |
| `long_example_cxx`                 | Файл короткого примера, собранного C++                                                                                          |
| `long_example_cxx.build_fail`      | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции.      |
| `long_example_cxx_bltn`            | Опциональный файл кайл короткого примера, собранного C++ с ключом EXAMPLE_VLA_BUILTIN_ENABLE, если компилятор поддерживает VLA. |
| `long_example_cxx_bltn.build_fail` | В норме, файл не должен создаваться, т.к. при сборке определяется `-DEXAMPLE_FAIL` и должна происходить ошибка компиляции.      |
## Использование

### Использование с расширениями C/C++

`countof_ns()` так же применима, как к расширенным массивам нулевой длины, так и к массивам содержащим расширенные объекты нулевой длины ( [пустые структуры](https://gcc.gnu.org/onlinedocs/gcc/Empty-Structures.html), [объединения c массивом неопределённого размера](https://gcc.gnu.org/onlinedocs/gcc/Flexible-Array-Members-in-Unions.html) или [структуры с единственным массивом неопределённого размера](https://gcc.gnu.org/onlinedocs/gcc/Flexible-Array-Members-alone-in-Structures.html)). У С реализации `countof_ns()` есть одно ограничение:

```
#if !__cplusplus
    size_t a = countof_ns(int[0][0]);  // OK, =0
    size_t b = countof_ns(int[0][5]);  // OK, =0
    size_t c = countof_ns(int[5][0]);  // Compilation error
    int n = 5;
    size_t d = countof_ns(int[n][0]);  // Difference, for VLA of ZLA, return 0
#else
    static_assert(0 == countof_ns(int[0][0]));  // OK, =0
    static_assert(0 == countof_ns(int[0][5]));  // OK, =0
    static_assert(5 == countof_ns(int[5][0]));  // OK, =5
#endif
```

С++ реализация `countof_ns()` выдаёт результаты идентичные `countof()`.

## Участие

Замечания (issues), добавления или исправления (PR) - принимаются и
приветствуются.

Небольшое примечание: При редактировании Readme, пожалуйста, придерживайтесь
рекомендаций [standard-readme](https://github.com/RichardLitt/standard-readme).

## Оговорка

Извините, пока документ очень далёк от идеала. TODO

## Лицензия

[BSD-2-Clause © 2025 Сергей Леонтьев (leo@sai.msu.ru).](LICENSE)
