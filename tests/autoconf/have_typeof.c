// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(cntf_foo) {
    int a = 1;
    typeof(a) b = 1;
    (void)a, (void)b;
}
