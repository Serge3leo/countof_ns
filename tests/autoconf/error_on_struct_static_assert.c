// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#if __STDC_VERSION__ < 201112L
    #warning "Seems before C11, __STDC_VERSION__ < 201112L"
#endif

// C11 6.7.10 Static assertions
// C23 6.7.11 Static assertions
// Constraints
// (2) The constant expression shall compare unequal to 0.

#include "tac_defs.h"

#define ssa_bug_on_zero(e)  sizeof(struct{tac_static_assert((e), "Check"); \
                                          short foo;})

TAC_CHECK_FUNC(ssa_foo) {
    const int good = ssa_bug_on_zero(1);
    (void)good;
    tac_static_assert(sizeof(short) == ssa_bug_on_zero(1),
                      "Check return value");
    #if !TAC_DONT_FAIL
        const int abort_compiler = ssa_bug_on_zero(0);
        (void)abort_compiler;
    #endif
}
