// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if defined __has_builtin
    #if __has_builtin (__builtin_types_compatible_p)
        #define btc_builtin_types_compatible_p(type1, type2)  \
                        __builtin_types_compatible_p(type1, type2)
    #endif
#endif
#ifndef btc_builtin_types_compatible_p
    #error "Don't have __builtin_types_compatible_p()"
#endif

TAC_CHECK_FUNC(btc_foo) {
    int a[1] = {0};
    int *b = 0;
    tac_static_assert(btc_builtin_types_compatible_p(__typeof__(a), int[1]),
                      "1");
    tac_static_assert(btc_builtin_types_compatible_p(__typeof__(&a), int(*)[1]),
                      "2");
    tac_static_assert(!btc_builtin_types_compatible_p(__typeof__(b), int[1]),
                      "3");
    tac_static_assert(!btc_builtin_types_compatible_p(__typeof__(&b), int(*)[1]),
                     "4");
    (void)a, (void)b;
}
