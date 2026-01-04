// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(bs_foo) {
#if (__cplusplus && HAVE_VLA_CXX) || (!__cplusplus && !__STDC_NO_VLA__)
    volatile size_t m = 1917;
    volatile size_t n = 10;
    volatile int v[m][n];
    size_t i = 25;
    tac_assert(sizeof(int) == sizeof(v[i++][0]));
    tac_assert(25 == i);
    tac_assert(n*sizeof(int) == sizeof(v[i++]));
    tac_assert(26 == i);
#endif
}
