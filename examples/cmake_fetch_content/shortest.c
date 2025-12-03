// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "countof_ns.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    int a[42][56] = { { 0 } };
    printf("countof_ns(a) = %zu countof_ns(a[0]) = %zu\n",
            countof_ns(a), countof_ns(a[0]));
    assert(42 == countof_ns(a));
    assert(56 == countof_ns(a[0]));

    #ifdef EXAMPLE_FAIL
        int *p = &a[0][0];

        printf("Fail countof_ns(p) = %zu\n",
               countof_ns(p));
    #endif
}
