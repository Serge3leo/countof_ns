// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if __STDC_VERSION__ >= 202300L
    #define ss_constexpr  constexpr
#else
    #define ss_constexpr
#endif

#define ss_stm_expr(t, x)  ({ \
                ss_constexpr t v = (x); \
                (void)v; \
                (sizeof(v) ? sizeof(v) : 1 ); \
            })

TAC_CHECK_FUNC(ss_foo) {
    tac_static_assert(ss_stm_expr(int, 1) > 0,
                      "ss_stm_expr(int, 1) > 0");
#if 0  // TODO: clang/icx separate tests
    struct {
        int a[ss_stm_expr(size_t, 2)];
    } s = {{0}};  // clang/icx -
    int a[ss_stm_expr(size_t, 2)];
    tac_static_assert(sizeof(a) > 0,
                      "sizeof(a) > 0");
#endif
}
