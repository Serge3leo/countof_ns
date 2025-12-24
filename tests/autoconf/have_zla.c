// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Check clang/gcc/intel(?)/nvidia/... extension for zero-length array

#include "tac_defs.h"

TAC_CHECK_FUNC(zla_foo) {
    int z[0];
    tac_static_assert(0 == sizeof(z), "0 == sizeof(z)");
    tac_static_assert(0 < sizeof(z[0]), "0 < sizeof(z[0])");
    tac_assert(0 == sizeof(z));
    tac_assert(0 < sizeof(z[0]));
    printf("ZLA - Ok\n");
}
