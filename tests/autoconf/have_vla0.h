// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Strictly speaking, VLA arrays with zero elements are UB.
//
// C23: J.2 Undefined behavior
//
// (72) The size expression in an array declaration is not a constant
//      expression and evaluates at program execution time to a nonpositive
//      value (6.7.6.2).
//
//
// However, a good implementation of countof_ns() should not crash if the C
// language extensions of the given compiler allow the unhindered creation of
// such arrays, retrieval of their size and the size of their sub-arrays
// without any errors or warnings.
//
// We check the possibility of creating and performing minimal operations with
// one-dimensional, two-dimensional and three-dimensional arrays.

#ifndef HAVE_VLA0_H_5115
#define HAVE_VLA0_H_5115

#include "tac_defs.h"

static void have_vla0(void) {
    size_t n0 = 0;
    int a1[n0];
    assert(n0 == sizeof(a1));
    assert(sizeof(int) == sizeof(a1[0]));

#if HAVE_VLA0_2DIM
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
#endif
}

#endif  // HAVE_VLA0_H_5115
