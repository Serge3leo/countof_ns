// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#if __GNUC__ > 4
    #define GNU_WORKAROUND  (1)
    // GNU 16.0.1 20260114 (experimental)
#endif
#if __clang__
    #define Clang_WORKAROUND  (1)
    // Clang 21.0.0git (https://github.com/Bloomberg/clang-p2996 9813722a72c07f47206d50604f0e8fd00781e067)
#endif

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <meta>
#include <ranges>
#include <string>
#if GNU_WORKAROUND
    #include <type_traits>
#endif
#include <vector>

int a7[7];
int z0[0];
std::array<int, 1> arr1;

template<bool IsArray>
constexpr static size_t zero_assert() noexcept {
    static_assert(IsArray, "Must be array");
    return 0;
}
constexpr size_t unthinkable = 1917;
constexpr size_t bias = 1;
using no_t = char [bias + false];
using yes_t = char [bias + true];
template<class T>
constexpr static bool has_size() {
    if constexpr (is_class_type(^^T)) {
        constexpr auto ctx = std::meta::access_context::current();
        constexpr auto ms = define_static_array(members_of(^^T, ctx)
                |std::views::filter(std::meta::is_function)
                |std::views::filter(std::meta::has_identifier)
                |std::views::filter([](const std::meta::info& m){
                    return identifier_of(m) == "size";
                }));
        return !ms.empty();
    }
    return false;
}
template<class T>
constexpr static auto match_size(const T&) -> char (*)[bias + has_size<T>()];
constexpr static auto match_size(...) -> no_t *;
static_assert(sizeof(no_t) == sizeof(*match_size(a7)));
static_assert(sizeof(no_t) == sizeof(*match_size(z0)));
static_assert(sizeof(yes_t) == sizeof(*match_size(arr1)));

template<class T>
static auto match_countof(const T&)
    #if !GNU_WORKAROUND
        -> char (*)[1 <= rank(^^T) ? bias + extent(^^T) : unthinkable];
    #else
        -> char (*)[1 <= rank(^^T) ? bias + std::extent_v<T> : unthinkable];
    #endif
static char (*match_countof(...))[unthinkable];  // For VLA

static_assert(bias + 7 == sizeof(*match_countof(a7)));
static_assert(bias + 0 == sizeof(*match_countof(z0)));
static_assert(unthinkable == sizeof(*match_countof(arr1)));

template<class C>
constexpr static size_t _countof_26_container(const C& c) {
    if constexpr (sizeof(*match_size(c)) == sizeof(yes_t)) {
        return c.size();
    }
    return unthinkable;
}
constexpr static size_t _countof_26_container(...) {  // For VLA
    return unthinkable;
}
static_assert(unthinkable == _countof_26_container(a7));
static_assert(unthinkable == _countof_26_container(z0));
static_assert(1 == _countof_26_container(arr1));

#define countof_26_fixcnt(a)  (sizeof(*match_size(a)) == sizeof(yes_t) \
                               ? _countof_26_container(a) \
                               : sizeof(*match_countof(a)) - bias)
static_assert(7 == countof_26_fixcnt(a7));
static_assert(0 == countof_26_fixcnt(z0));
static_assert(1 == countof_26_fixcnt(arr1));

template<class T>
constexpr static yes_t *match_not_vla(const T&);
constexpr static no_t *match_not_vla(...);
#define _countof_ns_unsafe(a)  (sizeof(a)/(sizeof(a[0]) ?: 2*sizeof(void *)))
#if !Clang_WORKAROUND
    #define countof_26_must_vla(a)  (zero_assert< \
                        sizeof(*match_not_vla(a)) != sizeof(no_t) || \
                        1 <= rank(^^decltype(a))>())
#else
    #define countof_26_must_vla(a)  (zero_assert< \
                        sizeof(*match_not_vla(a)) != sizeof(no_t) || \
                        1 <= __array_rank(decltype(a))>())
#endif
#define countof_26_vla(a)  (_countof_ns_unsafe(a) + countof_26_must_vla(a))
#define countof_26(a)  (sizeof(*match_not_vla(a)) == sizeof(no_t) \
                        ? countof_26_vla(a) \
                        : sizeof(*match_size(a)) == sizeof(yes_t) \
                            ? _countof_26_container(a) \
                            : sizeof(*match_countof(a)) - bias)
static_assert(7 == countof_26(a7));
static_assert(0 == countof_26(z0));
static_assert(1 == countof_26(arr1));

int main(void) {
    printf("countof_26_fixcnt(a7) = %zu\n",
            countof_26_fixcnt(a7));
    printf("countof_26_fixcnt(z0) = %zu\n",
            countof_26_fixcnt(z0));
    printf("countof_26_fixcnt(arr1) = %zu\n",
            countof_26_fixcnt(arr1));
    std::vector<int> vec0;
    printf("countof_26_fixcnt(vec0) = %zu\n",
            countof_26_fixcnt(vec0));
    std::string str5 = "Hello";
    #if !GNU_WORKAROUND
        printf("countof_26_fixcnt(str5) = %zu\n",
                countof_26_fixcnt(str5));
        assert(5 == countof_26_fixcnt(str5));
    #endif
    assert(7 == countof_26_fixcnt(a7));
    assert(0 == countof_26_fixcnt(z0));
    assert(1 == countof_26_fixcnt(arr1));
    assert(0 == countof_26_fixcnt(vec0));
    int a77[7][7];
    int a70[7][0];
    int a07[0][7];
    int a00[0][0];
    static_assert(7 == countof_26_fixcnt(a77));
    static_assert(extent(^^decltype(a77)) ==
                  std::extent_v<decltype(a77)>);
    static_assert(7 == countof_26_fixcnt(a70));
    #if !GNU_WORKAROUND
        printf("extent(^^decltype(a70)) = %zu\n",
                extent(^^decltype(a70)));
        static_assert(extent(^^decltype(a70)) ==
                      std::extent_v<decltype(a70)>);
    #else
        static_assert(extent(^^decltype(a70)) !=
                      std::extent_v<decltype(a70)>,
                      "GNU 16.0.1 20260114 (experimental)");
        printf("std::extent_v<decltype(a70) = %zu\n",
                std::extent_v<decltype(a70)>);
    #endif
    static_assert(0 == countof_26_fixcnt(a07));
    static_assert(extent(^^decltype(a07)) ==
                  std::extent_v<decltype(a07)>);
    static_assert(0 == countof_26_fixcnt(a00));
    static_assert(extent(^^decltype(a00)) ==
                  std::extent_v<decltype(a00)>);
    size_t n2 = 2;
    int v22[n2][n2];
    assert(2 == countof_26(v22));
    #if !Clang_WORKAROUND
        printf("rank(^^int[n2][n2]) = %zu\n",
                rank(^^int[n2][n2]));
        printf("rank(^^decltype(v22)) = %zu\n",
                rank(^^decltype(v22)));
    #else
        printf("__array_rank(int[n2][n2]) = %zu\n",
                __array_rank(int[n2][n2]));
        printf("__array_rank(int[n2][n2]) = %zu\n",
                __array_rank(decltype(v22)));
    #endif
    size_t n0 = 0;
    int v20[n2][n0];
    int v00[n0][n0];
    int v02[n0][n2];
    assert(0 == countof_26(v20));  // Отличие от _Countof
    assert(0 == countof_26(v00));
    assert(0 == countof_26(v02));
    int vf20[n2][0];
    int vf00[n0][0];
    int vf02[n0][2];
    #if !GNU_WORKAROUND
        assert(0 == countof_26(vf20));  // Отличие от _Countof
        assert(0 == countof_26(vf00));
    #endif
    assert(0 == countof_26(vf02));
    int fv20[2][n0];
    int fv00[0][n0];
    int fv02[0][n2];
    assert(0 == countof_26(fv20));  // Отличие от _Countof
    assert(0 == countof_26(fv00));
    assert(0 == countof_26(fv02));

    printf("Ok %s\n", __VERSION__);
}
