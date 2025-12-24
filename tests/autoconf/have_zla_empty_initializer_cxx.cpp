// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(zeixx_foo) {
    int a[] = {};
    (void)a[0];
    tac_static_assert(0 == sizeof(a), "ZLA empty initializer");
    tac_static_assert(0 < sizeof(a[0]), "0 < sizeof(a[0])");
    assert(0 == sizeof(a));
    assert(0 < sizeof(a[0]));
    printf("ZLA empty initializer - Ok\n");
}
