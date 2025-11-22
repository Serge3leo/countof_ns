// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if __has_include(<stdcountof.h>)
    #include <stdcountof.h>
#endif

#ifdef HAVE_ZLA
    #define CV_MIN  (0)
#else
    #define CV_MIN  (1)
#endif

TAC_CHECK_FUNC(cv_foo) {
    int m = CV_MIN;
    int f = 5;
    int a[m][m];
    int b[f][m];
    int c[m][f];
    printf("%zu == countof(a), %zu == countof(a[0]) for a[%d][%d]\n",
            countof(a), countof(a[0]), m, m);
    printf("%zu == countof(b), %zu == countof(b[0]) for b[%d][%d]\n",
            countof(b), countof(b[0]), f, m);
    printf("%zu == countof(c), %zu == countof(c[0]) for c[%d][%d]\n",
            countof(c), countof(c[0]), m, f);
    assert(m == countof(a) && m == countof(a[0]) && "a[0][0]");
    assert(f == countof(b) && m == countof(b[0]) && "b[5][0]");
    assert(m == countof(c) && f == countof(c[0]) && "c[0][5]");
    (void)a, (void)b, (void)c;
}
