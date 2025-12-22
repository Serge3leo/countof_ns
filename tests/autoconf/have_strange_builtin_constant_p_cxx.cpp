// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if !HAVE_BUILTIN_CONSTANT_P_CXX && !HAVE_HIDDEN_BUILTIN_CONSTANT_P_CXX
    #error "Don't have __builtin_constant_p()"
#endif

#include <assert.h>

TAC_CHECK_FUNC(sbc_foo) {
    size_t n = 1917;
    int v[n];

    static_assert(25 == (!__builtin_constant_p(sizeof(v)) ? 25 : 10),
                  "25 == (!__builtin_constant_p(sizeof(v)) ? 25 : 10)");
    assert(       10 == (!__builtin_constant_p(sizeof(v)) ? 25 : 10));
}
