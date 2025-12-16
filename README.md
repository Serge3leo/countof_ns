[![CMake on multiple platforms](https://github.com/Serge3leo/countof_ns/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Serge3leo/countof_ns/actions/workflows/cmake-multi-platform.yml)

# C2y `countof()` platform independent implementation with C23/C++11

`countof_ns()` is a portable implementation of the macro `countof()` draft C2y by means of the C23/C++11 standards or using extensions
of the C language standards of previous editions, which are supported by most
compilers.

Incomplete list of supported compilers: Clang, GNU gcc, Intel (classic icc),
IntelLLVM (icx), LCC (MCST Elbrus), MSVC (Visual Studio 2022), NVHPC (NVIDIA
HPC Compiler), Pelles C, PGI (The Portland Group(?)), SunPro (Oracle Developer
Studio).

## Table of Contents

- [Background](#background)
- [Install](#install)
- [Usage](#usage)
- [Rationale](#rationale)
- [Contributing](#contributing)
- [License](#license)

## Background

### Any optional sections

## Install

Implementing the macro `countofi_ns()` consists of a single, dependency-free
file [`include/countof_ns.h`](include/countof_ns.h).  You can simply copy this
file to the desired location on the header search paths.

Alternatively, you can use `FetchContent` in your cmake project:

```
include(FetchContent)
FetchContent_Declare(
    CountofNS
    GIT_REPOSITORY https://github.com/Serge3leo/countof_ns.git
    GIT_TAG  71aa5528b84dbdc168a4750e4621a209ae5903c2 # v0.1pre-examples
)
FetchContent_MakeAvailable(CountofNS)
```

See the example:
[`examples/cmake_fetch_content/CMakeLists.txt`](examples/cmake_fetch_content/CMakeLists.txt).


This module depends upon a knowledge of [Markdown]().

```

```

### Any optional sections

## Usage

```

```

## Rationale

### Any optional sections

## More optional sections

## Contributing

PRs accepted.

Small note: If editing the Readme, please conform to the
[standard-readme](https://github.com/RichardLitt/standard-readme)
specification.

## Disclaimer

Sorry for my best English. Alas, this file is actually a yandex translation of
[README.ru.md](README.ru.md) with minimal editorial changes.

## License

[BSD-2-Clause © 2025 Сергей Леонтьев (leo@sai.msu.ru).](../LICENSE)
