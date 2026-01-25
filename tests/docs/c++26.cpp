// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2026-01-25 21:04:23 - Создан.
//

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
int *p;

using namespace std::meta;

constexpr size_t unthinkable = 1917;

consteval bool has_size(info type) {
    if (is_class_type(type)) {
        auto ctx = access_context::current();
        auto ms = define_static_array(members_of(type, ctx)
                |std::views::filter(std::meta::is_function)
                |std::views::filter(std::meta::has_identifier)
                |std::views::filter([](const std::meta::info& m){
                    return identifier_of(m) == "size";
                }));
        return !ms.empty();
    }
    return false;
}
static_assert(!has_size(^^decltype(a7)));
static_assert(!has_size(^^decltype(z0)));
static_assert(has_size(^^decltype(arr1)));
static_assert(!has_size(^^decltype(p)));

consteval size_t count_of(info type) {
    if (1 <= rank(type)) {
        #if !GNU_WORKAROUND
            return extent(type);
        #else
            return extract<size_t>(substitute(^^std::extent_v, {type}));
        #endif
    } else if(!has_size(type)) {
        throw "Must array or container";
    }
    return unthinkable;
}

static_assert(7 == count_of(^^decltype(a7)));
static_assert(0 == count_of(^^decltype(z0)));
static_assert(unthinkable == count_of(^^decltype(arr1)));
// static_assert(unthinkable == count_of(^^decltype(p)));

template<class C> requires (has_size(^^C))
constexpr static auto cnt_size(const C& c) noexcept(noexcept(c.size())) {
    return c.size();
}
constexpr static size_t cnt_size(...) { return unthinkable; }

static_assert(unthinkable == cnt_size(a7));
static_assert(unthinkable == cnt_size(z0));
static_assert(1 == cnt_size(arr1));
static_assert(unthinkable == cnt_size(&p));

#define countof_26_fixcnt(a)  (has_size(^^decltype(a)) \
                               ? cnt_size(a) \
                               : count_of(^^decltype(a)))

static_assert(7 == countof_26_fixcnt(a7));
static_assert(0 == countof_26_fixcnt(z0));
static_assert(1 == countof_26_fixcnt(arr1));
#if !Clang_WORKAROUND
    consteval bool tst() {
        try {
            (void)countof_26_fixcnt(&p);
        } catch(...) {
            return true;
        }
        return false;
    }
    static_assert(tst());
#endif

template<bool IsArray>
constexpr static size_t zero_assert() noexcept {
    static_assert(IsArray, "Must be array");
    return 0;
}
class no_t { char no_[1]; };
class yes_t { long long yes_[2]; };
static_assert(sizeof(no_t) != sizeof(yes_t));
template<class T>
static yes_t match_not_vla(const T&);
static no_t match_not_vla(...);

#define _countof_ns_unsafe(a)  (sizeof(a)/(sizeof(a[0]) ?: 2*sizeof(void *)))
#if !Clang_WORKAROUND
    #define _countof_26_must_vla(a)  (zero_assert< \
                        sizeof(match_not_vla(a)) != sizeof(no_t) || \
                        1 <= rank(^^decltype(a))>())
#else
    #define _countof_26_must_vla(a)  (zero_assert< \
                        sizeof(match_not_vla(a)) != sizeof(no_t) || \
                        1 <= __array_rank(decltype(a))>())
#endif
#define _countof_26_vla(a)  (_countof_ns_unsafe(a) + _countof_26_must_vla(a))
consteval bool has_size(size_t sizeof_not_vla, info type) {
    if (sizeof_not_vla != sizeof(no_t)) {
        return has_size(type);
    }
    return false;
}
consteval size_t count_of(size_t sizeof_not_vla, info type) {
    if (sizeof_not_vla != sizeof(no_t)) {
        return count_of(type);
    }
    return unthinkable;
}
#define countof_26(a)  (sizeof(match_not_vla(a)) == sizeof(no_t) \
                        ? _countof_26_vla(a) \
                        : has_size(sizeof(match_not_vla(a)), ^^decltype(a)) \
                               ? cnt_size(a) \
                               : count_of(sizeof(match_not_vla(a)), \
                                          ^^decltype(a)))

static_assert(7 == countof_26(a7));
static_assert(0 == countof_26(z0));
static_assert(1 == countof_26(arr1));
#if !Clang_WORKAROUND
    consteval bool tst1() {
        try {
            (void)countof_26(&p);
        } catch(...) {
            return true;
        }
        return false;
    }
    static_assert(tst1());
#endif


int main(void) {
    printf("countof_26(a7) = %zu\n",
            countof_26(a7));
    printf("countof_26(z0) = %zu\n",
            countof_26(z0));
    printf("countof_26(arr1) = %zu\n",
            countof_26(arr1));
    std::vector<int> vec0;
    printf("countof_26(vec0) = %zu\n",
            countof_26(vec0));
    std::string str5 = "Hello";
    #if !GNU_WORKAROUND
        printf("countof_26(str5) = %zu\n",
                countof_26(str5));
        assert(5 == countof_26(str5));
    #endif
    assert(7 == countof_26(a7));
    assert(0 == countof_26(z0));
    assert(1 == countof_26(arr1));
    assert(0 == countof_26(vec0));
    int a77[7][7];
    int a70[7][0];
    int a07[0][7];
    int a00[0][0];
    static_assert(7 == countof_26(a77));
    static_assert(extent(^^decltype(a77)) ==
                  std::extent_v<decltype(a77)>);
    static_assert(7 == countof_26(a70));
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
    static_assert(0 == countof_26(a07));
    static_assert(extent(^^decltype(a07)) ==
                  std::extent_v<decltype(a07)>);
    static_assert(0 == countof_26(a00));
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

    printf("Ok __GNUC__=%d  __VERSION__='%s'\n",
            __GNUC__, __VERSION__);
}
