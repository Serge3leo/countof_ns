[![CMake on multiple platforms](https://github.com/Serge3leo/countof_ns/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Serge3leo/countof_ns/actions/workflows/cmake-multi-platform.yml)

# C2y `countof()` platform independent implementation with C23/C++14
The macro `countof_ns(array)` returns the number of elements in its operand. The number of elements is determined by the type of the operand. The result is an integer. For variable-length arrays (VLA), the operand is evaluated, otherwise the operand is not evaluated, and the result is an integer constant expression.

`countof_ns()` is a portable implementation of the macro `countof()` draft C2y
by means of the C23/C++14 standards or using extensions of the C language
standards of previous editions, which are supported by most compilers.

Incomplete list of supported compilers: Clang (clang), GNU (gcc), classic Intel (icc), IntelLLVM (icx), LCC (MCST Elbrus), MSVC (Visual Studio), NVHPC (NVIDIA HPC Compiler), Pelles C, PGI (The Portland Group(?)), SunPro (Oracle Developer Studio), XL (IBM® XL C/C++ for AIX®), XLClang (IBM Clang-based XL).
## Table of Contents
- [Install](#install)
- [Usage](#usage)
- [Contributing](#contributing)
- [Links](#links)
- [License](#license)
## Install
Implementing the macro `countofi_ns()` consists of a single, dependency-free
file [`include/countof_ns.h`](include/countof_ns.h).  You can simply copy this file to the desired location on the header search paths.

Alternatively, you can use `FetchContent` in your cmake project:
```
include(FetchContent)
FetchContent_Declare(
    CountofNS
    GIT_REPOSITORY https://github.com/Serge3leo/countof_ns.git
    GIT_TAG  411dceb5934ac86fac3de44b11ec47167835cf82 # v0.6.0-pre-examples
)
FetchContent_MakeAvailable(CountofNS)
```
See the example: [`examples/cmake_fetch_content/CMakeLists.txt`](examples/cmake_fetch_content/CMakeLists.txt).
## Usage
```c
#include "countof_ns/countof_ns.h"
```
For a more detailed description of the usage, see: [`include/countof_ns/countof_ns.h`](include/countof_ns/countof_ns.h).
### Use with C/C++ extensions
The macro `countof_ns()` is applicable to any arrays, including both extended zero-length arrays and arrays containing extended zero-length objects ([empty structures](https://gcc.gnu.org/onlinedocs/gcc/Empty-Structures.html ), [joins with an array of indeterminate size](https://gcc.gnu.org/onlinedocs/gcc/Flexible-Array-Members-in-Unions.html ) or [structures with a single array of indeterminate size](https://gcc.gnu.org/onlinedocs/gcc/Flexible-Array-Members-alone-in-Structures.html )). Unfortunately, in these cases, `countof_ns()` is different from `countof()`:
<!-- example: "diff_countof.h" -->
```c++
int a07[0][7];
int a00[0][0];
int a70[7][0];
volatile size_t n7 = 7;
int v70[n7][0];

assert(0 == countof_ns(a07));
assert(0 == countof_ns(a00));  // Successful resolution of uncertainty 0/0
#if !__cplusplus
		// The C version of the macro may not always resolve the 0/0 uncertainty.
	(void)countof_ns(a70);  // Compilation error
#else
	assert(7 == countof_ns(a70));
#endif
	// For VLA, the 0/0 uncertainty cannot be detected during compilation.
assert(0 == countof_ns(v70));  // The result differs from countof(v70)
```
<!-- endexample: "diff_countof.h" -->
For a discussion of the implementation, see the article: [The long-awaited operator `_Countof`](docs/Long-awaited_Countof.ru.md#реализация-countof_ns) (in Russian).
## Contributing
Issues or PRs are accepted and welcome.
## Disclaimer
Sorry for my best English. Alas, this file is actually a yandex translation of
[README.ru.md](README.ru.md) with minimal editorial changes.
## Links
- WG14: [N3369: The `_Lengthof` Operator](https://www.open-std.org/JTC1/SC22/WG14/www/docs/n3369.pdf);
- WG14: [N3469: Big Array Size Survey](https://www.open-std.org/JTC1/SC22/WG14/www/docs/n3469.htm);
- Stack Overflow (SO): [How do I determine the size of my array in C?](https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c);
- SO: [Array-size macro that rejects pointers](https://stackoverflow.com/questions/19452971/array-size-macro-that-rejects-pointers);
- SO: [Is there a way for countof() to test if its argument is an array?](https://stackoverflow.com/questions/44621553/is-there-a-way-for-countof-to-test-if-its-argument-is-an-array);
- ruSO: [How can I determine the number of elements of an array C?](https://ru.stackoverflow.com/q/1621716/430734) (in Russian).
## License
[BSD-2-Clause © 2025 Сергей Леонтьев (leo@sai.msu.ru).](LICENSE)
