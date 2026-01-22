// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <cstdio>
#include <type_traits>

int zla[0];
#if __NVCOMPILER || __SUNPRO_CC || (__clang__ && \
    __clang_major__ <= 16 && !__apple_build_version__ && !__INTEL_COMPILER)
    static_assert(std::is_array<decltype(zla)>::value, "std::is_array_v<>");
#else
    static_assert(!std::is_array<decltype(zla)>::value, "!std::is_array_v<>");
#endif
static_assert(0 == std::extent<decltype(zla)>::value, "0 == std::extent<>");
template <class T, size_t N>
constexpr static size_t size(const T (&)[N]) { return N; }
template <class T>
constexpr static size_t size(const T (&)[0]) { return 0; }
template <class T>
constexpr static size_t size(const T&) { return 1917; }
#if !__clang__ || __INTEL_COMPILER
    static_assert(0 == size(zla), "ZLA is T[0]");
#else
    static_assert(1917 == size(zla), "ZLA not T[0]");
#endif

int main(void) {
    printf("Ok\n");
}
