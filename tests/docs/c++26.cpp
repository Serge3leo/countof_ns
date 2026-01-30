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
            throw exception("Must be VLA", ^^type);
        }
        int debug_level = 0;
        do {
            type = remove_extent(type);
            if (_not_vmt(type) && 0 == size_of(type)) {
                if (debug_level) {
                    throw exception("VLA has zero size elements,"
                                    " debug_level>0", ^^type);
                }
                throw exception("VLA has zero size elements,"
                                " 0==debug_level", ^^type);
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
                throw exception("Must be array or container", ^^type);
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

consteval bool is_variably_modified_size_of(info type);
class variably_modified_size_of {
    const bool variable_;
    const size_t size_of_;
    variably_modified_size_of() = delete;
 public:
    consteval variably_modified_size_of(info type);
    constexpr size_t size_of(void) const;
};
consteval bool is_variably_modified_extent(info type);
class variably_modified_extent {
    const bool variable_;
    const size_t size_;
    variably_modified_extent() = delete;
 public:
    consteval variably_modified_extent(info type);
    constexpr size_t size(void) const;
};

consteval bool is_variably_modified_size_of(info type) {
    #if !Clang_WORKAROUND
        return !is_pointer_type(type) &&
               !can_substitute(^^std::extent_v, {type});  // TODO: VLA of ZLA
    #else
        return false;
    #endif
}
static_assert(!is_variably_modified_size_of(^^decltype(a7)));
static_assert(!is_variably_modified_size_of(^^decltype(z0)));

consteval variably_modified_size_of::variably_modified_size_of(info type)
    : variable_(is_variably_modified_size_of(type)),
      size_of_(!variable_ ? std::meta::size_of(type)
                          : 1917)  // TODO: Implementation defined
{}
constexpr size_t variably_modified_size_of::size_of(void) const {
    if consteval {
        if (variable_) {
            throw std::logic_error(
                    "Can't use variable size at compile time");
        }
    }
    return size_of_;
}
static_assert(sizeof(a7) ==
                variably_modified_size_of(^^decltype(a7)).size_of());
static_assert(0 == variably_modified_size_of(^^decltype(z0)).size_of());

consteval bool is_variably_modified_extent(info type) {
    #if !Clang_WORKAROUND
        return !can_substitute(^^std::extent_v, {type});
    #else
        return false;
    #endif
}
static_assert(!is_variably_modified_extent(^^decltype(a7)));
static_assert(!is_variably_modified_extent(^^decltype(z0)));

consteval variably_modified_extent::variably_modified_extent(info type)
    : variable_(is_variably_modified_extent(type)),
      size_(!variable_
        #if !GNU_WORKAROUND
              ? extent(type)
        #else
              ? extract<size_t>(substitute(^^std::extent_v, {type}))
        #endif
              : 1917)  // TODO: Implementation defined
{
    if (0 >= rank(type)) {
        #if !Clang_WORKAROUND
            throw exception("Must be array", ^^type);
        #else
            throw "Must be array";
        #endif
    }
}
constexpr size_t variably_modified_extent::size(void) const {
    if consteval {
        if (variable_) {
            throw std::logic_error(
                    "Can't use variable size at compile time");
        }
    }
    return size_;
}
static_assert(7 == variably_modified_extent(^^decltype(a7)).size());
static_assert(0 == variably_modified_extent(^^decltype(z0)).size());


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

    printf("is_variably_modified_extent(^^decltype(a00)) = %d\n",
            is_variably_modified_extent(^^decltype(a00)));
    printf("is_variably_modified_extent(^^decltype(v20)) = %d\n",
            is_variably_modified_extent(^^decltype(v20)));
    printf("is_variably_modified_extent(^^decltype(v00)) = %d\n",
            is_variably_modified_extent(^^decltype(v00)));
    printf("is_variably_modified_extent(^^decltype(v02)) = %d\n",
            is_variably_modified_extent(^^decltype(v02)));
    printf("is_variably_modified_extent(^^decltype(vf20)) = %d\n",
            is_variably_modified_extent(^^decltype(vf20)));
    printf("is_variably_modified_extent(^^decltype(vf00)) = %d\n",
            is_variably_modified_extent(^^decltype(vf00)));
    printf("is_variably_modified_extent(^^decltype(vf02)) = %d\n",
            is_variably_modified_extent(^^decltype(vf02)));
    printf("is_variably_modified_extent(^^decltype(fv20)) = %d\n",
            is_variably_modified_extent(^^decltype(fv20)));
    printf("is_variably_modified_extent(^^decltype(fv00)) = %d\n",
            is_variably_modified_extent(^^decltype(fv00)));
    printf("is_variably_modified_extent(^^decltype(fv02)) = %d\n",
            is_variably_modified_extent(^^decltype(fv02)));

    printf("is_variably_modified_size_of(^^decltype(&a00)) = %d\n",
            is_variably_modified_size_of(^^decltype(&a00)));
    printf("is_variably_modified_size_of(^^decltype(&v20)) = %d\n",
            is_variably_modified_size_of(^^decltype(&v20)));
    printf("is_variably_modified_size_of(^^decltype(&v00)) = %d\n",
            is_variably_modified_size_of(^^decltype(&v00)));
    printf("is_variably_modified_size_of(^^decltype(&v02)) = %d\n",
            is_variably_modified_size_of(^^decltype(&v02)));
    printf("is_variably_modified_size_of(^^decltype(&vf20)) = %d\n",
            is_variably_modified_size_of(^^decltype(&vf20)));
    printf("is_variably_modified_size_of(^^decltype(&vf00)) = %d\n",
            is_variably_modified_size_of(^^decltype(&vf00)));
    printf("is_variably_modified_size_of(^^decltype(&vf02)) = %d\n",
            is_variably_modified_size_of(^^decltype(&vf02)));
    printf("is_variably_modified_size_of(^^decltype(&fv20)) = %d\n",
            is_variably_modified_size_of(^^decltype(&fv20)));
    printf("is_variably_modified_size_of(^^decltype(&fv00)) = %d\n",
            is_variably_modified_size_of(^^decltype(&fv00)));
    printf("is_variably_modified_size_of(^^decltype(&fv02)) = %d\n",
            is_variably_modified_size_of(^^decltype(&fv02)));

    printf("size_of(^^decltype(a00)) = %zu\n",
            size_of(^^decltype(a00)));
#if Clang_WORKAROUND
    printf("bug size_of(^^decltype(v22)) = %zu\n",
            size_of(^^decltype(v22)));
    assert(sizeof(v22) != size_of(^^decltype(v22)));
    printf("bug size_of(^^decltype(v20)) = %zu\n",
            size_of(^^decltype(v20)));
    printf("bug size_of(^^decltype(v00)) = %zu\n",
            size_of(^^decltype(v00)));
    printf("bug size_of(^^decltype(v02)) = %zu\n",
            size_of(^^decltype(v02)));
#endif
    printf("size_of(^^decltype(vf20)) = %zu\n",
            size_of(^^decltype(vf20)));
    assert(sizeof(vf20) == size_of(^^decltype(vf20)));
    printf("size_of(^^decltype(vf00)) = %zu\n",
            size_of(^^decltype(vf00)));
    assert(sizeof(vf00) == size_of(^^decltype(vf00)));
#if Clang_WORKAROUND
    printf("bug size_of(^^decltype(vf02)) = %zu\n",
            size_of(^^decltype(vf02)));
    printf("bug size_of(^^decltype(fv20)) = %zu\n",
            size_of(^^decltype(fv20)));
    printf("size_of(^^decltype(fv00)) = %zu\n",
            size_of(^^decltype(fv00)));
    printf("size_of(^^decltype(fv02)) = %zu\n",
            size_of(^^decltype(fv02)));
#endif

    printf("Ok __GNUC__=%d  __VERSION__='%s'\n",
            __GNUC__, __VERSION__);
}
