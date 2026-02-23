// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#if HAVE_ELVIS
    #error "HAVE_ELVIS, not broken"
#endif

#include "tac_defs.h"

TAC_CHECK_FUNC(be_foo) {
    char c;
    int a;
    tac_static_assert(sizeof(c) < (sizeof(a) ? sizeof(a) : sizeof(c)),
                      "Conditional left");
    tac_static_assert(sizeof(c) == ((size_t)0 ? (size_t)0 : sizeof(c)),
                      "Conditional right");
    tac_assert(sizeof(c) < (sizeof(a) ?: sizeof(c)));
    tac_assert(sizeof(c) == ((size_t)0 ?: sizeof(c)));
}
