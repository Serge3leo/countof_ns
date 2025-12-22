// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(bbc_foo) {
    #if HAVE_BUILTIN_CONSTANT_P_CXX || HAVE_HIDDEN_BUILTIN_CONSTANT_P_CXX
        volatile size_t n = 1917;
        volatile int v[n];
        tac_static_assert(!__builtin_constant_p(sizeof(v)),
                          "!__builtin_constant_p(sizeof(v))");
        tac_static_assert(!__builtin_constant_p(sizeof(int[n])),
                          "!__builtin_constant_p(sizeof(int[n]))");
    #else
        printf("Don't have __builtin_constant_p()\n");
    #endif
}
