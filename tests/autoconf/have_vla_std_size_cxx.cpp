// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#include <vector>

TAC_CHECK_FUNC(vss_foo) {
    size_t n = 1917;
    int v[n];
    tac_assert(1917 == std::size(v));
}
