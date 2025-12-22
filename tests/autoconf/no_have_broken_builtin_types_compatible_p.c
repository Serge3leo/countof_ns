// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(bbtc_foo) {
    #if HAVE_BUILTIN_TYPES_COMPATIBLE_P || \
        HAVE_HIDDEN_BUILTIN_TYPES_COMPATIBLE_P
        size_t d1 = 1;
        int v[d1];

        tac_static_assert(sizeof(void*) == sizeof((int (**)[d1])&v -
                                                  (int (**)[d1])&v),
                          "sizeof(void*)");
        tac_static_assert(__builtin_types_compatible_p(int (**)[d1],
                                                       int (**)[d1]),
                          "__builtin_types_compatible_p");
    #else
        printf("Don't have _builtin_types_compatible_p()");
    #endif
}
