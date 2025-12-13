// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// C11/23 6.5.1.1 Generic selection
// Constraints
// (2) ...If a generic selection has no default generic association, its
// controlling expression shall have type compatible with exactly one of the
// types named in its generic association list.

#include "tac_defs.h"

#define unsafe(a)  (sizeof(*(a)) ? sizeof(a)/sizeof(*(a)) : 0)
#define g_bug_on_not_array(bt, a) _Generic(&(a), \
                                            bt (*)[unsafe(a)]: 0)

typedef int base_t;
typedef base_t array_t[42];

TAC_CHECK_FUNC(g_foo) {
    array_t a;
    const int good = !g_bug_on_not_array(base_t, a);
    tac_assert(good);
    tac_static_assert(!g_bug_on_not_array(base_t, a),
                      "Check return value");
    #if !TAC_DONT_FAIL
        base_t *p = a;
        const int abort_compiler = g_bug_on_not_array(base_t, p);
        (void)abort_compiler;
    #endif
}
