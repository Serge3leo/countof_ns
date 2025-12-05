// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#define uto_typeof(t)  __typeof__(t)

#define uto_check_by_Generic(type_t, arr) { \
                type_t arr[42] = { 0 }; \
                tac_static_assert(-1 == _Generic(&*arr, uto_typeof(*arr) *: 1, \
                                                        default: -1), \
                                 "uto_typeof(*arr) *, " #type_t ", " #arr); \
                tac_static_assert(-1 == _Generic(&arr, \
                                                 uto_typeof(*arr)(*)[42]: 1, \
                                                 default: -1), \
                        "uto_typeof(*arr)(*)[42], " #type_t ", " #arr); \
                (void)arr; \
            }

TAC_CHECK_FUNC(uto_foo) {
    uto_check_by_Generic(volatile char, va);
    uto_check_by_Generic(const char, ca);
    uto_check_by_Generic(const volatile char, cva);
}
