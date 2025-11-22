// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if __has_include(<stdcountof.h>)
    #include <stdcountof.h>
#endif

TAC_CHECK_FUNC(cz_foo) {
    int a[0][0];
    int b[5][0];
    int c[0][5];
    tac_static_assert(0 == countof(a) && 0 == countof(a[0]), "a[0][0]");
    tac_static_assert(5 == countof(b) && 0 == countof(b[0]), "b[5][0]");
    tac_static_assert(0 == countof(c) && 5 == countof(c[0]), "c[0][5]");
    (void)a, (void)b, (void)c;
}
