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
#include <typeinfo>
#include <vector>

using namespace std::meta;
#if !Clang_WORKAROUND
    template<class F>
    consteval bool has_exception(const F& f) {
        try {
            (void)f();
        } catch(...) {
            return true;
        }
        return false;
    }
#else
    #define has_exception(...)  (true)
#endif

int a7[7];
int z0[0];
std::array<int, 1> arr1;
int i;
int *p;



/*
 * <source>:166:12: error: variably modified type 'int[n22]' cannot be used as a template argument
  166 |     return can_substitute(^^not_vla_v, {type});
      |            ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
 */

#if !Clang_WORKAROUND
        // Тип аргумента не является изменяемым
    template<class T>
    constexpr bool _detect_not_vmt;
    consteval bool _not_vmt(info type) {
        return can_substitute(^^_detect_not_vmt, {type});
    }
        // TODO: Совместимость с обходом ошибки clang
    #define not_vmt(a)  _not_vmt(^^decltype(a))

        // Проверка, что аргумент - VLA
    consteval size_t _must_vla(info type) {
        if (!_not_vmt(type) && 0 == rank(type)) {
            throw exception(u8"Must be VLA", ^^type);
        }
        int debug_level = 0;
        do {
            type = remove_extent(type);
            if (_not_vmt(type) && 0 == size_of(type)) {
                if (debug_level) {
                    throw exception(u8"VLA has zero size elements,"
                                    u8" debug_level>0", ^^type);
                }
                throw exception(u8"VLA has zero size elements,"
                                u8" 0==debug_level", ^^type);
            }
            debug_level++;
        } while (rank(type));
        return 0;
    }
        // TODO: Совместимость с обходом ошибки clang
    #define must_vla(a)  _must_vla(^^decltype(a))
#else
    class no_t { char no_[1]; };
    class yes_t { long long yes_[2]; };
    template<class T>
    static yes_t _detect_not_vmt(const T&);
    static no_t _detect_not_vmt(...);
    #define not_vmt(a)  (sizeof(yes_t) == sizeof(_detect_not_vmt(a)))

    consteval size_t _must_vla(bool not_vmt, size_t rank) {
        if (!not_vmt && 0 == rank) {
            throw "Must be VLA";
        }
        return 0;
    }
    #define must_vla(a)  _must_vla(not_vmt(a), __array_rank(decltype(a)))
#endif
    // Число элементов VLA
#define _countof_ns_unsafe(a)  (sizeof(a)/(sizeof((a)[0]) ?: 2*sizeof(void *)))
#define _countof_vla(a)  (_countof_ns_unsafe(a) + must_vla(a))

    // Аргумент - контейнер
consteval bool has_size(bool not_vmt, info type) {
        //  TODO: not_vmt - workaround clang bug: crash at _not_vmt(type)
    if (not_vmt && is_class_type(type)) {
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

static_assert(!has_size(true, ^^decltype(a7)));
static_assert(!has_size(true, ^^decltype(z0)));
static_assert(has_size(true, ^^decltype(arr1)));
static_assert(!has_size(true, ^^decltype(p)));

    // Размер заглушка, только для успеха компиляции
constexpr size_t unthinkable = 1917;
    // Число элементов фиксированного массива, возможно, ZLA
consteval size_t count_of(bool not_vmt, info type) {
    if (not_vmt) {  // TODO: clang bug: crash at _not_vmt(type)
        if (1 <= rank(type)) {
            #if !GNU_WORKAROUND
                return extent(type);
            #else
                return extract<size_t>(substitute(^^std::extent_v, {type}));
            #endif
        } else if(!has_size(not_vmt, type)) {
            #if !Clang_WORKAROUND
                throw exception(u8"Must be array or container", ^^type);
            #else
                throw "Must be array or container";
            #endif
        }
    }
    return unthinkable;
}

static_assert(7 == count_of(true, ^^decltype(a7)));
static_assert(0 == count_of(true, ^^decltype(z0)));
static_assert(unthinkable == count_of(true, ^^decltype(arr1)));
static_assert(has_exception([]() consteval {
                                (void)count_of(true, ^^decltype(p));
                            }));

    // Число элементов контейнера
template<class C> requires (has_size(true, ^^C))
constexpr static auto cnt_size(const C& c) noexcept(noexcept(c.size())) {
    return c.size();
}
constexpr static size_t cnt_size(...) { return unthinkable; }

static_assert(unthinkable == cnt_size(a7));
static_assert(unthinkable == cnt_size(z0));
static_assert(1 == cnt_size(arr1));
static_assert(unthinkable == cnt_size(&p));

#define countof_26(a)  (!not_vmt(a) \
                        ? _countof_vla(a) \
                        : has_size(not_vmt(a), ^^decltype(a)) \
                           ? cnt_size(a) \
                           : count_of(not_vmt(a), ^^decltype(a)))

static_assert(7 == countof_26(a7));
static_assert(0 == countof_26(z0));
static_assert(1 == countof_26(arr1));
static_assert(has_exception([]() consteval { (void)countof_26(&p); }));

consteval bool is_variably_modified(info type);
consteval bool is_variably_modified_size_of(info type);
consteval auto variably_modified_size_of(info type) -> size_t (*)(void);
consteval bool is_variably_modified_extent(info type);
consteval auto variably_modified_extent(info type) -> size_t (*)(void);

size_t free_eval(void) {
    if consteval {
        throw "Don't use in consteval";
        return 42;
    }
    return 1917;
}

#if !Clang_WORKAROUND
    template<class T>
    constexpr bool _detect_variably_modified;
    consteval bool is_variably_modified(info type) {
        if (can_substitute(^^_detect_variably_modified, {type})) {
            (void)size_of(type);
            (void)(0 == rank(type) || extent(type));
            #if 1  // TODO: gcc strange or?
                for (;;) {
                    if (rank(type)) {
                        type = remove_extent(type);
                    } else if (is_pointer_type(type)) {
                        type = remove_pointer(type);
                    } else {
                        break;
                    }
                    if (!can_substitute(^^_detect_variably_modified, {type})) {
                        return true;
                    }
                    (void)size_of(type);
                    (void)(0 == rank(type) || extent(type));
                }
            #endif
            return false;
        }
        return true;
    }
#else
    consteval bool is_variably_modified(info type) { return false; }
#endif
consteval bool is_variably_modified_size_of(info type) {
    return is_variably_modified(type);
}
consteval auto variably_modified_size_of(info type) -> size_t (*)(void) {
    return free_eval;
}
consteval bool is_variably_modified_extent(info type) {
    return is_variably_modified(type);
}
consteval auto variably_modified_extent(info type) -> size_t (*)(void) {
    return free_eval;
}

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
    // (void)countof_26(vf20);  // Ошибка, отличие от _Countof
    // (void)countof_26(vf00);  // Ошибка, отличие от _Countof
    assert(0 == countof_26(vf02));
    int fv20[2][n0];
    int fv00[0][n0];
    int fv02[0][n2];
        // Нельзя использовать static_assert(), отличие от _Countof
    assert(0 == countof_26(fv20));  // Отличие от _Countof
    assert(0 == countof_26(fv00));
    assert(0 == countof_26(fv02));

    printf("typeid(a77).name()=%s\n",
            typeid(a77).name());
    printf("typeid(a70).name()=%s\n",
            typeid(a70).name());
    printf("typeid(a00).name()=%s\n",
            typeid(a00).name());
    printf("typeid(a07).name()=%s\n",
            typeid(a07).name());
//     printf("typeid(v20).name()=%s\n",
//             typeid(v20).name());
//     printf("typeid(v00).name()=%s\n",
//             typeid(v00).name());
//     printf("typeid(v02).name()=%s\n",
//             typeid(v02).name());
//     printf("typeid(vf20).name()=%s\n",  // gcc internal error
//             typeid(vf20).name());
//     printf("typeid(vf00).name()=%s\n",  // gcc internal error
//             typeid(vf00).name());
//     printf("typeid(vf02).name()=%s\n",
//             typeid(vf02).name());
//     printf("typeid(fv20).name()=%s\n",
//             typeid(fv20).name());
//     printf("typeid(fv00).name()=%s\n",
//             typeid(fv00).name());
//     printf("typeid(fv02).name()=%s\n",
//             typeid(fv02).name());

    printf("is_variably_modified(^^decltype(a00)) = %d\n",
            is_variably_modified(^^decltype(a00)));
    printf("is_variably_modified(^^decltype(v20)) = %d\n",
            is_variably_modified(^^decltype(v20)));
    printf("is_variably_modified(^^decltype(v00)) = %d\n",
            is_variably_modified(^^decltype(v00)));
    printf("is_variably_modified(^^decltype(v02)) = %d\n",
            is_variably_modified(^^decltype(v02)));
    printf("is_variably_modified(^^decltype(vf20)) = %d\n",
            is_variably_modified(^^decltype(vf20)));
    printf("is_variably_modified(^^decltype(vf00)) = %d\n",
            is_variably_modified(^^decltype(vf00)));
    printf("is_variably_modified(^^decltype(vf02)) = %d\n",
            is_variably_modified(^^decltype(vf02)));
    printf("is_variably_modified(^^decltype(fv20)) = %d\n",
            is_variably_modified(^^decltype(fv20)));
    printf("is_variably_modified(^^decltype(fv00)) = %d\n",
            is_variably_modified(^^decltype(fv00)));
    printf("is_variably_modified(^^decltype(fv02)) = %d\n",
            is_variably_modified(^^decltype(fv02)));

    printf("is_variably_modified(^^decltype(&a00)) = %d\n",
            is_variably_modified(^^decltype(&a00)));
    printf("is_variably_modified(^^decltype(&v20)) = %d\n",
            is_variably_modified(^^decltype(&v20)));
    printf("is_variably_modified(^^decltype(&v00)) = %d\n",
            is_variably_modified(^^decltype(&v00)));
    printf("is_variably_modified(^^decltype(&v02)) = %d\n",
            is_variably_modified(^^decltype(&v02)));
    printf("is_variably_modified(^^decltype(&vf20)) = %d\n",
            is_variably_modified(^^decltype(&vf20)));
    printf("is_variably_modified(^^decltype(&vf00)) = %d\n",
            is_variably_modified(^^decltype(&vf00)));
    printf("is_variably_modified(^^decltype(&vf02)) = %d\n",
            is_variably_modified(^^decltype(&vf02)));
    printf("is_variably_modified(^^decltype(&fv20)) = %d\n",
            is_variably_modified(^^decltype(&fv20)));
    printf("is_variably_modified(^^decltype(&fv00)) = %d\n",
            is_variably_modified(^^decltype(&fv00)));
    printf("is_variably_modified(^^decltype(&fv02)) = %d\n",
            is_variably_modified(^^decltype(&fv02)));

    printf("Ok __GNUC__=%d  __VERSION__='%s'\n",
            __GNUC__, __VERSION__);
}
