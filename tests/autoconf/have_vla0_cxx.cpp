// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(vla_foo) {
    size_t n0 = 0;
    int a1[n0];
    assert(n0 == sizeof(a1));
    assert(sizeof(int) == sizeof(a1[0]));

    size_t m2 = 42;
    int a2[m2][sizeof(a1)];
    assert(n0 == sizeof(a2));
    assert(n0 == sizeof(a2[0]));
    assert(sizeof(int) == sizeof(a2[0][0]));
    for (size_t i = 0; i < m2; i++) {
        assert(n0 == sizeof(a2[i]));
        assert(sizeof(int) == sizeof(a2[i][0]));
    }

    int z2[sizeof(a1)][sizeof(a2[0])];
    assert(n0 == sizeof(z2));
    assert(n0 == sizeof(z2[0]));
    assert(sizeof(int) == sizeof(z2[0][0]));

    size_t l3 = 56;
    int a3[l3][m2][sizeof(a2[0])];
    assert(n0 == sizeof(a3));
    assert(n0 == sizeof(a3[0]));
    assert(n0 == sizeof(a3[0][0]));
    assert(sizeof(int) == sizeof(a3[0][0][0]));
    for (size_t i = 0; i < l3; i++) {
        assert(n0 == sizeof(a3[i]));
        for (size_t j = 0; j < m2; j++) {
            assert(n0 == sizeof(a3[i][j]));
            assert(sizeof(int) == sizeof(a3[i][j][0]));
        }
    }

    int z3[l3][sizeof(a3[0])][sizeof(a3[0][0])];
    assert(n0 == sizeof(z3));
    assert(n0 == sizeof(z3[0]));
    assert(n0 == sizeof(z3[0][0]));
    assert(sizeof(int) == sizeof(z3[0][0][0]));
    for (size_t i = 0; i < l3; i++) {
        assert(n0 == sizeof(z3[i]));
        assert(n0 == sizeof(z3[i][0]));
        assert(sizeof(int) == sizeof(z3[i][0][0]));
    }

    int zz3[sizeof(z3)][sizeof(z3[0])][sizeof(z3[0][0])];
    assert(n0 == sizeof(zz3));
    assert(n0 == sizeof(zz3[0]));
    assert(n0 == sizeof(zz3[0][0]));
    assert(sizeof(int) == sizeof(zz3[0][0][0]));

    assert(0 == n0);

    (void)zz3;
}
