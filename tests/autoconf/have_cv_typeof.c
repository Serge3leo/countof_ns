// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#define check_by_subtraction(type_t, arr) { \
                type_t arr[42] = { 0 }; \
                tac_static_assert(sizeof(&arr - (__typeof__(arr) *)&arr), \
                        "(__typeof__(arr) *), " #type_t ", " #arr); \
                tac_static_assert(sizeof((__typeof__(*arr) (*)[42])&arr \
                        - &arr), "(__typeof__(*arr) (*)[42]), " \
                        #type_t ", " #arr); \
            }

#define check_by_Generic(type_t, arr) { \
                type_t arr[42] = { 0 }; \
                tac_static_assert(1 == _Generic(&*arr, __typeof__(*arr) *: 1, \
                                                      default: -1), \
                                 "__typeof__(*arr) *, " #type_t ", " #arr); \
                tac_static_assert( \
                        1 == _Generic(&arr, __typeof__(arr) *: 1, \
                                            default: -1), \
                        "__typeof__(arr) *, " #type_t ", " #arr); \
                tac_static_assert( \
                        1 == _Generic(&arr, __typeof__(*arr)(*)[42]: 1, \
                                            default: -1), \
                        "__typeof__(*arr)(*)[42], " #type_t ", " #arr); \
            }

TAC_CHECK_FUNC(cvt_foo) {
    check_by_subtraction(char, a);
    check_by_Generic(char, a);
    check_by_subtraction(volatile char, va);
    check_by_Generic(volatile char, va);
    check_by_subtraction(const char, ca);
    check_by_Generic(const char, ca);
    check_by_subtraction(const volatile char, cva);
    check_by_Generic(const volatile char, cva);
}
