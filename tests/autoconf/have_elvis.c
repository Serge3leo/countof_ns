// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(e_foo) {
    char c;
    int a;
    tac_static_assert(sizeof(c) < (sizeof(a) ? sizeof(a) : sizeof(c)),
                      "Conditional left");
    tac_static_assert(sizeof(c) == ((size_t)0 ? (size_t)0 : sizeof(c)),
                      "Conditional right");
    #if !TAC_PREREQUISITES
        tac_static_assert(sizeof(c) < (sizeof(a) ?: sizeof(c)),
                          "Elvis left");
        tac_static_assert(sizeof(c) == ((size_t)0 ?: sizeof(c)),
                          "Elvis right");
    #endif
}
