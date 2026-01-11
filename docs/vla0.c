// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <stdio.h>

void foo(size_t m, size_t n, int (*v)[m][n]) {
    printf("m=%zu n=%zu sizeof(*v) = %zu sizeof(**v) = %zu\n",
            m, n, sizeof(*v), sizeof(**v));
}
int main(void) {
    for (size_t m = 0; m < 3; m++) {
        for (size_t n = 0; n < 3; n++) {
            int v[m][n];
            foo(m, n, &v);
        }
    }
}
